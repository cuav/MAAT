#ifndef ANT_TRACKER_H
#define ANT_TRACKER_H

#include "pid.h"

class AntTracker
{
public:
    AntTracker(MyServo* servo_pan, MyServo* servo_tilt)    
    {
        eeprom_addr = eeprom.alloc(sizeof(float) * 6);
        sp = servo_pan;
        st = servo_tilt;
        pan_pwm = 1500;
        slow = true;
        reset();
    }

    void load()
    {
        pid_spd.load(eeprom_addr);
        pid_acc.load((float*)eeprom_addr + 3);
    }

    void save()
    {
        pid_spd.save(eeprom_addr);
        pid_acc.save((float*)eeprom_addr + 3);
    }
    
    void reset()
    {
    	pid_spd.set(3.0f, 0.0f, 40.0f, 0.0f);
    	pid_acc.set(5.0f, 0.0f, 20.0f, 0.0f);
    }

    void update();

    void stop()
    {
        run = false;
        sp->set_angle(0);
    }

    void start()
    {
        //Serial.println("tracker start");
        run = true;
        pan_pwm = 1500;
        pid_spd.reset();
        pid_acc.reset();
    }

    void slow_mode(bool on, uint32_t timeout)
    {
        slow = on;
        slow_timeout = timeout;
    }

    /*float get_pan_error()
    {
        return pan;
    }*/

public:
    PID pid_spd;
    PID pid_acc;
    //float pan_p, pan_i, pan_d,
    //rotate_p, rotate_i, rotate_d;
public:
    void* eeprom_addr;
    MyServo* sp, *st;

//    float tilt;
//    float pan;
    bool run;
    uint32_t last_update;

    int16_t pan_pwm;

    bool slow;
    uint32_t slow_timeout;

    static const float MAX_I = 15.0f;
    static const float MAX_R_I = 3.0f;
};
#endif
