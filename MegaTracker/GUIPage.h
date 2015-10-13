#ifndef GUI_PAGE_H
#define GUI_PAGE_H

#include <Graphics.h>
#include "GUI.h"

class GUIPage
{
public:
    GUIPage(const prog_char* text = 0)
    {
        _text = text;
        invalid = true;
        curr_item = 0;
    }

    virtual void on_paint(Graphics* g)
    {
        //debug("GUIPage::on_paint");
        if (invalid)
        {
            g->clear_screen();
            if (_text)
            {
                g->draw_string_p(3, 2, _text);
                g->draw_hline(3, 12, 152);
            }
        }
    }

    virtual void on_key_press(uint8_t key_code)
    {
        if (key_code == KEY_S2)
            gui.go_back();
    }

    virtual void on_load()
    {
        invalid = true;
    }
    virtual void on_exit() {}
    virtual void invalidate()
    {
        invalid = true;
    }
    virtual const prog_char* text()
    {
        return _text;
    }
protected:
    const prog_char* _text;
    bool invalid;
    int8_t curr_item;
};
#endif

