#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
		Component(uint8_t left, uint8_t top)
			:_left(left), _top(top), invalid(false)
	  {
	  }
	  
	  uint8_t get_left(){return _left;}
	  uint8_t get_top(){return _top;}
		virtual void on_paint(Graphics* g) = 0;
		virtual void invalidate(){invalid = true;}
protected:
		uint8_t _left, _top;
		bool invalid;
};

#endif