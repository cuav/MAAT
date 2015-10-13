/*
  glcd.cpp - Arduino library support for graphic LCDs
  Copyright (c) 2008,2009,2010 Michael Margolis and Bill Perry

  vi:ts=4

  This file is part of the Arduino GLCD library.

  GLCD is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 2.1 of the License, or
  (at your option) any later version.

  GLCD is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with GLCD.  If not, see <http://www.gnu.org/licenses/>.

  This file contains high level functions based on the previous ks0108 library.
  The functions were inspired from code written and copyright by Fabian Maximilian Thiele.
  you can obtain a copy of his original work here:
  http://www.scienceprog.com/wp-content/uploads/2007/07/glcd_ks0108.zip

  The glcd class impliments high level graphics routines.
  It is derived from the glcd_Device class that impliments the protocol for sending and
  receiving data and commands to a GLCD device

*/

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include "glcd.h"
#include "SystemFont5x7.h"
#include "uart.h"
//#include "glcd_Config.h"


#define min(x,y) (x<y?x:y)

#define BITMAP_FIX // enables a bitmap rendering fix/patch


#define RD				4
#define WR				5
#define RS				6
#define RSET			7
#define BLK				7

#define CTRL_DDR	DDRC
#define CTRL_PORT	PORTC

#define DATA_DDR	DDRA
#define DATA_PORT	PORTA

#define BLED_DDR	DDRD
#define BLED_PORT	PORTD


//#define _BV(x) (1 << x)

const uint8_t OFFSET_X = 0x60;

#define FontRead(addr) pgm_read_byte(addr);

inline void delay()
{
    //asm("nop");
    //asm("nop");
    //asm("nop");
}

inline uint8_t ReadPgmData(const uint8_t* ptr)  // note this is a static function
{
    // note this is a static function
    return pgm_read_byte(ptr);
}

glcd::glcd()
{
    //glcd_Device::Inverted = NON_INVERTED;
    fgcolor = WHITE;
    bgcolor = BLACK;
//    brightness = 80;
    draw_dir = DIR_NOTSET;
    font = System5x7;
}

/**
 * Initilize the GLCD library and hardware
 *
 * @param invert specifices whether display is in normal mode or inverted mode.
 *
 * This should be called prior to any other graphic library function.
 * It does all the needed initializations including taking care of the
 * low level hardware initalization of the display device.
 *
 * The optional @em invert parameter specifies if the display should be run in a normal
 * mode, dark pixels on light background or inverted, light pixels on a dark background.
 *
 * To specify dark pixels use the define @b NON-INVERTED and to use light pixels use
 * the define @b INVERTED
 *
 * Upon completion of the initialization, then entire display will be cleared.
 * @note
 * This function can also be called to re-initliaze the display hardware.
 */



int glcd::Init(uint8_t contrast, uint8_t brightness/*uint8_t invert*/)
{
    //return(glcd_Device::Init(invert));
    CTRL_DDR  |= (_BV(RD) | _BV(WR) | _BV(RS) | _BV(RSET));
    CTRL_PORT |= (_BV(RD) | _BV(WR) | _BV(RS) | _BV(RSET));

	//BLED_PORT 
    BLED_DDR |= _BV(BLK);
    BLED_PORT &= ~_BV(BLK);
		
    TCCR2 = _BV(WGM21) | _BV(WGM20) | _BV(COM21) | _BV(CS22);// | _BV(CS21);// | _BV(CS20);
    //TCNT2 = 100;
    OCR2 = brightness;
    //OCR2 = birghtness;
		
    _delay_ms(50);
		
    CTRL_PORT &= ~_BV(RSET);
		
    _delay_ms(35);
		
    CTRL_PORT |= _BV(RSET);
		
		
    for (uint8_t i = 0; i < 4; i++)
        _delay_ms(250);

	DATA_DDR = 0xFF;

    SendCmd(0x30);
    SendCmd(0x94);
    SendCmd(0xd1);
    SendCmd(0x20);
    SendData(0x08);
    _delay_ms(5);

    SendCmd(0x20);
    SendData(0x0b);

    //SendCmd(0x81);
    //SendData(0x1f);
    //SendData(0x04);
    SetContrast(contrast);

    SendCmd(0xca);
    SendData(0x04);
    SendData(0x1f);
    SendData(0x00);

    SendCmd(0xa6);

    SendCmd(0xbb);
    SendData(0x01);

    SetDirection(DIR_ROW);
    /*
    SendCmd(0xbc);
    SendData(0x02);
    SendData(0x01);
    SendData(0x02);
    */

    SendCmd(0x31);

    SendCmd(0x32);
    SendCmd(0x04);
    SendData(0x00);
    SendData(0x02);

    // READ EEPROM
    /*
    SendCmd(0x30);
    SendCmd(0x07);
    SendData(0x19);
    SendCmd(0x31);
    SendCmd(0xcd);
    SendData(0x00);
    _delay_ms(100);
    SendCmd(0xfd);
    _delay_ms(100);
    SendCmd(0xcc);
    */
    SendCmd(0x30);
    //SendCmd(0xaf);
    SetActiveWindow(OffsetX-3, 0, OffsetX+Right, Bottom+1);

    /*    SendCmd(0x15);
        SendData(0x20);
        SendData(0x30);

        SendCmd(0x75);
        SendData(0);
        SendData(127);
      */
      
      
    SendCmd(0x5c);
    for (short i = 0; i < (Width+1) * (Height+2); i++)
        SendData(bgcolor);

    SendCmd(0xaf);
    

}

