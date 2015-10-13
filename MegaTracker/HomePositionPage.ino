#include "HomePositionPage.h"
    
static const char NO_FIX[] PROGMEM = "No Fix     ";

void HomePositionPage::on_paint(Graphics* g)
{
    GUIPage::on_paint(g);

    if (invalid)
    {
        //sat_fix = aircraft.sat_fix;
		
        g->draw_string_p(3, 20, PSTR("Home Position"));
        g->draw_string_p(15, 30, PSTR("Lat"));
        g->draw_string_p(15, 40, PSTR("Lon"));
        g->draw_string_p(15, 50, PSTR("Alt"));
        g->draw_string_p(15, 60, PSTR("Use ground GPS"));
        g->draw_string_p(3, 75, PSTR("Craft Position"));
        g->draw_string_p(15, 85, PSTR("Lat"));
        g->draw_string_p(15, 95, PSTR("Lon"));
        g->draw_string_p(15, 105, PSTR("Alt"));

        g->draw_string_p(15, 115, PSTR("Set as Home"));
        
        cb_use_gps.invalidate();
        gui.get_cursor()->set(9, curr_item? 115: 60, CURSOR_TYPE_NAVIGATE, false);
        
        invalid = false;
    }

    //refresh position
    StringBuilder sb;


    if (refresh_count == 0)
    	draw_pos_when_fix(g, 39, 30, aircraft.home_lat, aircraft.got_home);
    else if (refresh_count == 1)
    	draw_pos_when_fix(g, 39, 40, aircraft.home_lon, aircraft.got_home);
    else if (refresh_count == 2)
    	draw_pos_when_fix(g, 39, 50, aircraft.home_alt, aircraft.got_home);
    else if (refresh_count == 3)
    	draw_pos_when_fix(g, 39, 85, aircraft.lat, aircraft.sat_fix);
    else if (refresh_count == 3)
    	draw_pos_when_fix(g, 39, 95, aircraft.lon, aircraft.sat_fix);
    else if (refresh_count == 3)
    	draw_pos_when_fix(g, 39, 105, aircraft.alt, aircraft.sat_fix);
    else if (refresh_count == 6)
    {
    	cb_use_gps.set_checked(aircraft.use_ground_gps);
    	cb_use_gps.on_paint(g);
    }

    if (++refresh_count == 20)
        refresh_count = 0;
}
void HomePositionPage::draw_pos_when_fix(Graphics* g, uint8_t x, uint8_t y, int pos, bool fix)
{
	if (fix)
    {
    	StringBuilder sb;
        sb.printf_P(PSTR("%5i"), pos);
        g->draw_string(x, y, (const char*)sb);
    }
    else
        g->draw_string_p(x, y, NO_FIX);
}

void HomePositionPage::draw_pos_when_fix(Graphics* g, uint8_t x, uint8_t y, float pos, bool fix)
{
	if (fix)
    {
    	StringBuilder sb;
        sb.printf_P(PSTR("%11.6f"), pos);
        g->draw_string(x, y, (const char*)sb);
    }
    else
        g->draw_string_p(x, y, NO_FIX);
}

void HomePositionPage::on_key_press(uint8_t key_code)
{
    if (key_code == KEY_S1)
    {
        if (curr_item == 0)
        {
        	aircraft.use_ground_gps = ! aircraft.use_ground_gps;
        	aircraft.save();
        	cb_use_gps.set_checked(aircraft.use_ground_gps);
        }
       	else if (curr_item == 1&& aircraft.sat_fix)
            aircraft.set_home();
    }
    else if (key_code == KEY_S2)
    {
        gui.go_back();
    }
    else if (key_code == KEY_UP || key_code == KEY_DOWN)
    {
    	curr_item = curr_item? 0: 1;
    	gui.get_cursor()->move_to(9, curr_item? 115:60);
    }
    /*
    else if ((key_code == KEY_LEFT || key_code == KEY_RIGHT) && sat_fix)
    {
    	if (curr_item == 0)
    	{
    		curr_item = 1;
    		gui.get_cursor()->move_to(45, 45);
    	}
    	else
    	{
    		curr_item = 0;
    		gui.get_cursor()->move_to(24, 45);
    	}
    }*/
}

    

