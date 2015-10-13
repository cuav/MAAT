#include "SensorPage.h"
#include "Mark.h"

enum SENSOR_SETUP_ITEM
{
    SENSOR_SETUP_ITEM_DECL_DEG = 1,
    SENSOR_SETUP_ITEM_DECL_MIN,
    SENSOR_SETUP_ITEM_ACCEL,
    SENSOR_SETUP_ITEM_GYRO,
    SENSOR_SETUP_ITEM_MAG,
};

const char D4[] PROGMEM = "%-4d";
const char D2[] PROGMEM = "%-2d";

uint8_t SensorPage::progress_row;

SensorPage::SensorPage()
    :GUIPage(PSTR("Sensor Setup")),
     l_decl_deg(15, 55, PSTR("Degree")),
     l_decl_min(15, 65, PSTR("Minute")),
     l_acc(15, 90, PSTR("Accelerator")),
     l_gyro(15, 100, PSTR("Gyroscope")),
     l_mag(15, 110, PSTR("Compass")),
     cb_acc(90, 90, false),
     cb_gyro(90, 100, false),
     cb_mag(90, 110, false)
{
}

void SensorPage::on_paint(Graphics* g)
{
    GUIPage::on_paint(g);
    if (invalid)
    {
        g->draw_string_p(3, 20, PSTR("Attitude"));
        g->draw_string_p(3, 45, PSTR("Declination"));
        g->draw_string_p(3, 80, PSTR("Calibration"));

        g->draw_string_p(15, 30, PSTR("R"));
        g->draw_string_p(60, 30, PSTR("P"));
        g->draw_string_p(105, 30, PSTR("Y"));
        for (Label* l = &l_decl_deg; l <= &l_mag; l++)
        {
            l->on_paint(g);
            delay(2);
        }
        for (Label* l = &l_acc; l <= &l_mag; l++)
        {
            CheckBox::draw(g, 90, l->get_top(), false);
            delay(2);
        }

        selected_item = SENSOR_SETUP_ITEM_DECL_DEG;
//        gui.get_cursor()->set_type(CURSOR_TYPE_NAVIGATE);
        gui.get_cursor()->set(l_decl_deg.get_left() - 6, l_decl_deg.get_top(),
                              CURSOR_TYPE_NAVIGATE, false);

        invalid = false;
    }


    StringBuilder sb;
    // update attitude
    if (update_count == 0)
    {
        sb.printf_P(D4, ahrs.euler_angle[0]/10);
        g->draw_string(27, 30, (const char*)sb);
        sb.clear();
    }
    else if (update_count == 1)
    {
        sb.printf_P(D4, ahrs.euler_angle[1]/10);
        g->draw_string(72, 30, (const char*)sb);
        sb.clear();
    }
    else if (update_count == 2)
    {
        sb.printf_P(D4, ahrs.euler_angle[2]/10);
        g->draw_string(117, 30, (const char*)sb);
        sb.clear();
    }
    // update value
    else if (update_count == 3)
    {
        sb.printf_P(D4, decl_deg);
        g->draw_string(90, l_decl_deg.get_top(), (const char*)sb);
        sb.clear();
    }
    else if (update_count == 4)
    {
        sb.printf_P(D2, decl_min);
        g->draw_string(90, l_decl_min.get_top(), (const char*)sb);
        sb.clear();
    }

    if (++update_count > 10)
        update_count = 0;

}

void SensorPage::on_key_press(uint8_t key_code)
{
    CURSOR_TYPE ctype = gui.get_cursor()->get_type();

    if (key_code == KEY_S1)
    {
        if (selected_item >= SENSOR_SETUP_ITEM_ACCEL && selected_item <= SENSOR_SETUP_ITEM_MAG)
        {
            if (ctype == CURSOR_TYPE_NAVIGATE)
            {
                gui.get_cursor()->set_type(CURSOR_TYPE_SELECTED);

                Graphics* g = gui.get_graphics();
                Label* l = &l_acc + selected_item - SENSOR_SETUP_ITEM_ACCEL;
                CheckBox::draw(g, 90, l->get_top(), true);
                //gui.enable_key(false);
                progress_row = l->get_top();
                tracker.stop();
                pan_servo.set_angle(0);

                if (selected_item == SENSOR_SETUP_ITEM_ACCEL)
                    imu.init_accel(SensorPage::progress_delay);
                else if (selected_item == SENSOR_SETUP_ITEM_GYRO)
                    imu.init_gyro(SensorPage::progress_delay);
                else
                    mag.calibrate(SensorPage::progress_delay);

                ahrs.reset(true, true);
                //gui.enable_key(true);
                //            cb_acc.set_checked(false);
                CheckBox::draw(g, 90, l->get_top(), false);
                gui.get_cursor()->set_type(CURSOR_TYPE_NAVIGATE);

                tracker.start();
            }
            else if (ctype == CURSOR_TYPE_SELECTED && selected_item == SENSOR_SETUP_ITEM_MAG)
            {
                mag.quit_calibrate();
            }
        }
        else //if (selected_item > 0)
        {
            //selected_item = -selected_item;
            gui.get_cursor()->set_type(ctype == CURSOR_TYPE_NAVIGATE? CURSOR_TYPE_SELECTED: CURSOR_TYPE_NAVIGATE);
        }
    }
    else if (key_code == KEY_S2)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
        {
            gui.go_back();
        }
        else
        {

        }
    }
    else if (key_code == KEY_UP || key_code == KEY_DOWN)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
        {
            if (key_code == KEY_UP)
                selected_item--;
            else
                selected_item++;
            if (selected_item <= 0)
                selected_item = SENSOR_SETUP_ITEM_MAG;
            else if (selected_item > SENSOR_SETUP_ITEM_MAG)
                selected_item = SENSOR_SETUP_ITEM_DECL_DEG;

            Label* curr = &l_decl_deg + selected_item - 1;
            gui.get_cursor()->move_to(curr->get_left() - 6, curr->get_top());
        }
        else if (selected_item == SENSOR_SETUP_ITEM_DECL_DEG)
        {
            if (key_code == KEY_UP)
                decl_deg++;
            else
                decl_deg--;

            decl_deg = constrain(decl_deg, -180, 180);
            mag.set_declination(ToRad(decl_deg + decl_min / 60.0f));
            ahrs.reset(true, true);
        }
        else if (selected_item == SENSOR_SETUP_ITEM_DECL_MIN)
        {
            if (key_code == KEY_UP)
                decl_min++;
            else
                decl_min--;

            decl_min = constrain(decl_deg, 0, 59);
            mag.set_declination(ToRad(decl_deg + decl_min / 60.0f));
            ahrs.reset(true, true);
        }
    }
}


const prog_char* chars = "-\0\\\0|\0/";//{"-","\\","|","/"};
void SensorPage::progress_delay(uint32_t t)
{
    static uint8_t idx = 0;
    static uint32_t time = 0;

    uint32_t outtime = millis() + t;

    do
    {
        systime = millis();
        mavcomm.update();
        ctrlcomm.update();
        if (systime > time)
        {
            debug("update progress\r\n");
            time = systime + 200;
            Graphics* g = gui.get_graphics();
            g->draw_string(9, progress_row, chars + (idx << 1));
            if (++idx > 3)
                idx = 0;
        }
    }
    while (systime < outtime);
}

void SensorPage::on_exit()
{
    mag.save();
}


