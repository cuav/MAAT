#include "ServoPage.h"

enum SERVO_SETUP_ITEM
{
    SERVO_SETUP_ITEM_TLT_MIN, SERVO_SETUP_ITEM_TLT_MAX,
    SERVO_SETUP_ITEM_TLT_CENTER, SERVO_SETUP_ITEM_TLT_90,

    /*SERVO_SETUP_ITEM_PAN_AUTO,
    SERVO_SETUP_ITEM_PAN_MIN, SERVO_SETUP_ITEM_PAN_MAX,
    SERVO_SETUP_ITEM_PAN_CENTER, SERVO_SETUP_ITEM_PAN_ACC,*/

    SERVO_SETUP_ITEM_PAN_SP, SERVO_SETUP_ITEM_PAN_SI,
    SERVO_SETUP_ITEM_PAN_SD,

    SERVO_SETUP_ITEM_PAN_AP, SERVO_SETUP_ITEM_PAN_AI,
    SERVO_SETUP_ITEM_PAN_AD,
    SERVO_SETUP_ITEM_NULL,
};

const char I4[] PROGMEM = "%-+4i";
const char F41[] PROGMEM = "%-+4.1f";

const uint8_t LEFT_LABEL_LEFT = 15;
const uint8_t LEFT_VALUE_LEFT = 39;
const uint8_t RIGHT_LABEL_LEFT = 81;
const uint8_t RIGHT_VALUE_LEFT = 105;

const uint8_t L_LABEL_LEFT = 15;
const uint8_t L_VALUE_LEFT = L_LABEL_LEFT + 12;
const uint8_t M_LABEL_LEFT = L_VALUE_LEFT + 36;
const uint8_t M_VALUE_LEFT = M_LABEL_LEFT + 12;
const uint8_t R_LABEL_LEFT = M_VALUE_LEFT + 36;
const uint8_t R_VALUE_LEFT = R_LABEL_LEFT + 12;

ServoPage::ServoPage()
    :GUIPage(PSTR("Servo  Setup")),
     l_tmin(LEFT_LABEL_LEFT, 30, PSTR("Min")),
     l_tmax(RIGHT_LABEL_LEFT, 30, PSTR("Max")),
     l_tcen(LEFT_LABEL_LEFT, 40, PSTR("Cen")),
     l_t90(RIGHT_LABEL_LEFT, 40, PSTR("90\x80")),
 /*l_pauto(15, 65, PSTR("Auto")),
 l_pmin(15, 75, PSTR("Min")),
 l_pmax(81, 75, PSTR("Max")),
 l_pcen(15, 85, PSTR("Cen")),
 l_pacc(81, 85, PSTR("Acc")),*/

     l_sp(LEFT_LABEL_LEFT, 75, PSTR("P")),
     l_si(RIGHT_LABEL_LEFT, 75, PSTR("I")),
     l_sd(LEFT_LABEL_LEFT, 85, PSTR("D")),
     l_ap(LEFT_LABEL_LEFT, 110, PSTR("P")),
     l_ai(RIGHT_LABEL_LEFT, 110, PSTR("I")),
     l_ad(LEFT_LABEL_LEFT, 120, PSTR("D"))
     //cb_pauto(RIGHT_LABEL_LEFT, 65)
{
    curr_item = SERVO_SETUP_ITEM_TLT_MIN;
//    focus_item = -1;
}