/**
 * Clear the lcd display
 *
 * @param color BLACK or WHITE
 *
 * Sets all the pixels on the display from 0,0 to GLCD.Width-1,GLCD.Height-1
 * to the specified color.
 *
 * Color is optional and defaults to WHITE.
 *
 * @note
 * If the display is in INVERTED mode, then the color WHITE will paint the screen
 * BLACK and the color BLACK will paint the screen WHITE.
 *
 *
 */

void glcd::ClearScreen()
{
    this->SetPixels(0,0,GLCD.Width-1,GLCD.Height-1, bgcolor);
    CursorToXY(0,0);  // home text position
}

/*
 * Drawline code is based on Bresenham's line algorithm
 * http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *
 * This routine replaces the Teiele's DrawLine() routine which was
 * much larger and much slower.
 *	-- bperrybap
 */

/*
 * First, define a few macros to make the DrawLine code below read more like
 * the wikipedia example code.
 */

#define _GLCD_absDiff(x,y) ((x>y) ?  (x-y) : (y-x))
#define _GLCD_swap(a,b) \
do\
{\
uint8_t t;\
	t=a;\
	a=b;\
	b=t;\
} while(0)

/**
 * Draw a line
 *
 * @param x1 a value from 0 to GLCD.Width-1  indicating start x coordinate
 * @param y1 a value fron 0 to GLCD.Height-1 indicating start y coordinate
 * @param x2 a value from 0 to GLCD.Width-1  indicating end x coordinate
 * @param y2 a value fron 0 to GLCD.Height-1 indicating end y coordinate
 * @param color BLACK or WHITE
 *
 * Draws a line starting at x1,y2 and ending at x2,y2.
 *
 * Color is optional and defaults to BLACK.
 *
 * @see DrawHLine()
 * @see DrawVLine()
 *
 */

void glcd::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint8_t deltax, deltay, x,y, steep;
    int8_t error, ystep;

#ifdef XXX
    /*
     * Rather than fudge up coordinates that are outside of range,
     * It is better to allow the algorithm to run with them "as is"
     * so that any pixels on the line that land inside the valid display
     * coordinates will be drawn properly.
     *
     * If the coordinates are patched, then the line drawn will not
     * be anything close to the original slope as the endpoint coordinates
     * will have been altered.
     */

    if(x1>=DISPLAY_WIDTH) x1=0;
    if(x2>=DISPLAY_WIDTH) x2=0;
    if(y1>=DISPLAY_HEIGHT) y1=0;
    if(y2>=DISPLAY_HEIGHT) y2=0;
