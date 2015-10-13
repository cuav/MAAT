/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
#include <FastSerial.h>
#include <math.h>
#include <eeprom.h>
#include <safemath.h>
#include <../debug.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif

#include "Compass.h"

//const size_t setting_size = OFFSET(Compass, _setting_rom) - OFFSET(Compass, _offset);
/*
const AP_Param::GroupInfo Compass::var_info[] PROGMEM = {
    // index 0 was used for the old orientation matrix
    AP_GROUPINFO("OFS",    1, Compass, _offset),
    AP_GROUPINFO("DEC",    2, Compass, _declination),
    AP_GROUPINFO("LEARN",  3, Compass, _learn), // true if learning calibration
    AP_GROUPINFO("USE",    4, Compass, _use_for_yaw), // true if used for DCM yaw
    AP_GROUPINFO("AUTODEC",5, Compass, _auto_declination),
    AP_GROUPEND
};
*/
// Default constructor.
// Note that the Vector/Matrix constructors already implicitly zero
// their values.
//
bool Compass::healthy = false;        ///< true if last read OKint Compass::product_id = AP_COMPASS_TYPE_UNKNOWN;
int Compass::product_id = AP_COMPASS_TYPE_UNKNOWN;
Compass::Compass(void) :
    _orientation(ROTATION_NONE),
    _declination		(0.0),
    _learn(1),
    _use_for_yaw(1),
    //_auto_declination(1),
    _null_enable(false),
    _null_init_done(false)
{
    _setting_rom = (uint8_t*)eeprom.alloc(offsetof(Compass, _setting_rom) - offsetof(Compass, _offset));
    _offset[0] = _offset[1] = _offset[2] = 0;
    
}

// Default init method, just returns success.
//
bool
Compass::init()
{
    return true;
}

void
Compass::set_orientation(enum Rotation rotation)
{
    _orientation = rotation;
}

void
Compass::set_offsets(const Vector3f &offsets)
{
    //_offset.set(offsets);
    _offset[0] = offsets.x;
    _offset[1] = offsets.y;
    _offset[2] = offsets.z;
}

void
Compass::save_offsets()
{
    //_offset.save();
    debug("Compass save offset %f %f %f\n", _offset[0], _offset[1], _offset[2]);
    eeprom.write(_offset, _setting_rom, sizeof(_offset));
}

void
Compass::save()
{
	//    Serial.printf("Compass save offset %f %f %f\n", _offset[0], _offset[1], _offset[2]);
    eeprom.write(_offset, _setting_rom, offsetof(Compass, _setting_rom) - offsetof(Compass, _offset));
}

void
Compass::load()
{
    if (eeprom.is_valid())
    {
        eeprom.read(_offset, _setting_rom, offsetof(Compass, _setting_rom) - offsetof(Compass, _offset));
        if (isnan(_offset[0]) || isnan(_offset[1]) || isnan(_offset[2]))
        	_offset[0] = _offset[1] = _offset[2] = 0;
        
        debug("Load Compass offset %f %f %f\n", _offset[0], _offset[1], _offset[2]);
    }
    else
    {
    	//eeprom.write(_offset, _setting_rom, offsetof(Compass, _setting_rom) - offsetof(Compass, _offset));
    		//save_offsets();
    	save();
    }
}

float*
Compass::get_offsets()
{
    return _offset;
}


void
Compass::set_initial_location(long latitude, long longitude)
{
    // if automatic declination is configured, then compute
    // the declination based on the initial GPS fix
    /*if (_auto_declination) {
    	// Set the declination based on the lat/lng from GPS
    	null_offsets_disable();
    	_declination.set(radians(AP_Declination::get_declination((float)latitude / 10000000, (float)longitude / 10000000)));
    	null_offsets_enable();
    }*/
}

void
Compass::set_declination(float radians)
{
    //_declination.set_and_save(radians);
    _declination = radians;
    //eeprom.write(&_declination, _setting_rom + offsetof(Compass, _declination) - offsetof(Compass, _offset), sizeof(_declination));
}

float
Compass::get_declination()
{
    return _declination;//.get();
}