void ServoPage::on_paint(Graphics* g)
{
    GUIPage::on_paint(g);
    if (invalid)
    {
        invalid_item = 0xffff;
        curr_item = SERVO_SETUP_ITEM_TLT_MIN;

        //GUIPage::on_paint(g);

        g->draw_string_p(3, 20, PSTR("Tilt Servo"));
        g->draw_string_p(3, 65, PSTR("Pan Servo Speed"));
		g->draw_string_p(3, 100, PSTR("Pan Servo Accelerate"));
		
        for (Label* l = &l_tmin; l <= &l_ad; l++)
            l->on_paint(g);
        //cb_pauto.on_paint(g);

        Label* curr = &l_tmin + curr_item;
        //g->draw_string(curr->get_left() - 6, curr->get_top(), Mark::Select);
        gui.get_cursor()->set(curr->get_left() - 6, curr->get_top(), CURSOR_TYPE_NAVIGATE, false);
        Serial2.printf("cursor %d %d\r\n", gui.get_cursor()->get_left(), gui.get_cursor()->get_top());
//        Serial.printf("inv %x\n", invalid_item);
    }

    //cb_pauto.on_paint(g);

    //StringBuilder sb;

    //tlt servo
    if (draw_item_value(g, SERVO_SETUP_ITEM_TLT_CENTER, (tlt_servo.get_cen() - MyServo::Center)/5,
                        LEFT_VALUE_LEFT, l_tcen.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_TLT_90, tlt_servo.get_half_pi() / 5,
                             RIGHT_VALUE_LEFT, l_t90.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_TLT_MIN, (tlt_servo.get_min() - MyServo::Center)/5,
                             LEFT_VALUE_LEFT, l_tmin.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_TLT_MAX, (tlt_servo.get_max() - MyServo::Center)/5,
                             RIGHT_VALUE_LEFT, l_tmax.get_top()));


    if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_SP, tracker.pid_spd.p, LEFT_VALUE_LEFT, l_sp.get_top()))	;
    else if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_SI, tracker.pid_spd.i, RIGHT_VALUE_LEFT, l_si.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_SD, tracker.pid_spd.d, LEFT_VALUE_LEFT, l_sd.get_top()));

    if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_AP, tracker.pid_acc.p, LEFT_VALUE_LEFT, l_ap.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_AI, tracker.pid_acc.i, RIGHT_VALUE_LEFT, l_ai.get_top()));
    else if (draw_item_value(g, SERVO_SETUP_ITEM_PAN_AD, tracker.pid_acc.d, LEFT_VALUE_LEFT, l_ad.get_top()));

    //accel

    invalid = false;
//    invalid_item = 0;
}

bool ServoPage::draw_item_value(Graphics* g, uint8_t item, float value, uint8_t left, uint8_t top)
{
    if (invalid_item & _BV(item))
    {
        StringBuilder sb;
        sb.printf_P(F41, value);
        g->draw_string(left, top, (const char*)sb);
        delay(1);
        //sb.clear();
        cbl(invalid_item, item);
        return true;
    }
    return false;
}

bool ServoPage::draw_item_value(Graphics* g, uint8_t item, int16_t value, uint8_t left, uint8_t top)
{
    if (invalid_item & _BV(item))
    {
        StringBuilder sb;
        sb.printf_P(I4, value);
        g->draw_string(left, top, (const char*)sb);
        delay(1);
        cbl(invalid_item, item);
        return true;
    }
    return false;
}


