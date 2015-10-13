#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <avr/pgmspace.h>
#include <stdint.h>


const uint8_t COLOR_WHITE = 0;
const uint8_t COLOR_BLACK = 255;



class Graphics
{
public:
    virtual void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) = 0;
    virtual void draw_hline(uint8_t x, uint8_t y, uint8_t width) = 0;
    virtual void draw_vline(uint8_t x, uint8_t y, uint8_t height) = 0;
    virtual void draw_hparallel(uint8_t x, uint8_t y, uint8_t width, uint8_t interval, uint8_t count) = 0;
    virtual void draw_vparallel(uint8_t x, uint8_t y, uint8_t height, uint8_t interval, uint8_t count) = 0;
    virtual void clear_screen() = 0;
    virtual void draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) = 0;
    virtual void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) = 0;

    virtual void draw_string(uint8_t x, uint8_t y, const char* str) = 0;
    virtual void draw_string_p(uint8_t x, uint8_t y, const prog_char* str) = 0;
	
	virtual void draw_image_p(uint8_t x, uint8_t y, uint8_t id) = 0;
	virtual void draw_logo() = 0;
	
	virtual void set_contrast(int8_t c) = 0;
	virtual int8_t get_contrast() = 0;
	
	virtual void set_brightness(int16_t b) = 0;
	virtual int16_t get_brightness() = 0;
	
	virtual void set_black_light_time(int16_t time) = 0;
	virtual int16_t get_black_light_time() = 0;
	
	virtual void save_setting() = 0;
};

#endif