#endif

    steep = _GLCD_absDiff(y1,y2) > _GLCD_absDiff(x1,x2);

    if ( steep )
    {
        _GLCD_swap(x1, y1);
        _GLCD_swap(x2, y2);
    }

    if (x1 > x2)
    {
        _GLCD_swap(x1, x2);
        _GLCD_swap(y1, y2);
    }

    deltax = x2 - x1;
    deltay =_GLCD_absDiff(y2,y1);
    error = deltax / 2;
    y = y1;
    if(y1 < y2) ystep = 1;
    else ystep = -1;

    uint8_t d[3];

    for(x = x1; x <= x2; x++)
    {
        /*    if (steep) this->SetDot(y,x, fgcolor);
            else this->SetDot(x,y, fgcolor);
            error = error - deltay;
            if (error < 0)
            {
                y = y + ystep;
                error = error + deltax;
            }*/

        error = error - deltay;

        if (steep)
            this->SetDot(y, x, fgcolor);
        else
        {
            d[x%3] = fgcolor;
            //error -= deltay;
            if (x%3 == 2 || x == x2 || error < 0)
            {
                this->SetCol(x/3, y, d[0], d[1], d[2]);
                d[0] = d[1] = d[2] = bgcolor;
            }
        }

        if (error < 0)
        {
            y = y+ ystep;
            error = error + deltax;
        }

    }
}

/**
 * Draw a rectangle of given width and height
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color BLACK or WHITE
 *
 * Draws a rectangle with the specified width and height.
 * The upper left corner at x,y and the lower right
 * corner at x+width,y+width.
 *
 * The length of the horizontal sides will be width+1 pixels
 * The length of the vertical sides will be height+1 pixels
 *
 * Color is optional and defaults to BLACK.
 *
 * @note The width and height parameters work differently than FillRect()
 *
 * @see FillRect()
 * @see InvertRect()
 * @see DrawRoundRect()
 */

void glcd::DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    DrawVLine(x, y, height);			    // left
    DrawVLine(x+width, y, height);		// right
    DrawHLine(x, y, width);				// top
    DrawHLine(x, y+height, width);		// bottom
}

/**
 * Draw a rectangle with rounder corners
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param radius ????
 * @param color BLACK or WHITE
 *
 * Draws a rectangle the same as DrawRect() but with rounded corners.
 * Radius is a value from 1 to half the height or width of the rectangle.
 * (what does that really mean?????)
 * Which is it, or is it a the smaller of the two?
 * FIXME FIXME need more description here.
 *
 * @see DrawRect()
 */

void glcd::DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius)
{
    int16_t tSwitch;
    uint8_t x1 = 0, y1 = radius;
    tSwitch = 3 - 2 * radius;

    this->DrawVLine(x, y+radius, height-(2*radius));			// left
    this->DrawVLine(x+width, y+radius, height-(2*radius));	// right

    while (x1 <= y1)
    {
        this->SetDot(x+radius - x1, y+radius - y1, fgcolor);
        this->SetDot(x+radius - y1, y+radius - x1, fgcolor);

        this->SetDot(x+width-radius + x1, y+radius - y1, fgcolor);
        this->SetDot(x+width-radius + y1, y+radius - x1, fgcolor);

        this->SetDot(x+width-radius + x1, y+height-radius + y1, fgcolor);
        this->SetDot(x+width-radius + y1, y+height-radius + x1, fgcolor);

        this->SetDot(x+radius - x1, y+height-radius + y1, fgcolor);
        this->SetDot(x+radius - y1, y+height-radius + x1, fgcolor);

        if (tSwitch < 0)
        {
            tSwitch += (4 * x1 + 6);
        }
        else
        {
            tSwitch += (4 * (x1 - y1) + 10);
            y1--;
        }
        x1++;
    }

    this->DrawHLine(x+radius, y, width-(2*radius));			// top
    this->DrawHLine(x+radius, y+height, width-(2*radius));	// bottom

}

/**
 * Fill a Rectangle
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color BLACK or WHITE
 *
 * Fills a rectanglular area of the specified width and height.
 *
 * The resulting rectangle covers an area @em width pixels wide by @em height pixels
 * tall starting from the pixel at x,y.
 *
 * The upper left corner at x,y and the lower right corner at x+width-1,y+width-1.
 *
 *
 * The length of the horizontal sides will be width pixels
 * The length of the vertical sides will be height pixels
 *
 * Color is optional and defaults to BLACK.
 *
 * @note The width and height parameters work differently than DrawRect()
 *
 *
 * @warning FillRect() behavior has changed from the previous versions of the ks0108 library.
 *	The filled rectangle will be one pixel smaller in width and height than the old version.
 *	This change was to make the functionality consistent with the way
 *	Java and C# create filled rectangles
 *
 * @see DrawRect()
 * @see InvertRect()
 */

void glcd::FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    this->SetPixels(x,y,x+width,y+height,color);
}

