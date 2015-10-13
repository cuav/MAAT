#ifndef DISPLAY_PAGE
#define DISPLAY_PAGE

#include "GUIPage.h"
class DisplayPage: public GUIPage
{
public:
    DisplayPage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
    virtual void on_exit();
private:
    void adjust_value(uint8_t key_code);
    uint8_t invalid_item;
    int8_t bl_type;
};

#endif

