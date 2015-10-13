#ifndef GUI_H
#define GUI_H

#include <Graphics.h>
#include <Cursor.h>
#include "GUIPage.h"
//#include <prog_char>

/*
enum KEY_CODE
{
    KEY_UP = 1,	KEY_DOWN = 2, KEY_LEFT = 4, KEY_RIGHT = 8, KEY_ENTER = 0x10, KEY_S1 = 0x20, KEY_S2 = 0x40
};
*/

#define KEY_UP_PIN	0
#define KEY_DOWN_PIN	4
#define KEY_LEFT_PIN	1
#define KEY_RIGHT_PIN	2
#define KEY_S1_PIN	3
#define KEY_S2_PIN	5

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

class GUIPage;

class GUI
{
public:
    GUI(Graphics* _g /*, GUIPage* _pages[], uint8_t _page_num*/)
        : g(_g) /*, pages(_pages), page_num(_page_num)*/
    {
        active_page = 0;
        enable = false;
        key_enable = true;
    }
//    void init() {active_page->on_load();}
    void update();
    void key_press(uint8_t key_code);
//    void next_page();
//    void prev_page();
    void navigate(GUIPage* page, bool clear_history = false);
    void go_back();

//    void switch_page(GUIPage* page);
    void start(GUIPage* page = 0);

    Graphics* get_graphics()
    {
        return g;
    }

    Cursor* get_cursor()
    {
        return &_cursor;
    }

    void enable_key(bool enable)
    {
        key_enable = enable;
    }
private:
    Graphics* g;
    Cursor _cursor;
//    GUIPage** pages;
//    uint8_t page_num;
    GUIPage* active_page;
    GUIPage* page_stack[8];
    uint8_t active_page_idx;
    bool enable;
    bool key_enable;
};

extern GUI gui;
#endif

