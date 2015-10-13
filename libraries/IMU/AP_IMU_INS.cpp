// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
//
//
//	AP_IMU_INS.cpp - IMU Sensor Library for Ardupilot Mega
//		Code by Michael Smith, Doug Weibel, Jordi Muñoz and Jose Julio. DIYDrones.com
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//

/// @file	AP_IMU_INS.cpp
/// @brief	IMU driver on top of an INS driver. Provides calibration for the
//          inertial sensors (gyro and accel)

#include <FastSerial.h>
//#include <AP_Common.h>

//#include <avr/eeprom.h>


#include "AP_IMU_INS.h"
#include <debug.h>


//float _sensor_cal_eeprom[6] EEMEM;

void
AP_IMU_INS::init( Start_style style,
                  void (*delay_cb)(unsigned long t),
                  void (*flash_leds_cb)(bool on)//,
                  /*AP_PeriodicProcess * scheduler*/ )
{
    //_product_id = _ins->init(scheduler);
    // if we are warm-starting, load the calibration data from EEPROM and go
    //
    if (WARM_START == style)
    {
        //_sensor_cal.load();
        eeprom.read(_sensor_cal, _sensor_cal_rom, sizeof(_sensor_cal));
        //eeprom_read_block(_sensor_cal_eeprom, _sensor_cal, sizeof(_sensor_cal));
    }
    else
    {

        // do cold-start calibration for both accel and gyro
        // read accel cal
        eeprom.read(_sensor_cal, _sensor_cal_rom, sizeof(_sensor_cal));
        
        _init_gyro(delay_cb, flash_leds_cb);

        // save calibration
        //_sensor_cal.save();
        save();
    }
}

void
AP_IMU_INS::reset_calibration()
{
	memset(_sensor_cal, 0, sizeof(_sensor_cal));
	_sensor_cal[5] = 9.805;
}

/**************************************************/
#define FLASH_LEDS(on) do { if (flash_leds_cb != NULL) flash_leds_cb(on); } while (0)

void
AP_IMU_INS::init_gyro(void (*delay_cb)(unsigned long t), void (*flash_leds_cb)(bool on))
{
    _init_gyro(delay_cb, flash_leds_cb);
    //_sensor_cal.save();
    save();
    FLASH_LEDS(false);
}

void
AP_IMU_INS::_init_gyro(void (*delay_cb)(unsigned long t), void (*flash_leds_cb)(bool on))
{
    Vector3f last_average, best_avg;
    float ins_gyro[3];
    float best_diff = 0;

    // cold start
    delay_cb(100);
    debug_P(PSTR("Init Gyro"));

    for(int c = 0; c < 75; c++)
    {
        // Mostly we are just flashing the LED's here
        // to tell the user to keep the IMU still
        FLASH_LEDS(true);
        delay_cb(20);

        _ins->update();
        //_ins->get_gyros(ins_gyro);

        FLASH_LEDS(false);
        delay_cb(20);
    }

    // the strategy is to average 200 points over 1 second, then do it
    // again and see if the 2nd average is within a small margin of
    // the first

    last_average.zero();

    // we try to get a good calibration estimate for up to 10 seconds
    // if the gyros are stable, we should get it in 2 seconds
    for (int j = 0; j <= 10; j++)
    {
        Vector3f gyro_sum, gyro_avg, gyro_diff;
        float diff_norm;
        uint8_t i;

        debug_P(PSTR("*"));

        gyro_sum.zero();
        for (i = 0; i < 200; i++)
        {
            _ins->update();
            //_ins->get_gyros(ins_gyro);
            gyro_sum.add(_ins->gyro[0], _ins->gyro[1], _ins->gyro[2]);
            if (i % 40 == 20)
            {
                FLASH_LEDS(true);
            }
            else if (i % 40 == 0)
            {
                FLASH_LEDS(false);
            }
            delay_cb(5);
        }
        gyro_avg = gyro_sum / i;

        gyro_diff = last_average - gyro_avg;
        diff_norm = gyro_diff.length();

        if (j == 0)
        {
            best_diff = diff_norm;
            best_avg = gyro_avg;
        }
        else if (gyro_diff.length() < ToRad(0.02))
        {
            // we want the average to be within 0.1 bit, which is 0.04 degrees/s
            last_average = (gyro_avg * 0.5) + (last_average * 0.5);
            _sensor_cal[0] = last_average.x;
            _sensor_cal[1] = last_average.y;
            _sensor_cal[2] = last_average.z;
            // all done
            return;
        }
        else if (diff_norm < best_diff)
        {
            best_diff = diff_norm;
            best_avg = (gyro_avg * 0.5) + (last_average * 0.5);
        }
        last_average = gyro_avg;
    }

    // we've kept the user waiting long enough - use the best pair we
    // found so far
    debug_P(PSTR("\ngyro did not converge: diff=%f dps\n"), ToDeg(best_diff));

    _sensor_cal[0] = best_avg.x;
    _sensor_cal[1] = best_avg.y;
    _sensor_cal[2] = best_avg.z;
}

