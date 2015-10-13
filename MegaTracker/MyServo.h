#ifndef MY_SERVO_H
#define MY_SERVO_H

#include <stdint.h>
#include <eeprom.h>

enum SERVO_PORT
{
    SERVO_PORT_T3A, SERVO_PORT_T3B//SERVO_PORT_T4A, SERVO_PORT_T4B, SERVO_PORT_T4C
};

class MyServo
{
public:
    MyServo(SERVO_PORT port, int16_t min = Center, int16_t max = Center, int16_t cen = Center);

    static void init();

    void start();
    void stop();

    void load();

    void save()
    {
        eeprom.write(&this->cen, rom_addr, offsetof(MyServo, rom_addr) - offsetof(MyServo, cen));
    }
    
    void reset();

    void set_angle(float angle)
    {
        set_pwm(angle * hpi / 1.570796f + cen);
    }

    void set_pwm(int16_t value);
    
    inline int16_t set_min(int16_t value)
    {
        return min = constrain(value, Minim, Maxim);
    }
    
    inline int16_t set_max(int16_t value)
    {
        return max = constrain(value, Minim, Maxim);
    }
    
    inline int16_t set_cen(int16_t value)
    {
        return cen = constrain(value, Minim, Maxim);
    }
    
    inline int16_t set_half_pi(int16_t value)
    {
        return hpi = constrain(value, -1000, 1000);
    }
    
    inline int16_t set_acc(int16_t value)
    {
        return acc = constrain(value, -1000, 1000);
    }
    
    inline int16_t set_dzone(int16_t value)
    {
        return dzone = constrain(value, 0, 100);
    }

    inline int16_t get_min()
    {
        return min;
    }
    
    inline int16_t get_max()
    {
        return max;
    }
    
    inline int16_t get_cen()
    {
        return cen;
    }
    
    inline int16_t get_half_pi()
    {
        return hpi;
    }
    
    inline int16_t get_acc()
    {
        return acc;
    }
    
    inline int16_t get_dzone()
    {
        return dzone;
    }

    //int16_t get_value();

    inline int16_t get_raw(int16_t curr)
    {
        return constrain(dst, curr - acc, curr + acc);
    }

    static const int16_t Center = 1500;
    static const int16_t Minim = 500;
    static const int16_t Maxim = 2500;

    //setup
private:
    int16_t cen;
    int16_t max;
    int16_t min;
    int16_t hpi;
    int16_t acc;
    int16_t dzone;
//    int8_t rev;


    void* rom_addr;
    uint8_t port;
    //runtime
    volatile  int16_t dst;  //for speed control
//    uint16_t cur;
};


#endif

