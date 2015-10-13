#include "serial_lcd.h"
#include <FastSerial.h>
#include <ctrl_cmd.h>
#include <Arduino.h>

#include <debug.h>


void SerialLCD::draw_line(uint8_t x1,
                    uint8_t y1, uint8_t x2, uint8_t y2)
{
    DrawLineMsgData data;
    data.x1 = x1;
    data.y1 = y1;
    data.x2 = x2;
    data.y2 = y2;
    link->send_message(LCD_DRAW_LINE, &data, sizeof(data));
}

void SerialLCD::draw_hline(uint8_t x, uint8_t y, uint8_t width)
{
    DrawHLineMsgData data;
    data.x = x;
    data.y = y;
    data.width = width;
    link->send_message(LCD_DRAW_HLINE, &data, sizeof(data));
}

void SerialLCD::draw_vline(uint8_t x, uint8_t y, uint8_t height)
{
    DrawVLineMsgData data;
    data.x = x;
    data.y = y;
    data.height = height;
    link->send_message(LCD_DRAW_VLINE, &data, sizeof(data));
}

void SerialLCD::draw_hparallel(uint8_t x, uint8_t y, uint8_t width, uint8_t interval, uint8_t count)
{
    DrawHParallelMsgData data;
    data.x = x;
    data.y = y;
    data.width = width;
    data.interval = interval;
    data.count = count;
    link->send_message(LCD_DRAW_HPARALLEL, &data, sizeof(data));
}

void SerialLCD::draw_vparallel(uint8_t x, uint8_t y, uint8_t height, uint8_t interval, uint8_t count)
{
    DrawVParallelMsgData data;
    data.x = x;
    data.y = y;
    data.height = height;
    data.interval = interval;
    data.count = count;
    link->send_message(LCD_DRAW_VPARALLEL, &data, sizeof(data));
}

void SerialLCD::clear_screen()
{
    link->send_message(LCD_CLEAR_SCREEN, 0, 0);
}

void SerialLCD::draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    DrawRectMsgData data;
    data.x = x;
    data.y = y;
    data.width = width;
    data.height = height;
    link->send_message(LCD_DRAW_RECT, &data, sizeof(data));
}

void SerialLCD::fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
    FillRectMsgData data;
    data.x = x;
    data.y = y;
    data.width = width;
    data.height = height;
    data.color = color;
    link->send_message(LCD_FILL_RECT, &data, sizeof(data));
}

void SerialLCD::draw_string(uint8_t x, uint8_t y, const char* str)
{
    DrawStringMsgData data;
    data.x = x;
    data.y = y;

    uint8_t len = 2;
    uint8_t* dst = data.str;
    while (*str)
    {
        *(dst++) = *(str++);
        len++;
    }
    *dst = 0;
    link->send_message(LCD_DRAW_STRING, &data, len+1);
}

void SerialLCD::draw_string_p(uint8_t x, uint8_t y, const prog_char* str)
{
    DrawStringMsgData data;
    data.x = x;
    data.y = y;

    uint8_t len = 2;
    uint8_t* dst = data.str;
    uint8_t ch;
    while ((ch = pgm_read_byte(str++)))
    {
        *(dst++) = ch;
        len++;
    }
    *dst = 0;
    link->send_message(LCD_DRAW_STRING, &data, len + 1);
}

void SerialLCD::draw_image_p(uint8_t x, uint8_t y, uint8_t id)
{
	uint8_t data[3];
	data[0] = x;
	data[1] = y;
	data[2] = id;
		
	link->send_message(LCD_DRAW_IMAGE, data, sizeof(data));
}

void SerialLCD::draw_logo()
{
	link->send_message(LCD_DRAW_LOGO, 0, 0);
}

void SerialLCD::set_contrast(int8_t c)
{
	c = constrain(c, 0, 0x1e);
	if (contrast != c)
	{
		contrast = c;
		link->send_message(LCD_SET_CONTRAST, &contrast, 1);
	}
}

int8_t SerialLCD::get_contrast()
{
	if (!(got_setting & 0x1))
	{
		debug("query contrast\r\n");
		link->send_message(LCD_GET_CONTRAST);
		return -1;
	}
	else
		return contrast;
}
	
void SerialLCD::set_brightness(int16_t b)
{
	b = constrain(b, 0, 250);
	if (b != brightness)
	{
		brightness = b;
		link->send_message(LCD_SET_BRIGHTNESS, &brightness, 1);
	}
}

int16_t SerialLCD::get_brightness()
{
	if (!(got_setting & 0x2))
	{
		debug("query brightness\r\n");
		link->send_message(LCD_GET_BRIGHTNESS);
		return -1;
	}
	else
		return brightness;
}
	
void SerialLCD::set_black_light_time(int16_t time)
{
	time = constrain(time, 0, 3000);
	if (time != black_light_time)
	{
		black_light_time = time;
		link->send_message(LCD_SET_BL_TIMEOUT, &black_light_time, 2);
	}
}

int16_t SerialLCD::get_black_light_time()
{
	if (!(got_setting & 0x4))
	{
		debug("query bl time\r\n");
		link->send_message(LCD_GET_BL_TIMEOUT);
		return -1;
	}
	else
		return black_light_time;
}

void SerialLCD::save_setting()
{
	link->send_message(LCD_SAVE_SETTING);
}

void SerialLCD::message_received(ControlLinkMessage* msg)
{
	switch (msg->cmd)
	{
	case LCD_SET_CONTRAST:
		debug("got contrast %d\r\n", msg->data[0]);
		contrast = msg->data[0];
		got_setting |= 0x1;
		break;
	case LCD_SET_BRIGHTNESS:
		debug("got birghtness %d\r\n", msg->data[0]);
		brightness = constrain(msg->data[0], 0, 100);
		got_setting |= 0x2;
		break;
	case LCD_SET_BL_TIMEOUT:
		black_light_time = constrain(*((int16_t*)msg->data), 0, 3000);
		debug("got bl time %d\r\n", black_light_time);
		got_setting |= 0x4;
		break;
	}
}