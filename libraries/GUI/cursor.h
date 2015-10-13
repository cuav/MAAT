#ifndef CURSOR_H
#define CURSOR_H

#include "component.h"
#include <FastSerial.h>

enum CURSOR_TYPE
{
	CURSOR_TYPE_BLANK, CURSOR_TYPE_NAVIGATE, CURSOR_TYPE_SELECTED,
};

const char* mark = " \x82\x81";

class Cursor:public Component
{
public:
	Cursor(uint8_t left = 0, uint8_t top = 0)
		:Component(left, top)
	{
		_old_left = _old_top = -1;
		str[1] = 0;
	}
	
	virtual void on_paint(Graphics* g)
	{
		if (invalid)
		{
			Serial2.printf("cursor refresh %d %d %c\r\n", _left, _top, str[0]);
			if (_old_left != _left || _old_top != _top)
			{
				if (_old_left >= 0 && _old_top >= 0)
					g->draw_string_p(_old_left, _old_top, PSTR(" "));
				_old_left = _left;
				_old_top = _top;
			}
			
			g->draw_string(_left, _top, str);
			invalid = false;
		}
	}
	
	void move_to(int8_t left, int8_t top, bool erase = true)
	{
		if (erase)
		{
			_old_left = _left;
			_old_top = _top;
		}
		else
		{
			_old_left = -1;
			_old_top = -1;
		}
		_left = left;
		_top = top;
		invalid = true;
	}
	
	void set_type(CURSOR_TYPE type)
	{
		str[0] = mark[type];
		_type = type;
		invalid = true;
	}
	
	CURSOR_TYPE get_type(){return _type;}
	
	void set(int8_t left, int8_t top, CURSOR_TYPE type, bool erase_last = true)
	{
		if (erase_last)
		{
			_old_left = _left;
			_old_top = _top;
		}
		else
		{
			_old_left = -1;
			_old_top = -1;
		}
		_left = left;
		_top = top;
		_type = type;
		str[0] = mark[type];
		invalid = true;
	}
	
private:
	int8_t _old_left, _old_top;
	CURSOR_TYPE _type;
	//static 
	
	char str[2];
};

#endif