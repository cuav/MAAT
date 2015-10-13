#include "key.h"

void Key::init()
{
    PINE &= ~_BV(7);
    PORTE |= _BV(7);
}

bool Key::is_pressed()
{
    bool ret = pressed;
    pressed = false;
    return ret;
}

bool Key::is_down()
{
    return state == 3;
}

void Key::update()
{
    if (PINE & _BV(7))  //up
    {
        if (state > 0)
            state = -2;
        else if (state < 0)
            state++;
    }
    else
    {
        if (state < 0)
            state = 0;
        else if (state >= 0 && state < 2)
            state++;
        else if (state == 2 && !pressed)
        {
            pressed = true;
            state = 3;
        }
    }
}
