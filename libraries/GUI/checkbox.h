#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "graphics.h"
#include "component.h"
#include <avr/pgmspace.h>

class CheckBox: public Component
{
public:
		CheckBox(uint8_t left, uint8_t top, bool checked = false)
		 :Component(left, top), _checked(checked)
		{
				
		}
		 
		bool is_checked(){return _checked;}
		bool set_checked(bool checked) 
		{
			if (checked != _checked)
			{
				_checked = checked;
				invalid = true;
			}
		}
		
		void on_paint(Graphics* g)
		{
			//g->DrawString_P(x, y, _text);
				if (invalid)
				{
						draw(g, _left, _top, _checked);
						invalid = false;
				}
		}
		
		static void draw(Graphics* g, uint8_t left, uint8_t top, bool checked)
		{
			if (checked)
				g->fill_rect(left, top, 6, 6, COLOR_WHITE);
			else
			{
				g->fill_rect(left, top, 6, 6, COLOR_BLACK);
				g->draw_rect(left, top, 6, 6);
			}
		}
private:
	  bool _checked;
	  //prog_char* _text;
};

#endif
