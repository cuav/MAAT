#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <stdlib.h>

#include "rotations.h"

#define HALF_SQRT_2 0.70710678118654757

template <typename T>
class Vector3
{
public:
    T x, y, z;

    // trivial ctor
    Vector3<T>()
    {
        x = y = z = 0;
    }

    // setting ctor
    Vector3<T>(const T x0, const T y0, const T z0): x(x0), y(y0), z(z0) {}

    // function call operator
    void operator ()(const T x0, const T y0, const T z0)
    {
        x= x0;
        y= y0;
        z= z0;
    }

    /*
        T& operator[](uint8_t idx)
        {
            switch (idx)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
            	  return x;
            }
        }*/

    // test for equality
    bool operator ==(const Vector3<T> &v)
    {
        return (x==v.x && y==v.y && z==v.z);
    }

    // test for inequality
    bool operator !=(const Vector3<T> &v)
    {
        return (x!=v.x || y!=v.y || z!=v.z);
    }

    // negation
    Vector3<T> operator -(void) const
    {
        return Vector3<T>(-x,-y,-z);
    }

    // addition
    Vector3<T> operator +(const Vector3<T> &v) const
    {
        return Vector3<T>(x+v.x, y+v.y, z+v.z);
    }

    // subtraction
    Vector3<T> operator -(const Vector3<T> &v) const
    {
        return Vector3<T>(x-v.x, y-v.y, z-v.z);
    }

    // uniform scaling
    Vector3<T> operator *(const T num) const
    {
        Vector3<T> temp(*this);
        return temp*=num;
    }

    // uniform scaling
    Vector3<T> operator /(const T num) const
    {
        Vector3<T> temp(*this);
        return temp/=num;
    }

    // addition
    Vector3<T> &operator +=(const Vector3<T> &v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        return *this;
    }

    // subtraction
    Vector3<T> &operator -=(const Vector3<T> &v)
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        return *this;
    }

    // uniform scaling
    Vector3<T> &operator *=(const T num)
    {
        x*=num;
        y*=num;
        z*=num;
        return *this;
    }

    // uniform scaling
    Vector3<T> &operator /=(const T num)
    {
        x/=num;
        y/=num;
        z/=num;
        return *this;
    }

    // dot product
    T operator *(const Vector3<T> &v) const
    {
        return x*v.x + y*v.y + z*v.z;
    }

    // cross product
    Vector3<T> operator ^(const Vector3<T> &v) const
    {
        Vector3<T> temp(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
        return temp;
    }

	void add(const T& x, const T& y, const T& z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
	}
	
    // gets the length of this vector squared
    T length_squared() const
    {
        return (T)(*this * *this);
    }

    // gets the length of this vector
    float length() const
    {
        return (T)sqrt(*this * *this);
    }

    // normalizes this vector
    void normalize()
    {
        *this/=length();
    }

    // zero the vector
    void zero()
    {
        x = y = z = 0.0;
    }

    // returns the normalized version of this vector
    Vector3<T> normalized() const
    {
        return  *this/length();
    }

    // reflects this vector about n
    void reflect(const Vector3<T> &n)
    {
        Vector3<T> orig(*this);
        project(n);
        *this= *this*2 - orig;
    }

    // projects this vector onto v
    void project(const Vector3<T> &v)
    {
        *this= v * (*this * v)/(v*v);
    }

    // returns this vector projected onto v
    Vector3<T> projected(const Vector3<T> &v)
    {
        return v * (*this * v)/(v*v);
    }

    // computes the angle between 2 arbitrary vectors
    T angle(const Vector3<T> &v1, const Vector3<T> &v2)
    {
        return (T)acosf((v1*v2) / (v1.length()*v2.length()));
    }

    // computes the angle between 2 arbitrary normalized vectors
    T angle_normalized(const Vector3<T> &v1, const Vector3<T> &v2)
    {
        return (T)acosf(v1*v2);
    }

    // check if any elements are NAN
    bool is_nan(void)
    {
        return isnan(x) || isnan(y) || isnan(z);
    }

    // check if any elements are infinity
    bool is_inf(void)
    {
        return isinf(x) || isinf(y) || isinf(z);
    }

    // rotate by a standard rotation
    void rotate(enum Rotation rotation)
    {
        T tmp;
        switch (rotation)
        {
        case ROTATION_NONE:
        case ROTATION_MAX:
            return;
        case ROTATION_YAW_45:
        {
            tmp = HALF_SQRT_2*(x - y);
            y   = HALF_SQRT_2*(x + y);
            x = tmp;
            return;
        }
        case ROTATION_YAW_90:
        {
            tmp = x;
            x = -y;
            y = tmp;
            return;
        }
        case ROTATION_YAW_135:
        {
            tmp = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(x - y);
            x = tmp;
            return;
        }
        case ROTATION_YAW_180:
            x = -x;
            y = -y;
            return;
        case ROTATION_YAW_225:
        {
            tmp = HALF_SQRT_2*(y - x);
            y   = -HALF_SQRT_2*(x + y);
            x = tmp;
            return;
        }
        case ROTATION_YAW_270:
        {
            tmp = x;
            x = y;
            y = -tmp;
            return;
        }
        case ROTATION_YAW_315:
        {
            tmp = HALF_SQRT_2*(x + y);
            y   = HALF_SQRT_2*(y - x);
            x = tmp;
            return;
        }
        case ROTATION_ROLL_180:
        {
            y = -y;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_45:
        {
            tmp = HALF_SQRT_2*(x + y);
            y   = HALF_SQRT_2*(x - y);
            x = tmp;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_90:
        {
            tmp = x;
            x = y;
            y = tmp;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_135:
        {
            tmp = HALF_SQRT_2*(y - x);
            y   = HALF_SQRT_2*(y + x);
            x = tmp;
            z = -z;
            return;
        }
        case ROTATION_PITCH_180:
        {
            x = -x;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_225:
        {
            tmp = -HALF_SQRT_2*(x + y);
            y   =  HALF_SQRT_2*(y - x);
            x = tmp;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_270:
        {
            tmp = x;
            x = -y;
            y = -tmp;
            z = -z;
            return;
        }
        case ROTATION_ROLL_180_YAW_315:
        {
            tmp =  HALF_SQRT_2*(x - y);
            y   = -HALF_SQRT_2*(x + y);
            x = tmp;
            z = -z;
            return;
        }
        }
    }

};

typedef Vector3<float> 		Vector3f;
typedef Vector3<int16_t>	Vector3i;

#endif
