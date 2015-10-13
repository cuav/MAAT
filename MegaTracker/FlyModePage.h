#ifndef FLY_MODE_PAGE_H
#define FLY_MODE_PAGE_H

#include "GUIPage.h"

class FlyModePage: public GUIPage
{
public:
    FlyModePage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
private:
    //void set_cursor(CURSOR_TYPE type, bool remove_last);
    void get_cursor_pos(uint8_t idx, uint8_t* x, uint8_t* y);
    //static const prog_char** modes[2];

    //int8_t selected_index;
    int8_t cur_mode_index;
    uint8_t item_num;
    uint8_t mav_type;
    uint16_t fly_mode;
};
#endif

