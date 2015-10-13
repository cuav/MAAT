#include "OSDPage.h"
#include <FastSerial.h>
#include <StringBuilder.h>


const char DIR_N[] PROGMEM = "N";
const char DIR_E[] PROGMEM = "E";
const char DIR_S[] PROGMEM = "S";
const char DIR_W[] PROGMEM = "W";
const char DIR_NE[] PROGMEM = "NE";
const char DIR_SE[] PROGMEM = "SE";
const char DIR_SW[] PROGMEM = "SW";
const char DIR_NW[] PROGMEM = "NW";

const char* DIR[] = {DIR_NW, DIR_N, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SW, DIR_W};

const char FM_STAB[] PROGMEM = "Stab";
const char FM_ACRO[] PROGMEM = "Acro";
const char FM_ALTH[] PROGMEM = "Alth";
const char FM_AUTO[] PROGMEM = "Auto";
const char FM_GUID[] PROGMEM = "Guid";
const char FM_LOIT[] PROGMEM = "Loit";
const char FM_RETL[] PROGMEM = "Retl";
const char FM_CIRC[] PROGMEM = "Circ";
const char FM_POSI[] PROGMEM = "Posi";
const char FM_OFLO[] PROGMEM = "Oflo";
const char FM_MANU[] PROGMEM = "Manu";
const char FM_FBWA[] PROGMEM = "Fbwa";
const char FM_FBWB[] PROGMEM = "Fbwb";
const char FM_LAND[] PROGMEM = "Land";
//const char FM_UNKO[] PROGMEM = "Unkw

const prog_char* FM_ACM[] = {FM_STAB, FM_STAB, FM_ALTH, FM_AUTO, FM_GUID, FM_LOIT, FM_RETL, FM_CIRC, FM_POSI, FM_LAND, FM_OFLO};
const prog_char* FM_APM[] = {FM_MANU, FM_CIRC, FM_STAB, FM_STAB, FM_STAB, FM_FBWA, FM_FBWB, FM_STAB, FM_STAB, FM_STAB, FM_AUTO, FM_RETL, FM_LOIT};


