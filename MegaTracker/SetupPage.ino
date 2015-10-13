#include "SetupPage.h"


static uint8_t setup_label_left[] = {12, 61, 114};

SetupPage::SetupPage()
    :GUIPage(PSTR("Setup"))
{
}

void SetupPage::on_paint(Graphics* g)
{
    if (invalid)
    {
        debug("SetupPage::on_paint\r\n");
        GUIPage::on_paint(g);

        g->draw_image_p(9, 43, IMG_ID_SERVO);
        g->draw_image_p(61, 47, IMG_ID_SENSOR);
        g->draw_image_p(114, 40, IMG_ID_LCD_SMALL);

        g->draw_string_p(setup_label_left[0], 80, PSTR("Servo"));
        g->draw_string_p(setup_label_left[1], 80, PSTR("Sensor"));
        g->draw_string_p(setup_label_left[2], 80, PSTR("Screen"));

        gui.get_cursor()->set(setup_label_left[curr_item] - 6, 80, CURSOR_TYPE_NAVIGATE, false);
        invalid = false;
    }
}

void SetupPage::on_key_press(uint8_t key_code)
{
    if (key_code == KEY_S2)
        gui.go_back();
    else if (key_code == KEY_S1)
    {
        if (curr_item == 0)
            gui.navigate(&servo);
        else if (curr_item == 1)
            gui.navigate(&sensor);
        else if (curr_item == 2)
            gui.navigate(&screen);
    }
    else if (key_code == KEY_LEFT || key_code == KEY_RIGHT)
    {
        if (key_code == KEY_RIGHT)
        {
            if (++curr_item > 2)//sizeof(pages)/sizeof(GUIPage*))
                curr_item = 0;
        }
        else if (--curr_item < 0)
            curr_item = 2;//sizeof(pages)/sizeof(GUIPage*) - 1;

        gui.get_cursor()->move_to(setup_label_left[curr_item] - 6, 80);
    }
}

