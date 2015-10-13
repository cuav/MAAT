/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_INERTIAL_SENSOR_MPU6000_I2C_H__
#define __AP_INERTIAL_SENSOR_MPU6000_I2C_H__

#include <string.h>
#include <stdint.h>

//#include "../AP_PeriodicProcess/AP_PeriodicProcess.h"
//#include "../AP_Math/AP_Math.h"
#include <Matrix3.h>
#include <Vector3.h>
//#include "AP_InertialSensor.h"

#ifndef ToRad
#define ToRad(deg)	((deg)*0.0174533)
#endif

#ifndef ToDeg
#define ToDeg(rad)	((rad)*57.29578)
#endif

class AP_InertialSensor_MPU6000_I2C// : public AP_InertialSensor
{
public:

    AP_InertialSensor_MPU6000_I2C(uint8_t addr/*, uint8_t brd*/);

    bool init(/* AP_PeriodicProcess * scheduler */);

    /* Concrete implementation of AP_InertialSensor functions: */
    bool update();
    bool new_data_available();
    //float gx();
    //float gy();
    //float gz();
    //void get_gyros( float * );
    //float ax();
    //float ay();
    //float az();
    //void get_accels( float * );
    //void get_sensors( float * );
    float temperature();
    uint32_t sample_time();
    void reset_sample_time();
    float get_gyro_drift_rate();

    static bool read(/*uint32_t*/);
    
    float gyro[3];
    float accel[3];
private:


    static void hardware_init();

    //Vector3f _gyro;
    //Vector3f _accel;
    float _temp;

    uint32_t _last_sample_micros;

    float _temp_to_celsius( uint16_t );

    static const float _accel_scale;
    static const float _gyro_scale;

    static uint8_t _gyro_data_index[3];
    static int8_t _gyro_data_sign[3];

    static uint8_t _accel_data_index[3];
    static int8_t _accel_data_sign[3];

    static const uint8_t _temp_data_index;

    static int16_t _data[7];

    static uint8_t _board_Type;
    static int mpu_addr;

    // ensure we can't initialise twice
    unsigned _initialised:1;
};

typedef AP_InertialSensor_MPU6000_I2C AP_InertialSensor;

#endif // __AP_INERTIAL_SENSOR_MPU6000_I2C_H__