void
Compass::calculate_heading(float roll, float pitch)
{
//  Note - This function implementation is deprecated
//  The alternate implementation of this function using the dcm matrix is preferred
    float headX;
    float headY;
    float cos_roll;
    float sin_roll;
    float cos_pitch;
    float sin_pitch;
    cos_roll = cos(roll);
    sin_roll = sin(roll);
    cos_pitch = cos(pitch);
    sin_pitch = sin(pitch);

    // Tilt compensated magnetic field X component:
    headX = mag_x*cos_pitch + mag_y*sin_roll*sin_pitch + mag_z*cos_roll*sin_pitch;
    // Tilt compensated magnetic field Y component:
    headY = mag_y*cos_roll - mag_z*sin_roll;
    // magnetic heading
    heading = atan2(-headY,headX);

    // Declination correction (if supplied)
    if( fabs(_declination) > 0.0 )
    {
        heading = heading + _declination;
        if (heading > M_PI)    // Angle normalization (-180 deg, 180 deg)
            heading -= (2.0 * M_PI);
        else if (heading < -M_PI)
            heading += (2.0 * M_PI);
    }

    // Optimization for external DCM use. Calculate normalized components
    heading_x = cos(heading);
    heading_y = sin(heading);
}


void
Compass::calculate_heading(const Matrix3f &dcm_matrix)
{
    float headX;
    float headY;
    float cos_pitch = safe_sqrt(1-(dcm_matrix.c.x*dcm_matrix.c.x));
    // sin(pitch) = - dcm_matrix(3,1)
    // cos(pitch)*sin(roll) = - dcm_matrix(3,2)
    // cos(pitch)*cos(roll) = - dcm_matrix(3,3)

    if (cos_pitch == 0.0)
    {
        // we are pointing straight up or down so don't update our
        // heading using the compass. Wait for the next iteration when
        // we hopefully will have valid values again.
        return;
    }

    // Tilt compensated magnetic field X component:
    headX = mag_x*cos_pitch - mag_y*dcm_matrix.c.y*dcm_matrix.c.x/cos_pitch - mag_z*dcm_matrix.c.z*dcm_matrix.c.x/cos_pitch;
    // Tilt compensated magnetic field Y component:
    headY = mag_y*dcm_matrix.c.z/cos_pitch - mag_z*dcm_matrix.c.y/cos_pitch;
    // magnetic heading
    // 6/4/11 - added constrain to keep bad values from ruining DCM Yaw - Jason S.
    heading = constrain(atan2(-headY,headX), -3.15, 3.15);

    // Declination correction (if supplied)
    if( fabs(_declination) > 0.0 )
    {
        heading = heading + _declination;
        if (heading > M_PI)    // Angle normalization (-180 deg, 180 deg)
            heading -= (2.0 * M_PI);
        else if (heading < -M_PI)
            heading += (2.0 * M_PI);
    }

    // Optimization for external DCM use. Calculate normalized components
    heading_x = cos(heading);
    heading_y = sin(heading);

#if 0
    if (isnan(heading_x) || isnan(heading_y))
    {
        Serial.printf("COMPASS: c.x %f c.y %f c.z %f cos_pitch %f mag_x %d mag_y %d mag_z %d headX %f headY %f heading %f heading_x %f heading_y %f\n",
                      dcm_matrix.c.x,
                      dcm_matrix.c.y,
                      dcm_matrix.c.x,
                      cos_pitch,
                      (int)mag_x, (int)mag_y, (int)mag_z,
                      headX, headY,
                      heading,
                      heading_x, heading_y);
    }
#endif
}


/*
  this offset nulling algorithm is inspired by this paper from Bill Premerlani

  http://gentlenav.googlecode.com/files/MagnetometerOffsetNullingRevisited.pdf

  The base algorithm works well, but is quite sensitive to
  noise. After long discussions with Bill, the following changes were
  made:

    1) we keep a history buffer that effectively divides the mag
       vectors into a set of N streams. The algorithm is run on the
       streams separately

    2) within each stream we only calculate a change when the mag
       vector has changed by a significant amount.

  This gives us the property that we learn quickly if there is no
  noise, but still learn correctly (and slowly) in the face of lots of
  noise.
 */
