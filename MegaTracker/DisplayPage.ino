#include "DisplayPage.h"
#include "defines.h"

enum
{
    DISPLAY_SETUP_CONTRAST, DISPLAY_SETUP_BRIGHTNESS, DISPLAY_SETUP_BLACKLIGHT
};

const int16_t bl_time[] = {0, 30, 60, 180, 300};
const char* bl_str[] = {"ON  ", "30s ", "60s ", "180s", "300s"};

DisplayPage::DisplayPage()
    :GUIPage(PSTR("Screen Setup"))
{
    //selected_item = 1;
    //curr_item = 1;
    invalid_item = 0xff;
}


void DisplayPage::on_paint(Graphics* g)
{
    GUIPage::on_paint(g);
    if (invalid)
    {
        invalid = false;

        g->draw_string_p(9, 20, PSTR("Contrast  "));
        g->draw_string_p(9, 35, PSTR("Brightness"));
        g->draw_string_p(9, 50, PSTR("Blacklight"));

        gui.get_cursor()->set(3, 20 + 15 * curr_item, CURSOR_TYPE_NAVIGATE, false);
    }

    StringBuilder sb;

    if ((invalid_item & _BV(DISPLAY_SETUP_CONTRAST)) && g->get_contrast() >= 0)
    {
        sb.printf_P(PSTR("%2i"), g->get_contrast()/3);
        g->draw_string(75, 20, (const char*)sb);
        sb.clear();
        cbi(invalid_item, DISPLAY_SETUP_CONTRAST);
    }

    if ((invalid_item & _BV(DISPLAY_SETUP_BRIGHTNESS)) && g->get_brightness() >= 0)
    {
        sb.printf_P(PSTR("%2i"), g->get_brightness()/25);
        g->draw_string(75, 35, (const char*)sb);
        sb.clear();
        cbi(invalid_item, DISPLAY_SETUP_BRIGHTNESS);
    }

    if ((invalid_item & _BV(DISPLAY_SETUP_BLACKLIGHT)) && g->get_black_light_time() >= 0)
    {
        //sb.printf_P(PSTR("%i"), g->get_brightness())
        int16_t bl = g->get_black_light_time();
        for (int i = 0; i < sizeof(bl_time)/sizeof(int16_t); i++)
        {
            if (bl <= bl_time[i])
            {
                bl_type = i;
                g->draw_string(75, 50, bl_str[i]);
                break;
            }
        }

        cbi(invalid_item, DISPLAY_SETUP_BLACKLIGHT);
    }
}

void DisplayPage::on_key_press(uint8_t key_code)
{
    CURSOR_TYPE ctype = gui.get_cursor()->get_type();
    if (key_code == KEY_S1)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
            gui.get_cursor()->set_type(CURSOR_TYPE_SELECTED);
        else if (ctype == CURSOR_TYPE_SELECTED)
            gui.get_cursor()->set_type(CURSOR_TYPE_NAVIGATE);
    }
    else if (key_code == KEY_UP || key_code == KEY_DOWN)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
        {
            curr_item += (key_code == KEY_UP? -1: 1);
            if (curr_item < DISPLAY_SETUP_CONTRAST)
                curr_item = DISPLAY_SETUP_BLACKLIGHT;
            else if (curr_item > DISPLAY_SETUP_BLACKLIGHT)
                curr_item = DISPLAY_SETUP_CONTRAST;
            gui.get_cursor()->move_to(3, 20 + 15 * curr_item);
        }
        else if (curr_item != DISPLAY_SETUP_BLACKLIGHT)
            adjust_value(key_code);
        else
        {
            bl_type += (key_code == KEY_UP? 1: -1);
            if (bl_type < 0)
                bl_type = sizeof(bl_time)/sizeof(int16_t) - 1;
            else if (bl_type >= sizeof(bl_time) / sizeof(int16_t))
                bl_type = 0;
            gui.get_graphics()->set_black_light_time(bl_time[bl_type]);
            sbi(invalid_item, DISPLAY_SETUP_BLACKLIGHT);
        }
    }
    //else if ((key_code == KEY_LEFT || key_code == KEY_RIGHT) && ctype == CURSOR_TYPE_SELECTED)
    //{
    //adjust_value(key_code);
    //}
    else if (key_code == KEY_S2)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
            gui.go_back();
    }
}

void DisplayPage::adjust_value(uint8_t key_code)
{
    int8_t adj = 0;
    if (key_code == KEY_UP)
        adj = 1;
    else if (key_code == KEY_DOWN)
        adj = -1;
    /*else if (key_code == KEY_LEFT)
        adj = -10;
    else if (key_code == KEY_RIGHT)
        adj = 10;*/

    if (curr_item == DISPLAY_SETUP_CONTRAST)
    {
        gui.get_graphics()->set_contrast(gui.get_graphics()->get_contrast() + adj * 3);
        sbi(invalid_item, DISPLAY_SETUP_CONTRAST);
    }
    else if (curr_item == DISPLAY_SETUP_BRIGHTNESS)
    {
        gui.get_graphics()->set_brightness(gui.get_graphics()->get_brightness() + adj * 25);
        sbi(invalid_item, DISPLAY_SETUP_BRIGHTNESS);
    }
}

void DisplayPage::on_exit()
{
    gui.get_graphics()->save_setting();
}