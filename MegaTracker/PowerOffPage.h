#ifndef POWER_OFF_PAGE_H
#define POWER_OFF_PAGE_H

#include "defines.h"
#include "GUIPage.h"

class PowerOffPage: public GUIPage
{
public:
    virtual void on_paint(Graphics* g);
    virtual void on_load();
};
#endif