float
Compass::null_offsets(void)
{
    if (/*_null_enable == false || */_learn == 0)
    {
        // auto-calibration is disabled
        return 0;
    }

    // this gain is set so we converge on the offsets in about 5
    // minutes with a 10Hz compass
    const float gain = 0.02;
    const float max_change = 10.0;
    const float min_diff = 50.0;
    //Vector3f ofs;

//    ofs = _offset.get();

    if (!_null_init_done)
    {
        // first time through
        _null_init_done = true;
        for (uint8_t i=0; i<_mag_history_size; i++)
        {
            // fill the history buffer with the current mag vector,
            // with the offset removed
            _mag_history[i] = Vector3i((mag_x+0.5) - _offset[0]/*ofs.x*/,
                                       (mag_y+0.5) - _offset[1]/*ofs.y*/,
                                       (mag_z+0.5) - _offset[2]/*ofs.z*/);
        }
        _mag_history_index = 0;
        return 0;
    }

    Vector3f b1, b2, diff;
    float length;

    // get a past element
    b1 = Vector3f(_mag_history[_mag_history_index].x + _offset[0],
                  _mag_history[_mag_history_index].y + _offset[1],
                  _mag_history[_mag_history_index].z + _offset[2]);
    // the history buffer doesn't have the offsets
    //b1 += ofs;

    // get the current vector
    b2 = Vector3f(mag_x, mag_y, mag_z);

    // calculate the delta for this sample
    diff = b2 - b1;
    length = diff.length();
    if (length < min_diff)
    {
        // the mag vector hasn't changed enough - we don't get
        // enough information from this vector to use it.
        // Note that we don't put the current vector into the mag
        // history here. We want to wait for a larger rotation to
        // build up before calculating an offset change, as accuracy
        // of the offset change is highly dependent on the size of the
        // rotation.
        _mag_history_index = (_mag_history_index + 1) % _mag_history_size;
        return 1.0f;
    }

    // put the vector in the history
    _mag_history[_mag_history_index] = Vector3i((mag_x+0.5) - _offset[0]/*ofs.x*/,
                                       (mag_y+0.5) - _offset[1]/*ofs.y*/,
                                       (mag_z+0.5) - _offset[2]/*ofs.z*/);
    //Vector3i((mag_x+0.5) - ofs.x, (mag_y+0.5) - ofs.y, (mag_z+0.5) - ofs.z);
    _mag_history_index = (_mag_history_index + 1) % _mag_history_size;

    // equation 6 of Bills paper
    diff = diff * (gain * (b2.length() - b1.length()) / length);

    // limit the change from any one reading. This is to prevent
    // single crazy readings from throwing off the offsets for a long
    // time
    length = diff.length();
    if (length > max_change)
    {
        diff *= max_change / length;
    }

    // set the new offsets
    //_offset.set(_offset.get() - diff);
    //Serial.printf("Compass diff %f %f %f\n", diff.x, diff.y, diff.z);
    _offset[0] -= diff.x;
    _offset[1] -= diff.y;
    _offset[2] -= diff.z;

    return length;
}

void Compass::calibrate(void (*_delay)(uint32_t))
{
    /*uint8_t count = 0;
    uint32_t timeout = millis() + 2 * 60 * 1000;
    while (count < 16 && millis() < timeout)
    {
        if (read())
        {
            if (null_offsets() < 0.1)
                count++;
            else
                count = 0;
        }
        _delay(100);
    }
    save_offsets();*/
    
    //Serial.println("calibrate compass");
    //mag.set_offsets(0, 0, 0);
    memset(_offset, 0, sizeof(_offset));
    _in_calibrating = true;
    
    int16_t mag_min[3] = {0, 0, 0};
    int16_t mag_max[3] = {0, 0, 0};
    int16_t mag_read[3];

    //for (int i = 0; i < 300; i++)
    while (_in_calibrating)
    {
        _delay(100);
        if (read())
        {
            mag_read[0] = mag_x;
            mag_read[1] = mag_y;
            mag_read[2] = mag_z;
            for (int8_t j = 0; j < 3; j++)
            {
                mag_min[j] = min(mag_min[j], mag_read[j]);
                mag_max[j] = max(mag_max[j], mag_read[j]);
            }
//          mag.null_offsets();
//          float* off = mag.get_offsets();
            //Serial.printf("MAG OFF %.2f %.2f %.2f\n", off[0], off[1], off[2]);
        }
        else
        {
            //Serial.println("nani");
        }
    }
    set_offsets((-mag_min[0] - mag_max[0]) >> 1, (-mag_min[1] - mag_max[1]) >> 1, (-mag_min[2] - mag_max[2]) >> 1);
//    mag.save();
    save_offsets();
}


/*
void
Compass::null_offsets_enable(void)
{
    _null_init_done = false;
    _null_enable = true;
}

void
Compass::null_offsets_disable(void)
{
    _null_init_done = false;
    _null_enable = false;
}
*/