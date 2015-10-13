#include "PowerOffPage.h"
#include "defines.h"

void PowerOffPage::on_paint(Graphics* g)
{
    if (invalid)
    {
        invalid = false;
        g->clear_screen();
        delay(5);
        debug("draw logo\r\n");
        g->draw_logo();
        g->draw_string_p(LOGO_MSG_X, LOGO_MSG_Y, PSTR("Safe to power off"));
    }
}

void PowerOffPage::on_load()
{
    gui.enable_key(false);
    invalid = true;
    logger.end();
    tracker.stop();
    pan_servo.stop();
    tlt_servo.stop();
}


