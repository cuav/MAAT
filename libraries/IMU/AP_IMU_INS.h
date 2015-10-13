// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/// @file	AP_IMU_INS.h
/// @brief	IMU driver on top of an AP_InertialSensor (INS) driver.
//          Provides offset calibration for for the gyro and accel.

#ifndef __AP_IMU_INS_H__
#define __AP_IMU_INS_H__


//#include "../AP_Common/AP_Common.h"
//#include "../AP_Math/AP_Math.h"
//#include "../AP_InertialSensor/AP_InertialSensor.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#include <inttypes.h>
#include <eeprom.h>

//#include "IMU.h"
#include "AP_InertialSensor_MPU6000_I2C.h"




class AP_IMU_INS //: public IMU
{
public:
    enum Start_style
    {
        COLD_START = 0,
        WARM_START
    };

public:
    /// Constructor
    ///
    /// Saves the ADC pointer and constructs the calibration data variable.
    ///
    /// @param  adc         Pointer to the AP_ADC instance that is connected to the gyro and accelerometer.
    /// @param  key         The AP_Var::key value we will use when loading/saving calibration data.
    ///
    AP_IMU_INS(AP_InertialSensor *ins) :
        _ins(ins)
    {
        //_product_id = AP_PRODUCT_ID_NONE; // set during hardware init
        _sensor_cal_rom = eeprom.alloc(sizeof(_sensor_cal));
    }

    /// Do warm or cold start.
    ///
    /// @note   For a partial-warmstart where e.g. the accelerometer calibration should be preserved
    ///         but the gyro cal needs to be re-performed, start with ::init(WARM_START) to load the
    ///         previous calibration settings, then force a re-calibration of the gyro with ::init_gyro.
    ///
    /// @param  style   Selects the initialisation style.
    ///                 COLD_START performs calibration of both the accelerometer and gyro.
    ///                 WARM_START loads accelerometer and gyro calibration from a previous cold start.
    ///
    void init( Start_style style = COLD_START,
                              void (*delay_cb)(unsigned long t) = delay,
                              void (*flash_leds_cb)(bool on) = NULL
                                      /*AP_PeriodicProcess *scheduler = NULL */);

    void save();
    void init_accel(void (*delay_cb)(unsigned long t) = delay,
                                   void (*flash_leds_cb)(bool on) = NULL);
    void init_gyro(void (*delay_cb)(unsigned long t) = delay,
                                  void (*flash_leds_cb)(bool on) = NULL);
    bool update(void);
    bool new_data_available(void);

	void reset_calibration();
	
    virtual float get_gyro_drift_rate(void);


    const Vector3f&	get_gyro(void)
    {
        return _gyro;
    }

    /// Fetch the current accelerometer values
    ///
    /// @returns	vector of current accelerations in m/s/s
    ///
    const Vector3f&	get_accel(void)
    {
        return _accel;
    }
    
    uint32_t sample_time() {return _sample_time;}
    
    void reset_sample_time() {_ins->reset_sample_time();}

private:
    AP_InertialSensor   *_ins;          ///< INS provides an axis and unit correct sensor source.

    virtual void        _init_accel(void (*delay_cb)(unsigned long t),
                                    void (*flash_leds_cb)(bool on) = NULL);  ///< no-save implementation
    virtual void        _init_gyro(void (*delay_cb)(unsigned long t),
                                   void (*flash_leds_cb)(bool on) = NULL);   ///< no-save implementation

    float _calibrated(uint8_t channel, float ins_value);

    // Gyro and Accelerometer calibration criteria
    //
    static const float		_accel_total_cal_change = 4.0;
    static const float		_accel_max_cal_offset = 250.0;


//AP_Vector6f         _sensor_cal;    ///< Calibrated sensor offsets

    float _sensor_cal[6];
    void* _sensor_cal_rom;

    /// Most recent accelerometer reading obtained by ::update
    Vector3f _accel;

    /// Most recent gyro reading obtained by ::update
    Vector3f _gyro;

    /// number of microseconds that the accel and gyro values
    /// were sampled over
    uint32_t _sample_time;

//	AP_Int16 	   _product_id; // this is the product id returned from the INS init

};

#endif
