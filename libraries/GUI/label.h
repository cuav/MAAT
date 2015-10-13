#ifndef LABEL_H
#define LABEL_H

#include "graphics.h"
#include "component.h"
#include <avr/pgmspace.h>

class Label: public Component
{
public:
		Label(uint8_t left, uint8_t top, prog_char* text)
		 :Component(left, top), _text(text)
		 {
		 }
		prog_char* get_text() {return _text;}
		virtual void on_paint(Graphics* g){g->draw_string_p(_left, _top, _text);}
		
private:
	  prog_char* _text;
};

#endif
