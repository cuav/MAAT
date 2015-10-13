#ifndef SENSOR_SETUP_H
#define SENSOR_SETUP_H

#include <Graphics.h>
#include <Label.h>
#include "GUIPage.h"

class SensorPage: public GUIPage
{
public:
    SensorPage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
    virtual void on_exit();
private:
    static void progress_delay(uint32_t t);

    Label l_decl_deg, l_decl_min, l_acc, l_gyro, l_mag;
    CheckBox cb_acc, cb_gyro, cb_mag;
    int8_t selected_item;
    int16_t decl_deg, decl_min;
    uint32_t update_time;
    int8_t update_count;
    static uint8_t progress_row;
};

#endif
