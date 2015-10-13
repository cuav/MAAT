#include "MainMenuPage.h"

//static uint8_t main_menu_label_pos[][2] = {{9, 60}, {63, 60}, {114, 60}, {9, 115}, {63, 115}};

enum MAIN_MENU_ITEM
{
	MAIN_MENU_ITEM_FLYMODE,
	MAIN_MENU_ITEM_SETHOME,
	MAIN_MENU_ITEM_SETUP,
	MAIN_MENU_ITEM_UPGRADE,
	MAIN_MENU_ITEM_TURNOFF,
};

MainMenuPage::MainMenuPage()
    :GUIPage(PSTR("Menu")),
     l_flymode(9, 60, PSTR("Modes")),
     l_reset_home(69, 60, PSTR("Home")),
     l_setup(120, 60, PSTR("Setup")),
     l_upgrade(9, 115, PSTR("Upgrade")),
     l_poweroff(63, 115, PSTR("TurnOff"))
{
}

void MainMenuPage::on_paint(Graphics* g)
{
    if (invalid)
    {
        debug("MainMenu::on_paint\r\n");
        GUIPage::on_paint(g);

        g->draw_image_p(6, 19, IMG_ID_MODE);
        delay(1);
        g->draw_image_p(63, 21, IMG_ID_HOME);
        delay(1);
        g->draw_image_p(117, 22, IMG_ID_SETUP);
        delay(1);
        g->draw_image_p(6, 74, IMG_ID_UPGRADE);
        delay(1);
        g->draw_image_p(63, 74, IMG_ID_POWER);

        /*g->draw_string_p(main_menu_label_pos[0][0], main_menu_label_pos[0][1], PSTR("Modes"));
        g->draw_string_p(main_menu_label_pos[0][1], main_menu_
        g->draw_string_p(main_menu_label_pos[1][0], main_menu_label_pos[1][1], PSTR("Setup"));
        g->draw_string_p(main_menu_label_pos[2][0], main_menu_label_pos[2][1], PSTR("Upgrade"));
        g->draw_string_p(main_menu_label_pos[3][0], main_menu_label_pos[3][1], PSTR("TurnOff"));
        */
        for (Label* l = &l_flymode; l <= &l_poweroff; l++)
        {
            l->on_paint(g);
            delay(2);
        }
        //g->draw_image_p();
        //for (uint8_t i = 0; i < sizeof(pages)/sizeof(GUIPage*); i++)
        //    g->draw_string_p(9, 20 + 10 * i, pages[i]->text());
//        g->draw_string_p(3, 20 + selected_index* 10, PSTR(">"));
        gui.get_cursor()->set(
            (&l_flymode + curr_item)->get_left() - 6,
            (&l_flymode + curr_item)->get_top(),
            CURSOR_TYPE_NAVIGATE,
            false);

        invalid = false;
    }
}

void MainMenuPage::on_key_press(uint8_t key_code)
{
    if (key_code == KEY_S2)
        gui.go_back();
    else if (key_code == KEY_S1)
    {
        if (curr_item == MAIN_MENU_ITEM_FLYMODE)
            gui.navigate(&flymode);
        else if (curr_item == MAIN_MENU_ITEM_SETHOME)
        	gui.navigate(&sethome);
        else if (curr_item == MAIN_MENU_ITEM_SETUP)
            gui.navigate(&setup);
        else if (curr_item == MAIN_MENU_ITEM_UPGRADE)
            gui.navigate(&upgrade);
        else if (curr_item == MAIN_MENU_ITEM_TURNOFF)
            gui.navigate(&poweroff);
    }
    else if (key_code == KEY_LEFT || key_code == KEY_RIGHT)
    {
        if (key_code == KEY_RIGHT)
        {
            if (++curr_item > 4)//sizeof(pages)/sizeof(GUIPage*))
                curr_item = 0;
        }
        else if (--curr_item < 0)
            curr_item = 4;//sizeof(pages)/sizeof(GUIPage*) - 1;
        gui.get_cursor()->move_to(
            (&l_flymode + curr_item)->get_left() - 6,
            (&l_flymode + curr_item)->get_top());
    }
}