/**
 * Invert a Rectangular area
 *
 * @param x the x coordinate of the upper left corner of the rectangle
 * @param y the y coordinate of the upper left corner of the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 *
 * Inverts the pixels in the rectanglular area of the specified width and height.
 * BLACK pixels becom WHITE and WHITE pixels become BLACK.
 *
 * See FillRect() for full the full details of the rectangular area.
 *
 * @note The width and height parameters work differently than DrawRect()
 *
 * @warning InvertRect() behavior has changed from the previous versions of the ks0108 library.
 *	The inverted rectangle will be one pixel smaller in width and height than the old version.
 *
 * @see DrawRect()
 * @see FillRect()
 */

/*
void glcd::InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{

    uint8_t mask, pageOffset, h, i, data, tmpData;
    height++;

    pageOffset = y%8;
    y -= pageOffset;
    mask = 0xFF;
    if(height < 8-pageOffset)
    {
        mask >>= (8-height);
        h = height;
    }
    else
    {
        h = 8-pageOffset;
    }
    mask <<= pageOffset;


    // First do the fractional pages at the top of the region

    //glcd_Device::GotoXY(x, y);

    for(i=0; i<=width; i++)
    {
        data = this->ReadData();
        tmpData = ~data;
        data = (tmpData & mask) | (data & ~mask);
        this->WriteData(data);
    }


    // * Now do the full pages

    while(h+8 <= height)
    {
        h += 8;
        y += 8;
        //glcd_Device::GotoXY(x, y);

        for(i=0; i<=width; i++)
        {
            data = this->ReadData();
            this->WriteData(~data);
        }
    }


    //  Now do the fractional pages aat the bottom of the region

    if(h < height)
    {
        mask = ~(0xFF << (height-h));
        //glcd_Device::GotoXY(x, y+8);

        for(i=0; i<=width; i++)
        {
            data = this->ReadData();
            tmpData = ~data;
            data = (tmpData & mask) | (data & ~mask);
            this->WriteData(data);
        }
    }
}
*/


/**
 * Set LCD Display mode
 *
 * @param invert Inverted mode
 *
 * Sets the graphical state mode for the entire LCD display
 * to @b NON_INVERTED (BLACK colorerd pixeld are dark)
 * or @b INVERTED (WHITE colored pixels are dark)
 *
 */

void glcd::SetDisplayMode(uint8_t invert)    // was named SetInverted
{

    /*    if(this->Inverted != invert)
        {
            this->InvertRect(0,0,DISPLAY_WIDTH-1,DISPLAY_HEIGHT-1);
            this->Inverted = invert;
        }
        */
}

/**
 * Draw a glcd bitmap image
 *
 * @param bitmap a ponter to the bitmap data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param color BLACK or WHITE
 *
 * Draws a bitmap image with the upper left corner at location x,y
 * The bitmap data is assumed to be in program memory.
 *
 * Color is optional and defaults to BLACK.
 *
#ifdef NOTYET
 * @see DrawBitmapXBM()
#endif
 */
/*
void glcd::DrawGBitmap_P(Image_t bitmap, uint8_t x, uint8_t y)
{
	uint8_t width, height;
	uint8_t offset_x;
	uint16_t i, j;
	
	width = ReadPgmData(bitmap++);
	height = ReadPgmData(bitmap++);
	
	SetDirection(DIR_ROW);
	//offset_x = x % 3;

	
    SetActiveWindow(x + OffsetX, y, x + OffsetX + width - 1, y + height - 1);
    SendCmd(0x5c);

    for (i = 0; i < ((width * height) >> 2); i++)
    {
    	uint8_t data = ReadPgmData(bitmap++);
    	SendData((data << 6) | 0x3F);
    	SendData((data << 4) | 0x3F);
    	SendData((data << 2) | 0x3F);
    	SendData((data) | 0x3F);
    }
}*/

void glcd::DrawMBitmap_P(Image_t bitmap, uint8_t x, uint8_t y)
{
	uint8_t width, height;
	uint16_t i = 0, j = 0;
	
	width = ReadPgmData(bitmap++);
	height = ReadPgmData(bitmap++);
	
	SetDirection(DIR_ROW);
		//offset_x = x % 3;
	x = (x / 3) * 3;
    SetActiveWindow(x + OffsetX, y, x + OffsetX + width - 1, y + height - 1);
    SendCmd(0x5c);

    while (i < width * height)
    {
    		uint8_t data = ReadPgmData(bitmap++);
    		for (j = 0; j < 8 & i < width * height; j++, i++)
    		{
    				SendData((data & 0x1)? fgcolor:bgcolor);
    				data = data >> 1;
    		}
    }
}


