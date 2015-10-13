#ifndef LABEL_H
#define LABEL_H

#include "graphics.h"
#include "component.h"
#include <avr/pgmspace.h>

class Progress: public Component
{
public:
		Label(uint8_t left, uint8_t top, prog_char* text)
		 :Component(left, top), _text(text)
		 {
		 }
		prog_char* get_text() {return _text;}
		virtual void on_paint(Graphics* g){g->DrawString_P(_left, _top, _text);}
		
private:
	  prog_char* _text;
};

#endif