void
AP_IMU_INS::save()
{
    //eeprom_write_block(_sensor_cal_eeprom, _sensor_cal, sizeof(_sensor_cal));
    eeprom.write(_sensor_cal, _sensor_cal_rom, sizeof(_sensor_cal));
    //_sensor_cal.save();
}

void
AP_IMU_INS::init_accel(void (*delay_cb)(unsigned long t), void (*flash_leds_cb)(bool on))
{
    _init_accel(delay_cb, flash_leds_cb);
    //_sensor_cal.save();
    save();
    FLASH_LEDS(false);
}

void
AP_IMU_INS::_init_accel(void (*delay_cb)(unsigned long t), void (*flash_leds_cb)(bool on))
{
    int flashcount = 0;
    float adc_in;
    float prev[6] = {0,0,0,0,0,0};
    float total_change;
    float max_offset;
    //float* ins_accel = ins->accel;


    // cold start
    delay_cb(500);

    debug_P(PSTR("Init Accel"));

    for (int j=3; j<=5; j++) _sensor_cal[j] = 500;		// Just a large value to load prev[j] the first time

    do
    {
        _ins->update();
        //_ins->get_accels(ins_accel);

        for (int j = 3; j <= 5; j++)
        {
            prev[j] = _sensor_cal[j];
            adc_in 		    = _ins->accel[j-3];
            _sensor_cal[j]	= adc_in;
        }

        for(int i = 0; i < 50; i++) 		// We take some readings...
        {

            delay_cb(20);
            _ins->update();
            //_ins->get_accels(ins_accel);

            for (int j = 3; j < 6; j++)
            {
                adc_in 	    	= _ins->accel[j-3];
                _sensor_cal[j]	= _sensor_cal[j] * 0.9 + adc_in * 0.1;
            }

            if(flashcount == 5)
            {
                debug_P(PSTR("*"));
                FLASH_LEDS(true);
            }

            if(flashcount >= 10)
            {
                flashcount = 0;
                FLASH_LEDS(false);
            }
            flashcount++;
        }

        // null gravity from the Z accel
        _sensor_cal[5] += 9.805;
        //_sensor_cal[5] -= 9.805;

        total_change = fabs(prev[3] - _sensor_cal[3]) + fabs(prev[4] - _sensor_cal[4]) +fabs(prev[5] - _sensor_cal[5]);
        max_offset = (_sensor_cal[3] > _sensor_cal[4]) ? _sensor_cal[3] : _sensor_cal[4];
        max_offset = (max_offset > _sensor_cal[5]) ? max_offset : _sensor_cal[5];

        delay_cb(500);
    }
    while (  total_change > _accel_total_cal_change || max_offset > _accel_max_cal_offset);

    debug_P(PSTR(" "));
}

float
AP_IMU_INS::_calibrated(uint8_t channel, float ins_value)
{
    return ins_value - _sensor_cal[channel];
}


bool
AP_IMU_INS::update(void)
{
    //float gyros[3];
    //float accels[3];

    _ins->update();
    //_ins->get_gyros(gyros);
    //_ins->get_accels(accels);
    _sample_time = _ins->sample_time();

    // convert corrected gyro readings to delta acceleration
    //
    _gyro.x = /*_gyro.x * 0.7 + 0.3 **/ _calibrated(0, _ins->gyro[0]);
    _gyro.y = /*_gyro.y * 0.7 + 0.3 **/ _calibrated(1, _ins->gyro[1]);
    _gyro.z = /*_gyro.z * 0.7 + 0.3 **/ _calibrated(2, _ins->gyro[2]);

    // convert corrected accelerometer readings to acceleration
    //
    _accel.x = _calibrated(3, _ins->accel[0]);
    _accel.y = _calibrated(4, _ins->accel[1]);
    _accel.z = _calibrated(5, _ins->accel[2]);

    // always updated
    return true;
}

bool AP_IMU_INS::new_data_available(void)
{
    return _ins->new_data_available();
}

/// return the maximum gyro drift rate in radians/s/s. This
/// depends on what gyro chips are being used
float AP_IMU_INS::get_gyro_drift_rate(void)
{
    return _ins->get_gyro_drift_rate();
}
