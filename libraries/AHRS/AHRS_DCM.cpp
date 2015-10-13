#include <FastSerial.h>
#include "AHRS_DCM.h"
#include <math.h>


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

void AHRS_DCM::init()
{
    //uint8_t axis;
    /*acc.init();
    gyro.init();
    mag.init();
    gyro.calibrate();
    */

    //_init_euler = false;
}

void AHRS_DCM::reset(bool recover_euler, bool recover_from_sensor)
{
    if (recover_from_sensor)
    {
        //update_sensor();
        //mag->update();
        //update();
        imu->update();

        //Serial.printf("ACCEL %f %f %f\n", _accel_vector.x, _accel_vector.y, _accel_vector.z);
        //roll
        _euler[0] = atan2(-_accel_vector.y, -_accel_vector.z);//atan2(acc.data[1] , acc.data[2]);
        //pitch
        _euler[1] = atan2(_accel_vector.x, sqrt(_accel_vector.y*_accel_vector.y + _accel_vector.z*_accel_vector.z));

        //Serial.printf("EULER %f %f\n", _euler[0], _euler[1]);

        //atan2(-_accel_vector.x, _accel_vector.z);//atan2(-acc.data[0] , acc.data[2]);
        if (mag)
        {
            mag->read();
            mag->calculate_heading(_euler[0], _euler[1]);
            _euler[2] = mag->heading;
            //Serial.println("kidding?");
            //mag->calculate(_euler[0], _euler[1]);
            //_euler[2] = mag->heading;
        }
        else
        {
            //Serial.println("right");
            _euler[2] = 0;
        }
    }

    //Serial.printf("DCM %f %f %f\n", _euler[0], _euler[1], _euler[2]);
    if (recover_euler)
        _dcm_matrix.from_euler(_euler[0], _euler[1], _euler[2]);
    else
        _dcm_matrix.from_euler(0, 0, 0);

    _omega_I.zero();
    _omega_P.zero();
    _omega_yaw_P.zero();
    //_last_update = micros();
    imu->reset_sample_time();
}

/*
void AHRS_DCM::set_zero()
{
    for (uint8_t i = 0; i < 3; i++)
        _euler_zero[i] = _euler[i];
}
*/

/*
void AHRS_DCM::calibrate(IMU_DEVICE device)
{
    if (device & IMU_ACC)
        acc.calibrate();
    if (device & IMU_MAG)
        mag.calibrate();
    if (device & IMU_GYRO)
        gyro.calibrate();

}
*/
/*
void AHRS_DCM::update_sensor()
{
    static int8_t count = 0;
    //acc.update();
    //gyro.update();
    //imu->read();
    imu->update();

    _gyro_vector = imu->get_gyro();
    _accel_vector = imu->get_accel();
    if (count++ > 5)
    {
        mag->update();
        mag->calculate(_dcm_matrix);
        count = 0;
    }
}*/

void AHRS_DCM::update()
{
    /*uint32_t now = micros();
    if (_last_update == 0)
    {
        _last_update = now;
        return;
    }

    float dt = ((uint32_t)(now - _last_update)) * 1.0e-6;
    //float dt = 0;
    _last_update = now;
    //update_sensor();*/
    imu->update();
    _gyro_vector = imu->get_gyro();
    _accel_vector = imu->get_accel();


    matrix_update(imu->sample_time() * 1.0e-6);
    normalize();
    drift_correction();
    euler_angles();
}


void AHRS_DCM::normalize()
{
    float error;
    Vector3f t0, t1, t2;

    error = _dcm_matrix.a * _dcm_matrix.b; 						// eq.18

    t0 = _dcm_matrix.a - (_dcm_matrix.b * (0.5f * error));		// eq.19
    t1 = _dcm_matrix.b - (_dcm_matrix.a * (0.5f * error));		// eq.19
    t2 = t0 ^ t1;					                // c= a x b // eq.20

    if (!renorm(t0, _dcm_matrix.a) ||
            !renorm(t1, _dcm_matrix.b) ||
            !renorm(t2, _dcm_matrix.c))
    {
        // Our solution is blowing up and we will force back
        // to last euler angles
        reset(true);
    }

}