void ServoPage::on_key_press(uint8_t key_code)
{
//    if (ignore_key)
//        return;
    CURSOR_TYPE ctype = gui.get_cursor()->get_type();

    debug("ctype=%i\r\n", (int)ctype);

    if (key_code == KEY_S1)
    {
        Label* curr = 0;

        if (ctype == CURSOR_TYPE_NAVIGATE)
        {
            /*if (curr_item == SERVO_SETUP_ITEM_PAN_AUTO)
            {
            }
            else
            {*/
            //if (curr_item >= SERVO_SETUP_ITEM_PAN_AP)
            //    tracker.start();
            if (curr_item <= SERVO_SETUP_ITEM_TLT_90)
            	tracker.stop();
            	
            gui.get_cursor()->set_type(CURSOR_TYPE_SELECTED);
            adjust_value(key_code);
            //}
        }
        else
        {
            if (curr_item >= SERVO_SETUP_ITEM_TLT_MIN && curr_item <= SERVO_SETUP_ITEM_TLT_90)
            {
                tlt_servo.save();
                tlt_servo.set_angle(0);
                tracker.start();
            }
            /*else if (curr_item >= SERVO_SETUP_ITEM_PAN_MIN && curr_item <= SERVO_SETUP_ITEM_PAN_ACC)
            {
                pan_servo.save();
                pan_servo.set_angle(0);
            }*/
            else if (curr_item >= SERVO_SETUP_ITEM_PAN_SP)
            {
                //tracker.stop();
                tracker.save();
            }

            gui.get_cursor()->set_type(CURSOR_TYPE_NAVIGATE);
        }

        //curr = &l_tmin + abs(selected_item)-1;

        // if (curr == &l_pauto)
        // {
        //calibrate
        /*Graphics* g = gui.get_graphics();
        g->FillRect(87, l_accel.get_top() + 1, 5, 5, COLOR_WHITE);
        progress_row = l_accel.get_top();
        ignore_key = true;
        imu.init_accel(SetupPage::progress_delay);
        g->FillRect(87, l_accel.get_top(), 15, 8, COLOR_BLACK);
        g->DrawRect(87, l_accel.get_top() + 1, 5, 5);
        ahrs.reset(true, true);
        ignore_key = false;
        mode = SETUP_MODE_SELECT;*/

        //}
    }
    else if (key_code == KEY_UP || key_code == KEY_DOWN)
    {
        if (ctype == CURSOR_TYPE_SELECTED)
        {
            debug("adj servo\r\n");
            adjust_value(key_code);
        }
        else
        {
            if (key_code == KEY_DOWN)
            {
                //if (curr_item <= SERVO_SETUP_ITEM_TLT_CENTER
                //	|| curr_item == SERVO_SETUP_ITEM_PAN_AP
                //	|| curr_item == SERVO_SETUP_ITEM_PAN_SP)
                //    curr_item += 2;
                if (curr_item == SERVO_SETUP_ITEM_PAN_SI 
                		|| curr_item == SERVO_SETUP_ITEM_PAN_SD
                        || curr_item == SERVO_SETUP_ITEM_PAN_AI)
                    curr_item ++;
                else if (curr_item == SERVO_SETUP_ITEM_PAN_AD)
                    curr_item = SERVO_SETUP_ITEM_TLT_MIN;
                else
                    curr_item += 2;
            }
            else
            {
                if (curr_item < SERVO_SETUP_ITEM_TLT_CENTER)
                    curr_item = SERVO_SETUP_ITEM_PAN_AD;//SERVO_SETUP_ITEM_PAN_CENTER;
                else if (curr_item == SERVO_SETUP_ITEM_PAN_SP || curr_item == SERVO_SETUP_ITEM_PAN_AP)
                    curr_item --;
                else
                    //if (curr_item <= SERVO_SETUP_ITEM_PAN_AUTO)
                    curr_item -= 2;

            }
            Label* curr = &l_tmin + curr_item;
            gui.get_cursor()->move_to(curr->get_left()-6, curr->get_top());
        }
    }
    else if (key_code == KEY_LEFT || key_code == KEY_RIGHT)
    {
        if (ctype == CURSOR_TYPE_SELECTED)
            adjust_value(key_code);
        else
        {
            if (key_code == KEY_LEFT)
            {
                if (curr_item == SERVO_SETUP_ITEM_TLT_MIN)
                    curr_item = SERVO_SETUP_ITEM_PAN_SD;//SERVO_SETUP_ITEM_PAN_ACC;
                else
                    curr_item --;
            }
            else
            {
                if (curr_item == /*SERVO_SETUP_ITEM_PAN_ACC*/ SERVO_SETUP_ITEM_PAN_SD)
                    curr_item = SERVO_SETUP_ITEM_TLT_MIN;
                else
                    curr_item ++;
            }
            Label* curr = &l_tmin + curr_item;
            gui.get_cursor()->move_to(curr->get_left()-6, curr->get_top());
        }

    }
    else if (key_code == KEY_S2)
    {
        if (ctype == CURSOR_TYPE_NAVIGATE)
            gui.go_back();
        /*
        else if (focus_item <= SETUP_ITEM_SERVO_90)
        {
            if (selected_item == SETUP_ITEM_SERVO_MIN)
                tlt_servo.set_min(MyServo::Center);
            else if (selected_item == SETUP_ITEM_SERVO_MAX)
                tlt_servo.set_max(MyServo::Center);
            else if (selected_item == SETUP_ITEM_SERVO_CENTER)
                tlt_servo.set_cen(MyServo::Center);
            else if (selected_item == SETUP_ITEM_SERVO_90)
                tlt_servo.set_ninty(0);
            invalid_item |= _BV(selected_item);
            tlt_servo.set_pwm(MyServo::Center);
        }*/
    }
}

