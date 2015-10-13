#ifndef OSDPAGE_H
#define OSDPAGE_H

#include <Graphics.h>

#include "GUIPage.h"
#include "MAVLink.h"

class OSDPage: public GUIPage, public MAVLinkMessageHandler
{
public:
    OSDPage()
    {
        refresh_count = 0;
    }
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
    virtual void on_load();
    virtual void link_status_changed(MAVLINK_STATUS status)
    {
        invalid = true;
    }
private:
    void draw_horizon(Graphics* g);
    void draw_heading(Graphics* g);
    uint8_t refresh_count;
};

#endif