void glcd::DrawBitmap(Image_t bitmap, uint8_t x, uint8_t y)
{
    uint8_t width, height;
    uint8_t i, j;

    // width = ReadPgmData(bitmap++);
    //height = ReadPgmData(bitmap++);

#ifdef BITMAP_FIX // temporary ifdef just to show what changes if a new 
    // bit rendering routine is written.

    /*
     * In the absence of a new/better/proper bitmap rendering routine,
     * this routine will provide a temporary fix for drawing bitmaps that are
     * are non multiples of 8 pixels in height and start on non LCD page Y addresses.
     *
     * Note: nomally a routine like this should not have knowledge of how
     *	   how the lower level write routine works. But in this case it does.
     *
     *	Currently, the low level WriteData() routine ORs in the pixels when data spans
     *	a LCD memory page. So.....
     *
     * In order to ensure that the bitmap data is written to the pixels *exactly* as it
     * it defined in the bitmap data, i.e every black pixels is black and every white
     * pixel is white,...
     *
     * If height or y coordinate is not on a page boundary, clear the background first
     *	Techincally, this could be done all the time and it wouldn't hurt, it
     *	would just suck up a few more cycles.
     */
    if( (y & 7) || (height & 7))
    {
        this->FillRect(x, y, width, height, bgcolor);
    }
#endif
    /*
        for(j = 0; j < height / 8; j++)
        {
           // glcd_Device::GotoXY(x, y + (j*8) );
            for(i = 0; i < width; i++)
            {
                uint8_t displayData = ReadPgmData(bitmap++);
                if(color == BLACK)
                    this->WriteData(displayData);
                else
                    this->WriteData(~displayData);
            }
        }*/
}

#ifdef NOTYET

/**
 * Draw a glcd bitmap image in x11 XBM bitmap data format
 *
 * @param bitmapxbm a ponter to the glcd XBM bitmap data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param color BLACK or WHITE
 *
 * Draws a x11 XBM bitmap image with the upper left corner at location x,y
 * The glcd xbm bitmap data format consists of 1 byte of width followed by 1 byte of height followed
 * by the x11 xbm pixel data bytes.
 * The bitmap data is assumed to be in program memory.
 *
 * Color is optional and defaults to BLACK.
 *
 * @see DrawBitmapXBM_P()
 * @see DrawBitmap()
 */

void glcd::DrawBitmapXBM(ImageXBM_t bitmapxbm, uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t width, height;
    uint8_t bg_color;
    uint8_t *xbmbits;


    xbmbits = (uint8_t *) bitmapxbm;

    width = ReadPgmData(xbmbits++);
    height = ReadPgmData(xbmbits++);

    if(color == BLACK)
        bg_color = WHITE;
    else
        bg_color = BLACK;
    DrawBitmapXBM_P(width, height, xbmbits, x, y, color, bg_color);
}

/**
 * Draw a x11 XBM bitmap image
 *
 * @param width pixel width of the image
 * @param height pixel height of the image
 * @param xbmbits a ponter to the XBM bitmap pixel data
 * @param x the x coordinate of the upper left corner of the bitmap
 * @param y the y coordinate of the upper left corner of the bitmap
 * @param fg_color foreground color
 * @param bg_color background color
 *
 * Draws a x11 XBM bitmap image with the upper left corner at location x,y
 * The xbm bitmap pixel data format is the same as the X11 bitmap pixel data.
 * The bitmap data is assumed to be in program memory.
 *
 * @note All parameters are mandatory
 *
 * @see DrawBitmapXBM_P()
 * @see DrawBitmap()
 */


