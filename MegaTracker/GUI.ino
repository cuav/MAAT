#include "GUI.h"

void GUI::update()
{
//    if (active_page)
    if (enable)
    {
        active_page->on_paint(g);
        _cursor.on_paint(g);
    }
}

void GUI::key_press(uint8_t key_code)
{
//    if (active_page)
    debug("key pressed\r\n");
    if (key_enable && active_page)
        active_page->on_key_press(key_code);
}

/*
void GUI::next_page()
{
    if (++active_page_idx >= page_num)
        active_page_idx = 0;

    active_page = pages[active_page_idx];
//    if (active_page)
        active_page->on_load();
}
*/
/*
void GUI::prev_page()
{
    if (--active_page_idx <= 0)
        active_page_idx = page_num-1;

    active_page = pages[active_page_idx];
//    if (active_page)
        active_page->on_load();
}*/

void GUI::navigate(GUIPage* page, bool clear_history)
{
    if (active_page)
        active_page->on_exit();
    if (clear_history)
        active_page_idx = 0;
    else if (++active_page_idx >= 8)
        active_page_idx = 7;

    active_page = page_stack[active_page_idx] = page;
    active_page->on_load();
}

void GUI::go_back()
{
    if (active_page_idx > 0)
    {
        active_page->on_exit();
        active_page = page_stack[--active_page_idx];
        active_page->on_load();
    }
}

void GUI::start(GUIPage* page)
{
    if (!enable)
    {
        enable = true;
        navigate(page, true);
    }
}

