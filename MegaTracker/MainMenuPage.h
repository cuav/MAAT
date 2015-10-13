#ifndef MAIN_MENU_PAGE_H
#define MAIN_MENU_PAGE_H

#include "GUIPage.h"
#include "ServoPage.h"
#include "SetupPage.h"
#include "DisplayPage.h"
#include "SensorPage.h"
#include "FlyModePage.h"
#include "UpgradePage.h"
#include "PowerOffPage.h"
#include "HomePositionPage.h"

class MainMenuPage: public GUIPage
{
public:
    MainMenuPage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
private:
    FlyModePage flymode;
    HomePositionPage sethome;
    SetupPage setup;
    UpgradePage upgrade;
    PowerOffPage poweroff;
    //GUIPage* pages[5];
    //int8_t selected_index;

    Label l_flymode, l_reset_home, l_setup, l_upgrade, l_poweroff;
};
#endif

