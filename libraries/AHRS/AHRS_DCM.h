#ifndef AHRS_H
#define AHRS_H

//#include "ACCEL.h"
//#include "Gyro.h"
#include <Compass.h>
#include <AP_IMU_INS.h>
#include <Vector3.h>
#include <Matrix3.h>

enum IMU_DEVICE {IMU_ACC = 1, IMU_MAG = 2, IMU_GYRO = 4, IMU_ALL = 7};

class AHRS_DCM
{
public:
    AHRS_DCM(AP_IMU_INS* imu, Compass* mag)
    {
        this->imu = imu;
        this->mag = mag;
        //_gyro_drift_limit

        for (uint8_t axis = 0; axis < 3; axis++)
        {
            _euler[axis] = /*_euler_zero[axis] =*/ 0;
            euler_angle[axis] = 0;
        }
        //enable = true;
        _dcm_matrix.identity();
        //_kp_roll_pitch = 0.06;
        //_kp_roll_pitch = 0.13;
        _kp_roll_pitch = 0.05967;
        _kp_yaw = 0.2;//1.2;
        //_ki_roll_pitch = imu->get_gyro_drift_rate() * 5;//0.00002;//0.00005;
        _ki_roll_pitch = 0.00001278;
        _ki_yaw = 0.00004;//imu->get_gyro_drift_rate() * 8;// 0.00005;
    }

    void init();
    //void update_sensor();
    void update();
    //void calibrate(IMU_DEVICE device);

    inline void load_setting()
    {
        //acc.load_setting();
        //mag.load_setting();
    }

    int16_t euler_angle[3];
    /*
    inline bool ready()
    {
        return acc.ready() && mag.ready() && gyro.ready();
    }*/

    void reset(bool recover_euler = false, bool recover_from_sensor = false);
    //void set_zero();

    const Matrix3f& get_dcm_matrix()
    {
        return _dcm_matrix;
    }
public:
    Compass* mag;
    AP_IMU_INS* imu;
    //Gyro gyro;
    //Accelerometer acc;
    //bool enable;

//private:
    void normalize();
    void drift_correction();
    void matrix_update(float _G_Dt);
    bool renorm(Vector3f const &a, Vector3f &result);
    void euler_angles();
    //float yaw_error_compass();
public:
    //int16_t zero[3];
    //int16_t raw[3];
    float _euler[3];
    //float _euler_zero[3];
    //float roll, pitch, yaw;
    //float roll_zero, pitch_zero, yaw_zero;

    Matrix3f	_dcm_matrix;

    Vector3f 	_gyro_vector;		// Store the gyros turn rate in a vector
    Vector3f 	_accel_vector;		// current accel vector

    Vector3f	_omega_P;			// accel Omega Proportional correction
    Vector3f	_omega_yaw_P;		// yaw Omega Proportional correction
    Vector3f 	_omega_I;			// Omega Integrator correction
    Vector3f 	_omega_I_sum;		// summation vector for omegaI
    float		_omega_I_sum_time;
    Vector3f 	_omega_integ_corr;	// Partially corrected Gyro_Vector data - used for centrepetal correction
    Vector3f 	_omega;				// Corrected Gyro_Vector data

    float		_kp_roll_pitch;
    float		_ki_roll_pitch;
    float		_ki_yaw;
    float  _kp_yaw;

    uint32_t _compass_last_update;
    //uint32_t _last_update;
    //bool _init_euler;
};

//extern AHRS ahrs;

#endif
