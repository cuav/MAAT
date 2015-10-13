#ifndef RESET_SETTING_H
#define RESET_SETTING_H

#include "GUIPage.h"

class ResetSettingPage: public GUIPage
{
public:
    ResetSettingPage(): GUIPage(PSTR("Reset Settings")) {}
    virtual void on_paint(Graphics* g)
    {
        GUIPage::on_paint(g);
        if (invalid)
        {
            g->draw_string_p(15, 50, PSTR("Reset All Settings"))
            gui->get_cursor()->set(9, 50, CURSOR_TYPE_NAVIGAGTE, false);
            invalid = false;
        }
    }

    virtual void on_key_press(uint8_t key_code)
    {
        if (key_code == KEY_S2)
        {
            gui.go_back();
        }
        else if (key_code == KEY_S1)
        {
            reset_settings();
        }
    }
}

#endif