#ifndef SETUP_PAGE_H
#define SETUP_PAGE_H

#include "ServoPage.h"
//#include "SetupPage.h"
#include "DisplayPage.h"
#include "SensorPage.h"

class SetupPage: public GUIPage
{

public:
    SetupPage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
private:
    ServoPage servo;
    SensorPage sensor;
    DisplayPage screen;
};

#endif