void ServoPage::on_load()
{
    //tracker.stop();
    invalidate();
}

void ServoPage::on_exit()
{
//    tlt_servo.save();
//    pan_servo.save();
    //tracker.start();
}

void ServoPage::adjust_value(uint8_t key_code)
{
    int16_t pwm = 0;
    int16_t adj = 0;

    if (key_code == KEY_UP)
        adj = 5;
    else if (key_code == KEY_DOWN)
        adj = -5;
    else if (key_code == KEY_RIGHT)
        adj = 50;
    else if (key_code == KEY_LEFT)
        adj = -50;

    if (curr_item >= SERVO_SETUP_ITEM_TLT_MIN &&  curr_item <= SERVO_SETUP_ITEM_TLT_90)
    {
        if (curr_item == SERVO_SETUP_ITEM_TLT_CENTER)
            pwm = tlt_servo.set_cen(tlt_servo.get_cen() + adj);
        else if (curr_item == SERVO_SETUP_ITEM_TLT_90)
            pwm = tlt_servo.get_cen() + tlt_servo.set_half_pi(tlt_servo.get_half_pi() + adj);
        else if (curr_item == SERVO_SETUP_ITEM_TLT_MIN)
            pwm = tlt_servo.set_min(tlt_servo.get_min() + adj);
        else
            pwm = tlt_servo.set_max(tlt_servo.get_max() + adj);
        debug("tlt pwm %d\r\n", pwm);
        tlt_servo.set_pwm(pwm);
    }
    /*else if (curr_item >= SERVO_SETUP_ITEM_PAN_MIN && curr_item <= SERVO_SETUP_ITEM_PAN_ACC)
    {
        if (curr_item == SERVO_SETUP_ITEM_PAN_CENTER)
            pwm = pan_servo.set_cen(pan_servo.get_cen() + adj);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_ACC)
            pwm = pan_servo.get_cen() + pan_servo.set_half_pi(tlt_servo.get_half_pi() + adj);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_MIN)
            pwm = pan_servo.set_min(pan_servo.get_min() + adj);
        else
            pwm = pan_servo.set_max(pan_servo.get_max() + adj);
        debug("pan pwm %d\r\n", pwm);
        pan_servo.set_pwm(pwm);
    }*/
    else if (curr_item >= SERVO_SETUP_ITEM_PAN_SP)
    {
        float adjf = adj * 0.02f;
        /*  if (curr_item == SERVO_SETUP_ITEM_PAN_P)
          {
              tracker.pan_p = constrain(tracker.pan_p + adjf, 0, 10);
          }
          else if (curr_item == SERVO_SETUP_ITEM_PAN_I)
          {
              tracker.pan_i = constrain(tracker.pan_i + adjf * 1.0e-7, 0, 1.0e-6);
          }
          else if (curr_item == SERVO_SETUP_ITEM_PAN_D)
          {
              tracker.pan_d = constrain(tracker.pan_d + adjf, 0, 20);
        //            debug("pan d %.2f", tracker.pan_d);
          }*/
        if (curr_item == SERVO_SETUP_ITEM_PAN_SP)
            tracker.pid_spd.p = constrain(tracker.pid_spd.p + adjf, 0, 50);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_SI)
            tracker.pid_spd.i = constrain(tracker.pid_spd.i + adjf, 0, 50);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_SD)
            tracker.pid_spd.d = constrain(tracker.pid_spd.d + adjf, 0, 100);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_AP)
            tracker.pid_acc.p = constrain(tracker.pid_acc.p + adjf, 0, 25);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_AI)
            tracker.pid_acc.i = constrain(tracker.pid_acc.i + adjf, 0, 25);
        else if (curr_item == SERVO_SETUP_ITEM_PAN_AD)
            tracker.pid_acc.d = constrain(tracker.pid_acc.d + adjf, 0, 25);
    }
    sbl(invalid_item, curr_item);
}