void OSDPage::on_paint(Graphics* g)
{
    //uint8_t now = millis();

    if (invalid)
    {
        debug("OSD refresh\r\n");
        //paint static strings

        //paint dynamic components
        g->clear_screen();
        MAVLINK_STATUS mav_status = mavcomm.get_status();

        if (mav_status >= MAVLINK_STATUS_GOT_DATA)
        {
            g->draw_string_p(3, 2, PSTR("Spd"));
            g->draw_string_p(3, 10, PSTR("Thr"));
            g->draw_string_p(54, 2, PSTR("Dis"));
            g->draw_string_p(54, 10, PSTR("Alt"));
            g->draw_string_p(111, 2, PSTR("Vlt"));
            g->draw_string_p(111, 10, PSTR("Amp"));


            //g->DrawHParallel(0, 0, 6, 6, 6);
            //g->DrawHParallel(0, 0, 3, 6, 6);
            g->draw_hparallel(28, 40, 2, 5, 9);
            g->draw_hparallel(27, 40, 3, 10, 5);
            g->draw_hparallel(129, 40, 2, 5, 9);
            g->draw_hparallel(129, 40, 3, 10, 5);
            g->draw_string_p(81, 111, PSTR("\xf0"));
            g->draw_image_p(81, 111, IMG_ID_SAT);
        }
        else if (mav_status <= MAVLINK_STATUS_WAIT_HEARTBEAT)
        {
            g->draw_logo();
            g->draw_string_p(42, 65, PSTR("Wait Hearbeat"));
        }
        else
        {
            g->draw_logo();
            g->draw_string_p(42, 65, PSTR("Wait Data..."));
        }

        refresh_count = 0;
    }

    if (mavcomm.get_status() >= MAVLINK_STATUS_GOT_DATA)
    {
        StringBuilder sb;
        //refresh_time = systime + 333;  //2Hz
//        Serial.printf("update osd %d\n", refresh_time);

        if (refresh_count == 0)
        {
            sb.printf_P(PSTR("%-3i"), (int) aircraft.groundspeed);
            g->draw_string(24, 2, (const char*)sb);
            sb.clear();

            sb.printf_P(PSTR("%-3i%%"), (int) aircraft.throttle);
            g->draw_string(24, 10, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 2)
        {
            sb.printf_P(PSTR("%-6i"), aircraft.home_dist);
            g->draw_string(75, 2, (const char*)sb);
            sb.clear();

            sb.printf_P(PSTR("%-5i"), (int) aircraft.alt);
            g->draw_string(75, 10, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 4)
        {
            sb.printf_P(PSTR("%-4.1f"), aircraft.volt);
            g->draw_string(132, 2, (const char*)sb);
            sb.clear();

            sb.printf_P(PSTR("%-4.1f"), aircraft.curr);
            g->draw_string(132, 10, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 6)
        {
            //gps position
            sb.printf_P(PSTR("%11.6f"), aircraft.lat);
            g->draw_string(3, 111, (const char*)sb);
            sb.clear();

            sb.printf_P(PSTR("%11.6f"), aircraft.lon);
            g->draw_string(3, 119, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 8)
        {
            //flight mode
            if (aircraft.mav_type == 1 && aircraft.fly_mode <= 12 || aircraft.fly_mode <= 10)
                g->draw_string_p(132, 111, aircraft.mav_type == 1? FM_APM[aircraft.fly_mode]: FM_ACM[aircraft.fly_mode]);

            //waypoint
            sb.printf_P(PSTR("%i>%i"), aircraft.wp_dist, aircraft.wp_no);
            g->draw_string(156 - sb.length() * 6, 119, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 10)
        {
            sb.printf_P(PSTR("%i\x80    "), (int)aircraft.roll);
            g->draw_string(72, 30, (const char*)sb);
            sb.clear();

            sb.printf_P(PSTR("%i\x80    "), (int)aircraft.pitch);
            g->draw_string(138, 58, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 12)
        {
            uint8_t fly_time = aircraft.fly_time/1000;
            sb.printf_P(PSTR("%02i:%02i"), fly_time / 60, fly_time % 60);
            g->draw_string(3, 20, (const char*)sb);
            sb.clear();

            //sat
            sb.printf_P(PSTR("%i"), aircraft.sat_count);
            g->draw_string(96, 111, (const char*)sb);
            sb.clear();
        }
        else if (refresh_count == 14)
        {
            const prog_char* fix = 0;

            if (aircraft.sat_fix == 2)
                fix = PSTR("2D");
            else if (aircraft.sat_fix == 3)
                fix = PSTR("3D");
            else
                fix = PSTR("NF");
            g->draw_string_p(96, 119, fix);
        }
        else if (refresh_count == 16)
            draw_heading(g);
        else if (refresh_count == 18)
            draw_horizon(g);
        else if (refresh_count == 20)
        {
            if (aircraft.sys_status < 3)
            {
                g->fill_rect(54, 57, 53, 9, COLOR_BLACK);
                g->draw_string_p(57, 58, PSTR("APM Init"));
            }
            else if (aircraft.sys_status != 4)
            {
                g->fill_rect(54, 57, 53, 9, COLOR_BLACK);
                g->draw_string_p(57, 58, PSTR("Disarmed"));
            }
        }

        if (++refresh_count > 20)
            refresh_count = 0;
        /*
        if (logger.is_logging())
        {
            g->draw_image_p(3, 60, IMG_ID_TF);
        }
        else
        {
            g->FillRect(3, 60, 12, 14, COLOR_BLACK);
        }*/
    }
    invalid = false;
}

void OSDPage::draw_heading(Graphics* g)
{
    int course = (int)aircraft.heading;
    //todo: clear region
    g->fill_rect(21, 85, 120, 16, COLOR_BLACK);
    g->draw_vline(79, 81, 2);
    short num = (360-course)%45;
    num = (num >= 35 || num <= 10)? 3:2;
    g->draw_vparallel(24 + (370 - course) % 45, 85, 1, 45, num);
    g->draw_vparallel(24 + (360 - course) % 5, 87, 2, 5, 23);

    for (uint8_t i = 0; i < num; i++)
        g->draw_string_p(24 + (370 - course ) % 45 + i * 45 -3, 93, DIR[((course + 35) / 45 + i) % 8]);
}

void OSDPage::draw_horizon(Graphics* g)
{
    //todo: clear region
    float roll = aircraft.roll;
    float pitch = aircraft.pitch;
    int16_t x1 = 0, y1 = 0, x2 = 92, y2 = 0;

    if (roll == 90)
        roll = 89.99f;
    else if (roll == -90)
        roll = -89.99f;

    roll *= 0.0174533;
    float tan_roll = tan(roll);

    y1 = (int16_t)(20-46*tan_roll - pitch/2);
    if (y1 < 0)
    {
        y1 = 0;
        x1 = (int16_t)((pitch/2-20)/tan_roll + 46);
    }
    else if (y1 > 40)
    {
        y1 = 40;
        x1 = (int16_t)((pitch/2+20)/tan_roll + 46);
    }

    y2 = (int16_t)(20-pitch/2+46*tan_roll);
    if (y2 < 0)
    {
        y2 = 0;
        x2 = (int16_t)((pitch/2-20)/tan_roll + 46);
    }
    else if (y2 > 40)
    {
        y2 = 40;
        x2 = (int16_t)((pitch/2+20)/tan_roll + 46);
    }
    g->fill_rect(33, 40, 95, 41, COLOR_BLACK);
    g->draw_line((uint8_t)(x1 + 33), (uint8_t)(80 - y1), (uint8_t)(x2 + 33), (uint8_t)(80 - y2));
}

void OSDPage::on_key_press(uint8_t key_code)
{
    if (key_code == KEY_S1)
        gui.navigate(&menu);
    /*    else if (key_code == KEY_S2)
        {
            if (aircraft.got_home)
            {
              aircraft.home_lat = aircraft.lat;
              aircraft.home_lon = aircraft.lon;
              aircraft.home_alt = aircraft.alt;
              aircraft.start_time = 0;
              aircraft.fly_time = 0;
            }
        }*/
}

void OSDPage::on_load()
{
    GUIPage::on_load();
    tracker.start();
}

