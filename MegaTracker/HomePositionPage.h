#ifndef RESET_HOME_PAGE_H
#define RESET_HOME_PAGE_H

#include "GUIPage.h"
class HomePositionPage: public GUIPage
{
public:
    HomePositionPage()
        :GUIPage(PSTR("Home Position")), cb_use_gps(105, 60, false)
    {
    }
	
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
private:
	void draw_pos_when_fix(Graphics* g, uint8_t x, uint8_t y, float pos, bool fix);
	void draw_pos_when_fix(Graphics* g, uint8_t x, uint8_t y, int pos, bool fix);
    //bool sat_fix;
    CheckBox cb_use_gps;
    uint8_t refresh_count;
};
#endif