void AHRS_DCM::drift_correction()
{
    //Compensation the Roll, Pitch and Yaw drift.
    //float mag_heading_x;
    //float mag_heading_y;
    float error_course = 0;
    float accel_magnitude;
    float accel_weight;
    float integrator_magnitude;
    //static float scaled_omega_P[3];
    //static float scaled_omega_I[3];
    static bool in_motion = false;
    Matrix3f rot_mat;
    float yaw_delta;

    //*****Roll and Pitch***************
    //Vector3f _accel_vector = acc.get_vector() * (9.80665f/512);
    // Calculate the magnitude of the accelerometer vector
    accel_magnitude = _accel_vector.length() / 9.80665f;

    // Dynamic weighting of accelerometer info (reliability filter)
    // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
    accel_weight = constrain(1 - 3 * fabs(1 - accel_magnitude), 0, 1); // upped to (<0.66G = 0.0, 1G = 1.0 , >1.33G = 0.0)

    // We monitor the amount that the accelerometer based drift correction is deweighted for performance reporting
    //_health = constrain(_health+(0.02 * (accel_weight - .5)), 0, 1);

    // adjust the ground of reference
    Vector3f _error_roll_pitch =   _dcm_matrix.c ^ _accel_vector;// ^ ;//_dcm_matrix.c ^ _accel_vector;                     // Equation 27  *** sign changed from prev implementation???

    //Serial.printf("ACC M %f\n", accel_magnitude);
    //Serial.printf("ERROR %f %f %f\n", _error_roll_pitch.x, _error_roll_pitch.y, _error_roll_pitch.z);
    // error_roll_pitch are in Accel m/s^2 units
    // Limit max error_roll_pitch to limit max omega_P and omega_I
    _error_roll_pitch.x = constrain(_error_roll_pitch.x, -1.17f, 1.17f);
    _error_roll_pitch.y = constrain(_error_roll_pitch.y, -1.17f, 1.17f);
    _error_roll_pitch.z = constrain(_error_roll_pitch.z, -1.17f, 1.17f);




    _omega_P =  _error_roll_pitch * (_kp_roll_pitch * accel_weight);
    _omega_I += _error_roll_pitch * (_ki_roll_pitch * accel_weight);



    //if (mag)
    if (mag && _compass_last_update != mag->last_update)
    {
        yaw_delta = 1.0e-6*(mag->last_update - _compass_last_update);
        _compass_last_update = mag->last_update;
        error_course = (_dcm_matrix.a.x * mag->heading_y) - (_dcm_matrix.b.x * mag->heading_x);

        //error_course*=-1;
        if (error_course > PI)
        {
            error_course -= 2*PI;
        }
        else if (error_course < -PI)
        {
            error_course += 2*PI;
        }


        Vector3f error = _dcm_matrix.c * error_course;
        _omega_yaw_P = error * _kp_yaw;
        _omega_P += _omega_yaw_P;
        _omega_I.z += error.z * _ki_yaw /** yaw_delta*/;
        //_omega_I += _dcm_matrix.c * (error_course * _ki_yaw);
    }
    else
    {
        _omega_yaw_P *= 0.97;
        _omega_P += _omega_yaw_P;
        //_omega_I.z +=
    }

    /*
            Vector3f _error_yaw = _dcm_matrix.c * error_course;

            //_omega_P = _omega_P + _dcm_matrix.c * (error_course * _kp_yaw);
            //_omega_P.x += _error_yaw.x * _kp_yaw;//_error_yaw.x * _kp_yaw;
            if (mag)
            	{
            _omega_P += (_error_yaw * _kp_yaw);
            _omega_I += (_error_yaw * _ki_yaw);
          }
      */

    integrator_magnitude = _omega_I.length();
    if (integrator_magnitude > 30 * PI / 180)
    {
        _omega_I *= (30 * PI / 180 / integrator_magnitude);
    }
}

void AHRS_DCM::matrix_update(float _G_Dt)
{
    // _omega_integ_corr is used for _centripetal correction
    // (theoretically better than _omega)
    _omega_integ_corr = /*gyro.get_vector()*/_gyro_vector + _omega_I;

    // Equation 16, adding proportional and integral correction terms
    _omega = _omega_integ_corr + _omega_P;// + _omega_yaw_P;

    // this is a replacement of the DCM matrix multiply (equation
    // 17), with known zero elements removed and the matrix
    // operations inlined. This runs much faster than the original
    // version of this code, as the compiler was doing a terrible
    // job of realising that so many of the factors were in common
    // or zero. It also uses much less stack, as we no longer need
    // two additional local matrices

    Vector3f r = _omega * _G_Dt;
    _dcm_matrix.rotate(r);

}

int16_t wrap_180(int16_t angle)
{
    if (angle > 1800)
        angle -= 3600;
    else if (angle < -1800)
        angle += 3600;

    return angle;
}

