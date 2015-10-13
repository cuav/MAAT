#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>


#define KEY_UP_PIN		0
#define KEY_DOWN_PIN	4
#define KEY_LEFT_PIN	1
#define KEY_RIGHT_PIN	2
#define KEY_S1_PIN		3
#define KEY_S2_PIN		5

enum KEY_CODE
{
    //KEY_UP = 1,	KEY_DOWN = 2, KEY_LEFT = 4, KEY_RIGHT = 8, KEY_ENTER = 0x10, KEY_S1 = 0x20, KEY_S2 = 0x40
    KEY_UP	= 1 << KEY_UP_PIN,
    KEY_DOWN = 1 << KEY_DOWN_PIN,
    KEY_LEFT = 1 << KEY_LEFT_PIN,
    KEY_RIGHT = 1 << KEY_RIGHT_PIN,
    KEY_S1 = 1 << KEY_S1_PIN,
    KEY_S2 = 1 << KEY_S2_PIN,
};


class keyboard
{
public:
    void Init();
    void Update();
    uint8_t GetKeyState()
    {
        return keys;
    }
    uint8_t GetPressKey()
    {
        return press;
    }

private:
    int8_t  state[8];
    uint8_t keys;
    uint8_t press;
};

extern keyboard Keyboard;

#endif