void glcd::DrawBitmapXBM_P(uint8_t width, uint8_t height, uint8_t *xbmbits,
                           uint8_t x, uint8_t y, uint8_t fg_color, uint8_t bg_color)
{
    uint8_t xbmx, xbmy;
    uint8_t xbmdata;

    /*
     * Traverse through the XBM data byte by byte and plot pixel by pixel
     */
    for(xbmy = 0; xbmy < height; xbmy++)
    {
        for(xbmx = 0; xbmx < width; xbmx++)
        {
            if(!(xbmx & 7))	// read the flash data only once per byte
                xbmdata = ReadPgmData(xbmbits++);

            if(xbmdata & _BV((xbmx & 7)))
                this->SetDot(x+xbmx, y+xbmy, fg_color); // XBM 1 bits are fg color
            else
                this->SetDot(x+xbmx, y+xbmy, bg_color); // XBM 0 bits are bg color
        }
    }

}

#endif // NOTYET

// the following inline functions were added 2 Dec 2009 to replace macros

/**
 * Draw a Vertical Line
 *
 * @param x a value from 0 to GLCD.Width-1
 * @param y a value from 0 to GLCD.Height-1
 * @param height a value from 1 to GLCD.Height-y-1
 * @param color color of line
 *
 * color of BLACK or WHITE is an optional parameter indicating pixel color, default is BLACK
 *
 * The line drawn will be height+1 pixels.
 *
 * @note This function was previously named DrawVertLine() in the ks0108 library
 *
 * @see DrawLine()
 * @see DrawHLine()
 */


void glcd::DrawVLine(uint8_t x, uint8_t y, uint8_t height)
{
    // this->FillRect(x, y, 0, length, color);
    this->SetPixels(x,y,x,y+height, fgcolor);
}

/**
 * Draw a Horizontal Line
 *
 * @param x a value from 0 to GLCD.Width-1
 * @param y a value from 0 to GLCD.Height-1
 * @param width a value from 1 to GLCD.Width-x-1
 * @param color BLACK or WHITE
 *
 * The line drawn will be width+1 pixels.
 *
 * color is an optional parameter indicating pixel color and defaults to BLACK
 *
 * @note This function was previously named DrawHoriLine() in the ks0108 library
 *
 * @see DrawLine()
 * @see DrawVLine()
 */

void glcd::DrawHLine(uint8_t x, uint8_t y, uint8_t width)
{
    // this->FillRect(x, y, length, 0, color);
    this->SetPixels(x,y, x+width, y, fgcolor);
}

void glcd::DrawHParallel(uint8_t x, uint8_t y, uint8_t width, uint8_t interval, uint8_t count)
{
		for (uint8_t i = 0; i < count; i++)
		{
				SetPixels(x, y, x+width, y, fgcolor);
				y+= interval;
		}
}

void glcd::DrawVParallel(uint8_t x, uint8_t y, uint8_t height, uint8_t interval, uint8_t count)
{
		for (uint8_t i = 0; i < count; i++)
		{
				SetPixels(x,y,x,y+height, fgcolor);
				x += interval;
		}
}
/**
 * Draw a Circle
 *
 * @param xCenter X coordinate of the center of the circle
 * @param yCenter Y coordinate of the center of the circle
 * @param radius radius of circle
 * @param color BLACK or WHITE
 *
 * Draws a circle of the given radius extending out from
 * the center pixel.
 * The circle will fit inside a rectanglular area bounded by
 * x-radius,y-radius and x+radius,y+radius
 *
 * Because the circle is drawn from the center pixel out,
 * the diameter will be 2 * radius +1 pixels.
 *
 * Color is optional and defaults to BLACK.
 *
 * @see FillCircle()
 */
void glcd::DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius)
{
    this->DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius, 2*radius, radius);
}

/**
 * Draw a Filled in a Circle
 *
 * @param xCenter X coordinate of the center of the circle
 * @param yCenter Y coordinate of the center of the circle
 * @param radius radius of circle
 * @param color WHITE or BLACK
 *
 * Draws a filled in circle of the given radius extending out from
 * the center pixel.
 *
 * See DrawCircle() for the full details on sizing.
 *
 * Color is optional and defaults to BLACK.
 *
 * @see DrawCircle()
 *
 */

