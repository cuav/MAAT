#ifndef UPDATE_PAGE_H
#define UPDATE_PAGE_H

#include "GUIPage.h"
//#include "SetupPage.h"
#include "FlyModePage.h"

class UpgradePage: public GUIPage
{
public:
    UpgradePage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
private:
    //FlyModePage flymode;
    //SetupPage setup;
    //int8_t selected_index;
};
#endif