void AHRS_DCM::euler_angles(void)
{
    _dcm_matrix.to_euler(&_euler[0], &_euler[1], &_euler[2]);
    for (uint8_t i = 0; i < 3; i++)
        euler_angle[i] = wrap_180((_euler[i]/* - _euler_zero[i]*/) * 572.958f); // 1800 / PI

    /*
    if (euler_angle[2] < -1800)
    	euler_angle[2] += 3600;
    else if (euler_angle[2] > 1800)
    	euler_angle[2] -= 3600;
    */
}

bool AHRS_DCM::renorm(Vector3f const &a, Vector3f &result)
{
    float	renorm_val;

    renorm_val = a * a;

    if (renorm_val < 1.5625f && renorm_val > 0.64f)                         // Check if we are OK to use Taylor expansion
    {
        renorm_val = 0.5 * (3 - renorm_val);                                    // eq.21
    }
    else if (renorm_val < 100.0f && renorm_val > 0.01f)
    {
        renorm_val = 1.0 / sqrt(renorm_val);
        //    renorm_sqrt_count++;
    }
    else
    {
        //    problem = 1;
        return false;
        //    renorm_blowup_count++;
    }

    result = a * renorm_val;
    return true;
}


/*
float
AHRS_DCM::yaw_error_compass(void)
{
	Vector3f mag = Vector3f(_compass->mag_x, _compass->mag_y, _compass->mag_z);
	// get the mag vector in the earth frame
	Vector3f rb = _dcm_matrix * mag;

	rb.normalize();
	if (rb.is_inf()) {
		// not a valid vector
		return 0.0;
	}

	// get the earths magnetic field (only X and Y components needed)
	Vector3f mag_earth = Vector3f(cos(_compass->get_declination()),
				      sin(_compass->get_declination()), 0);

	// calculate the error term in earth frame
	Vector3f error = rb % mag_earth;

	return error.z;
}

void AHRS_DCM::drift_correction_yaw(void)
{
	bool new_value = false;
	float yaw_error;
	float yaw_deltat;

	if (_compass && _compass->use_for_yaw()) {
		if (_compass->last_update != _compass_last_update) {
			yaw_deltat = (_compass->last_update - _compass_last_update) * 1.0e-6;
			_compass_last_update = _compass->last_update;
			if (!_have_initial_yaw) {
				float heading = _compass->calculate_heading(_dcm_matrix);
				_dcm_matrix.from_euler(roll, pitch, heading);
				_omega_yaw_P.zero();
				_have_initial_yaw = true;
			}
			new_value = true;
			yaw_error = yaw_error_compass();
		}
	} else if (_fly_forward && _gps && _gps->status() == GPS::GPS_OK) {
		if (_gps->last_fix_time != _gps_last_update &&
		    _gps->ground_speed >= GPS_SPEED_MIN) {
			yaw_deltat = (_gps->last_fix_time - _gps_last_update) * 1.0e-3;
			_gps_last_update = _gps->last_fix_time;
			if (!_have_initial_yaw) {
				_dcm_matrix.from_euler(roll, pitch, ToRad(_gps->ground_course*0.01));
				_omega_yaw_P.zero();
				_have_initial_yaw = true;
			}
			new_value = true;
			yaw_error = yaw_error_gps();
		}
	}

	if (!new_value) {
		// we don't have any new yaw information
		// slowly decay _omega_yaw_P to cope with loss
		// of our yaw source
		_omega_yaw_P *= 0.97;
		return;
	}

	// the yaw error is a vector in earth frame
	Vector3f error = Vector3f(0,0, yaw_error);

	// convert the error vector to body frame
	error = _dcm_matrix.mul_transpose(error);

	// the spin rate changes the P gain, and disables the
	// integration at higher rates
	float spin_rate = _omega.length();

	// update the proportional control to drag the
	// yaw back to the right value. We use a gain
	// that depends on the spin rate. See the fastRotations.pdf
	// paper from Bill Premerlani

	_omega_yaw_P.z = error.z * _P_gain(spin_rate) * _kp_yaw.get();

	// don't update the drift term if we lost the yaw reference
	// for more than 2 seconds
	if (yaw_deltat < 2.0 && spin_rate < ToRad(SPIN_RATE_LIMIT)) {
		// also add to the I term
		_omega_I_sum.z += error.z * _ki_yaw * yaw_deltat;
	}

	_error_yaw_sum += fabs(yaw_error);
	_error_yaw_count++;
}
*/