void glcd::FillCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius)
{
    /*
     * Circle fill Code is merely a modification of the midpoint
     * circle algorithem which is an adaption of Bresenham's line algorithm
     * http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
     * http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
     *
     * While it looks different it is the same calculation that is in
     * DrawRoundRect()
     *
     * Note: FillCircle()
     *		could be modified to work like DrawCircle() by using
     *		a DrawRoundRect() function.
     * 		DrawRoundRect() first plots the cicrular quadrants and then draws
     *		the horizontal and verticl lines between them to create
     *		a rounded rectangle.
     *
     *		For fills of rounded rectangles, this type of circle fill algorithm
     *		would fill the upper and lower quadrants on each side of the rounded
     *		rectangle by connecting them vertically.
     *		When that was done, a filled rectangle would be neded need to fill the
     *		space between them.
     *		There really isn't an easy way to fill a rounded rectangle.
     *
     *		For now, it is limited to circles.
     *
     * 			--- bperrybap
     */

    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    uint8_t x = 0;
    uint8_t y = radius;

    /*
     * Fill in the center between the two halves
     */
    DrawLine(xCenter, yCenter-radius, xCenter, yCenter+radius);

    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        /*
         * Now draw vertical lines between the points on the circle rather than
         * draw the points of the circle. This draws lines between the
         * perimeter points on the upper and lower quadrants of the 2 halves of the circle.
         */

        DrawLine(xCenter+x, yCenter+y, xCenter+x, yCenter-y);
        DrawLine(xCenter-x, yCenter+y, xCenter-x, yCenter-y);
        DrawLine(xCenter+y, yCenter+x, y+xCenter, yCenter-x);
        DrawLine(xCenter-y, yCenter+x, xCenter-y, yCenter-x);
    }
}



int glcd::PutChar(uint8_t ch)
{
    if (ch < 0x20 || ch > 0x82)
        return 0;
        //ch = font[4];

    prog_uint8_t* start = font + 6 + (ch-0x20)*5;

    //SendCmd(0x5c);
    //for (uint8_t x = 0; x < 5; x++)
    //    uart_putc(ReadPgmData(start+x));


    for (uint8_t y = 0; y < 7; y++)
    {
        uint8_t msk = _BV(y);

        /*SendData(fgcolor);
        SendData(fgcolor);
        SendData(fgcolor);*/
        SendData((ReadPgmData(start) & msk)? fgcolor:bgcolor);
        SendData((ReadPgmData(start+1) & msk)? fgcolor:bgcolor);
        SendData((ReadPgmData(start+2) & msk)? fgcolor:bgcolor);
    }
    SendData(bgcolor);
    SendData(bgcolor);
    SendData(bgcolor);
    for (uint8_t y = 0; y < 7; y++)
    {
        uint8_t msk = _BV(y);
        SendData((ReadPgmData(start+3) & msk)? fgcolor:bgcolor);
        SendData((ReadPgmData(start+4) & msk)? fgcolor:bgcolor);
        SendData(bgcolor);
    }
    SendData(bgcolor);
    SendData(bgcolor);
    SendData(bgcolor);
    return 0;
}

void glcd::DrawString(uint8_t x, uint8_t y, const char* str, int8_t len)
{
    if (len < 0)
        len = strlen(str);
    SetDirection(DIR_COLUMN);
    SetActiveWindow(x+OffsetX, y, x + OffsetX + len*6 +  - 1, y+7);
    SendCmd(0x5c);
    while (*str)
    {
        PutChar(*(str++));
    }
}
//
// Font Functions
//



/*
 * Below here are text wrapper functions
 */

// override GotoXY to also call CursorToxy for backward compability
// with older v2 ks0108 library
// (older library didn't have seperate x & y for hardware/graphics vs text )
void glcd::GotoXY(uint8_t x, uint8_t y)
{
    //glcd_Device::GotoXY(x, y);
    CursorToXY(x,y);
}

void glcd::CursorToXY(uint8_t x, uint8_t y)
{
//    this->x = x;
    //this->y = y;
}

void glcd::SetDot(uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t d[3] = {255, 255, 255};
    x += OffsetX;
    /*SetActiveWindow(x, y, x+3, y+1);

    SendCmd(0x5d);
    SetPortDirection(false);
    d[0] = ReadData();
    d[0] = ReadData();
    d[1] = ReadData();
    d[2] = ReadData();
    uart_putc(d[0]);

    SetPortDirection(true);*/
    d[x%3] = color;
    SetActiveWindow(x, y, x+3, y+1);
    SendCmd(0x5c);
    SendData(d[0]);
    SendData(d[1]);
    SendData(d[2]);

    //SendCmd(0xee);
    /*SendCmd(0xe0);
    d[0] = ReadData();
    d[0] = ReadData();
    d[1] = ReadData();
    d[2] = ReadData();
    d[x%3] = color;
    SendData(d[0]);
    SendData(d[1]);
    SendData(d[2]);
    SendCmd(0xee);*/
}

