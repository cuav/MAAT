#ifndef SERIAL_LCD_H
#define SERIAL_LCD_H

#include <ctrl_link.h>
#include "graphics.h"
#include <stdint.h>

const uint8_t BLACK	= 0;
const uint8_t WHITE	= 0xFF;

class SerialLCD: public Graphics, ControlLinkMessageHandler
{
public:
    SerialLCD(ControlLink* _link):
    	link(_link), contrast(255), brightness(255), black_light_time(0xffff)
    {got_setting = 0;}
    
    virtual void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    virtual void draw_hline(uint8_t x, uint8_t y, uint8_t width);
    virtual void draw_vline(uint8_t x, uint8_t y, uint8_t height);
    virtual void draw_hparallel(uint8_t x, uint8_t y, uint8_t width, uint8_t interval, uint8_t count);
    virtual void draw_vparallel(uint8_t x, uint8_t y, uint8_t height, uint8_t interval, uint8_t count);
    virtual void clear_screen();
    virtual void draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    virtual void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

    virtual void draw_string(uint8_t x, uint8_t y, const char* str);
    virtual void draw_string_p(uint8_t x, uint8_t y, const prog_char* str);
	
	virtual void draw_image_p(uint8_t x, uint8_t y, uint8_t id);
	virtual void draw_logo();
	
	virtual void message_received(ControlLinkMessage* msg);


	virtual void set_contrast(int8_t c);
	virtual int8_t get_contrast();
	
	virtual void set_brightness(int16_t b);
	virtual int16_t get_brightness();
	
	virtual void set_black_light_time(int16_t time);
	virtual int16_t get_black_light_time();	

	virtual void save_setting();
private:
    ControlLink* link;
    int8_t contrast;
    int16_t brightness;
    int16_t black_light_time;
    uint8_t got_setting;
};


#endif
