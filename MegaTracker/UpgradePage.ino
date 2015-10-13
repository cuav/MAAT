#include "UpgradePage.h"

UpgradePage::UpgradePage()
    :GUIPage(PSTR("Firmware Upgrade"))
{
    //selected_index = 0;
}

void UpgradePage::on_paint(Graphics* g)
{
    GUIPage::on_paint(g);
    if (invalid)
    {
        debug("UpdatePage::on_paint\r\n");
        GUIPage::on_paint(g);

        g->draw_image_p(9, 44, IMG_ID_AAT);
        g->draw_image_p(90, 40, IMG_ID_LCD_SMALL);

        g->draw_string_p(9, 80, PSTR("Tracker"));
        g->draw_string_p(90, 80, PSTR("CtrlBox"));

        gui.get_cursor()->set(3 + 81 * curr_item, 80, CURSOR_TYPE_NAVIGATE, false);
        invalid = false;
    }
}

void UpgradePage::on_key_press(uint8_t key_code)
{
    if (key_code == KEY_S2)
    {
        gui.go_back();
    }
    else if (key_code == KEY_S1)
    {
        int update_mode = curr_item + 1;

        Graphics* g = gui.get_graphics();

        g->clear_screen();
        g->draw_string_p(30, 80, PSTR("Enter upgrade mode"));
        g->draw_string_p(30, 90, PSTR("Reboot to exit"));
        gui.enable_key(false);

        ctrlcomm.send_message(CTRL_UPDATE, &update_mode, sizeof(uint8_t));
        //if (curr_item == 1)
        {
            //update control box
            delay(100);	//wait to flush serial
            ctrlcomm.end();
            mavcomm.end();
            //SERIAL_CTL_PORT.end();
        }
        tracker.stop();
        pan_servo.stop();
        tlt_servo.stop();
    }
    else if (key_code == KEY_LEFT || key_code == KEY_RIGHT)
    {
        if (++curr_item > 1)
            curr_item = 0;

        gui.get_cursor()->move_to(3 + 81 * curr_item, 80);
    }
}

