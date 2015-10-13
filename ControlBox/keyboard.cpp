#include "keyboard.h"
#include <avr/io.h>
#include "uart.h"

keyboard Keyboard;
//#define _BV(x) (1<<x)

//const uint8_t key_msk[] = {_BV(2), _BV(3), _BV(0), _BV(5), _BV(_BV(7), _BV(6)};

void keyboard::Init()
{
    //ÊäÈë
    //DDRB &= 0xF0;
    //DDRB |= 0xFF;
    DDRB &= 0xC0;
    //DDRD &= 0x5F;
    //ÉÏÀ­
    //PORTB |= 0xF0;
    //PORTB |= 0xFF;
    PORTB |= 0x3F;
    //PORTD |= 0xA0;

    for (uint8_t i = 0; i < sizeof(state); i++)
        state[i] = 0;
}

void keyboard::Update()
{
    uint8_t new_key = 0;
		new_key = (PINB & 0x3F);
    //uart_putc(PINB&_BV(0));
    //uart_putc(new_key);
    keys = 0;

    press = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        state[i];
        uint8_t k = new_key & _BV(i);
        if (state[i] == 0)
        {
            if (!k)
                state[i]++;
        }
        else if (state[i] == 1)
        {
            if (k)
                state[i] = 0;
            else
                state[i]++;
        }
        else if (state[i] == 2)
        {
            if (k)
                state[i]++;
            keys |= _BV(i);
        }
        else if (state[i] == 3)
        {
            if (k)
            {
                //press
                state[i] = 0;
                press |= _BV(i);
            }
            else
                state[i]--;
        }
    }
}