void glcd::SetCol(uint8_t col, uint8_t row, uint8_t color1, uint8_t color2, uint8_t color3)
{
    SetActiveColWindow(col + 0x20, row, col+0x21, row+1);
    SendCmd(0x5c);
    SendData(color1);
    SendData(color2);
    SendData(color3);
}

void glcd::SetPixels(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    x1 += OffsetX;
    x2 += OffsetX;


    uint8_t col1 = x1/3;
    uint8_t col2 = x2/3;


    if (x1 % 3 != 0 || x2 % 3 != 2)
    {
        int block = col2 - col1 + 1;
        if (block == 1)
        {
            uint8_t d[3] = {255, 255, 255};
            for (uint8_t j = x1%3; j <= x2%3; j++)
                d[j] = color;
            SetActiveColWindow(col1, y1, col1, y2);
            SendCmd(0x5c);
            for (uint8_t i = 0; i <= y2-y1; i++)
            {
                SendData(d[0]);
                SendData(d[1]);
                SendData(d[2]);
            }
        }
        else if (block == 2)
        {
            SetPixels(x1-OffsetX, y1, col1*3+2-OffsetX, y2, color);
            SetPixels(col2*3-OffsetX, y1, x2-OffsetX, y2, color);
        }
        else
        {
            SetPixels(x1-OffsetX, y1, col1*3+2-OffsetX, y2, color);
            SetPixels((col1+1)*3-OffsetX, y1, col2*3-1-OffsetX, y2, color);
            SetPixels(col2*3-OffsetX, y1, x2-OffsetX, y2, color);
        }
    }
    else
    {

        //SendCmd(0x30);
        SetActiveColWindow(col1, y1, col2, y2);

        SendCmd(0x5c);
        // //
        for (short i = 0; i < (col2-col1+1)*(y2+1-y1); i++)
        {
            SendData(color);
            SendData(color);
            SendData(color);
        }
    }
}

void glcd::SendCmd(uint8_t cmd)
{
    CTRL_PORT &= ~(_BV(RS));
    DATA_PORT = cmd;
    delay();
    CTRL_PORT &= ~(_BV(WR));
    delay();
    CTRL_PORT |= _BV(WR);
}

void glcd::SendData(uint8_t data)
{
    CTRL_PORT |= _BV(RS);
    DATA_PORT = data;
    delay();
    CTRL_PORT &= ~(_BV(WR));
    delay();
    CTRL_PORT |= _BV(WR);
}


/*
void glcd::SetPortDirection(bool out)
{
    if (out)
    {
        DDRB = 0xFF;
        DDRB |= (_BV(4) | _BV(5));
    }
    else
    {
        DDRB = 0;
        //PORTC = 0xFF;
        PORTC = 0;
        DDRB &= ~(_BV(4)|_BV(5));
        PORTB &= ~(_BV(4)|_BV(5));
        //PORTB |= _BV(4) | _BV(5);
    }
}*/

/*
uint8_t glcd::ReadData()
{
    uint8_t data = 0;
    PORTD |= _BV(RS);
    PORTD &= ~_BV(RD);
    _delay_us(2);
    PORTD |= _BV(RD);
    _delay_us(2);
    data = (PINC & 0x3F) | ((PINB >> 4) << 6);
    return data;
}*/

void glcd::SetActiveColWindow(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2)
{
    if (c1 == c2)
        SetDirection(DIR_COLUMN);
    else if (r1 == r2)
        SetDirection(DIR_ROW);



    SendCmd(0x15);
    SendData(c1);
    SendData(c1 == c2? c2+1: c2);

    SendCmd(0x75);
    SendData(r1);
    SendData(r1 == r2? r2+1: r2);
}

void glcd::SetDirection(LCD_DIRECTION dir)
{
	  if (draw_dir != dir)
	  {
    SendCmd(0xbc);
    if (dir == DIR_COLUMN)
    {
        SendData(0x06);
        SendData(0x01);
    }
    else
    {
        SendData(0x02);
        SendData(0x01);
    }

    SendData(0x02);
    draw_dir = dir;
    }
}
// Make one instance for the user
glcd GLCD = glcd();

