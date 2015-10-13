#ifndef PID_H
#define PID_H

#include <eeprom.h>

class PID
{
public:
    float p;
    float i;
    float d;
    float I_MAX;
private:
    float I;
    float last_error;
    bool init;
public:
    PID(float _p = 0.0f, float _i = 0.0f, float _d = 0.0f, float _i_max = 0.0f)
    {
        set(_p, _i, _d, _i_max);
    }

    //PID():this(0.0f, 0.0f, 0.0f, 0.0f){}
	void set(float _p, float _i, float _d, float _i_max)
	{
		p = _p;
        i = _i;
        d = _d;
        I_MAX = _i_max;
        I = last_error = 0;
        init = true;
	}

    void load(void* addr)
    {
        if (eeprom.is_valid())
        {
            eeprom.read(&p, addr, sizeof(float)*3);
        }
        else
        {
            eeprom.write(&p, addr, sizeof(float)*3);
        }
    }

    void save(void* addr)
    {
        eeprom.write(&p, addr, sizeof(float)*3);
    }

    float update(float error, uint32_t diff_time)
    {
        float value = error * p;

        if (!init)
        {
            I += error * diff_time * i;

            if (I > I_MAX)
                I = I_MAX;
            else if (I < -I_MAX)
                I = -I_MAX;

            value += I + (error - last_error) * d;
        }
        init = false;

        last_error = error;

        return value;
    }

    void reset()
    {
        I = 0;
        last_error = 0;
        init = true;
    }
};

#endif
