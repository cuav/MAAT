#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector3.h"


float safe_asin(float v);

template <typename T>
class Matrix3
{
public:

    // Vectors comprising the rows of the matrix
    Vector3<T>	a, b, c;

    // trivial ctor
    // note that the Vector3 ctor will zero the vector elements
    Matrix3<T>() {}

    // setting ctor
    Matrix3<T>(const Vector3<T> a0, const Vector3<T> b0, const Vector3<T> c0): a(a0), b(b0), c(c0) {}

    // setting ctor
    Matrix3<T>(const T ax, const T ay, const T az, const T bx, const T by, const T bz, const T cx, const T cy, const T cz): a(ax,ay,az), b(bx,by,bz), c(cx,cy,cz) {}

    // function call operator
    void operator () (const Vector3<T> a0, const Vector3<T> b0, const Vector3<T> c0)
    {
        a = a0;
        b = b0;
        c = c0;
    }

    // test for equality
    bool operator == (const Matrix3<T> &m)
    {
        return (a==m.a && b==m.b && c==m.c);
    }

    // test for inequality
    bool operator != (const Matrix3<T> &m)
    {
        return (a!=m.a || b!=m.b || c!=m.c);
    }

    // negation
    Matrix3<T> operator - (void) const
    {
        return Matrix3<T>(-a,-b,-c);
    }

    // addition
    Matrix3<T> operator + (const Matrix3<T> &m) const
    {
        return Matrix3<T>(a+m.a, b+m.b, c+m.c);
    }

    Matrix3<T> &operator += (const Matrix3<T> &m)
    {
        a += m.a;
        b+=m.b;
        c+=m.c;
        return *this;
    }

    // subtraction
    Matrix3<T> operator - (const Matrix3<T> &m) const
    {
        return Matrix3<T>(a-m.a, b-m.b, c-m.c);
    }

    Matrix3<T> &operator -= (const Matrix3<T> &m)
    {
        a-=m.a;
        b-=m.b;
        c-=m.c;
        return *this;
    }

    // uniform scaling
    Matrix3<T> operator * (const T num) const
    {
        return Matrix3<T>(a*num, b*num, c*num);
    }

    Matrix3<T> &operator *= (const T num)
    {
        a *= num;
        b *= num;
        c *= num;
        return *this;
    }

    Matrix3<T> operator / (const T num) const
    {
        return Matrix3<T>(a/num, b/num, c/num);
    }

    Matrix3<T> &operator /= (const T num)
    {
        a /= num;
        b /= num;
        c /= num;
        return *this;
    }

    // multiplication by a vector
    Vector3<T> operator *(const Vector3<T> &v) const;

    // multiplication of transpose by a vector
    Vector3<T> mul_transpose(const Vector3<T> &v) const;

    // multiplication by another Matrix3<T>
    Matrix3<T> operator *(const Matrix3<T> &m) const;

    Matrix3<T> &operator *=(const Matrix3<T> &m)
    {
        return *this = *this * m;
    }

    // transpose the matrix
    Matrix3<T> transposed(void) const
    {
        return Matrix3<T>(Vector3<T>(a.x, b.x, c.x),
                          Vector3<T>(a.y, b.y, c.y),
                          Vector3<T>(a.z, b.z, c.z));
    }

    Matrix3<T> transpose(void)
    {
        return *this = transposed();
    }

    // zero the matrix
    void zero(void)
    {
        a.x = a.y = a.z = 0;
        b.x = b.y = b.z = 0;
        c.x = c.y = c.z = 0;
    }

    // setup the identity matrix
    void identity(void)
    {
        a.x = b.y = c.z = 1;
        a.y = a.z = 0;
        b.x = b.z = 0;
        c.x = c.y = 0;
    }

    // check if any elements are NAN
    bool is_nan(void)
    {
        return a.is_nan() || b.is_nan() || c.is_nan();
    }

    // fill in the matrix with a standard rotation
    //void rotation(enum Rotation rotation);

    // create a rotation matrix from Euler angles
    void from_euler(float roll, float pitch, float yaw);

    // create eulers from a rotation matrix
    void to_euler(float *roll, float *pitch, float *yaw);

    // apply an additional rotation from a body frame gyro vector
    // to a rotation matrix.
    void rotate(const Vector3<T> &g);
};

// create a rotation matrix given some euler angles
// this is based on http://gentlenav.googlecode.com/files/EulerAngles.pdf
template <typename T>
void Matrix3<T>::from_euler(float roll, float pitch, float yaw)
{
    float cp = cos(pitch);
    float sp = sin(pitch);
    float sr = sin(roll);
    float cr = cos(roll);
    float sy = sin(yaw);
    float cy = cos(yaw);

    a.x = cp * cy;
    a.y = (sr * sp * cy) - (cr * sy);
    a.z = (cr * sp * cy) + (sr * sy);
    b.x = cp * sy;
    b.y = (sr * sp * sy) + (cr * cy);
    b.z = (cr * sp * sy) - (sr * cy);
    c.x = -sp;
    c.y = sr * cp;
    c.z = cr * cp;
}

// calculate euler angles from a rotation matrix
// this is based on http://gentlenav.googlecode.com/files/EulerAngles.pdf
template <typename T>
void Matrix3<T>::to_euler(float *roll, float *pitch, float *yaw)
{
    if (pitch != NULL)
    {
        *pitch = -safe_asin(c.x);
    }
    if (roll != NULL)
    {
        *roll = atan2(c.y, c.z);
    }
    if (yaw != NULL)
    {
        *yaw = atan2(b.x, a.x);
    }
}

// apply an additional rotation from a body frame gyro vector
// to a rotation matrix.
template <typename T>
void Matrix3<T>::rotate(const Vector3<T> &g)
{
    Matrix3<T> temp_matrix;
    temp_matrix.a.x = a.y * g.z - a.z * g.y;
    temp_matrix.a.y = a.z * g.x - a.x * g.z;
    temp_matrix.a.z = a.x * g.y - a.y * g.x;
    temp_matrix.b.x = b.y * g.z - b.z * g.y;
    temp_matrix.b.y = b.z * g.x - b.x * g.z;
    temp_matrix.b.z = b.x * g.y - b.y * g.x;
    temp_matrix.c.x = c.y * g.z - c.z * g.y;
    temp_matrix.c.y = c.z * g.x - c.x * g.z;
    temp_matrix.c.z = c.x * g.y - c.y * g.x;

    (*this) += temp_matrix;
}


// multiplication by a vector
template <typename T>
Vector3<T> Matrix3<T>::operator *(const Vector3<T> &v) const
{
    return Vector3<T>(a.x * v.x + a.y * v.y + a.z * v.z,
                      b.x * v.x + b.y * v.y + b.z * v.z,
                      c.x * v.x + c.y * v.y + c.z * v.z);
}

// multiplication of transpose by a vector
template <typename T>
Vector3<T> Matrix3<T>::mul_transpose(const Vector3<T> &v) const
{
    return Vector3<T>(a.x * v.x + b.x * v.y + c.x * v.z,
                      a.y * v.x + b.y * v.y + c.y * v.z,
                      a.z * v.x + b.z * v.y + c.z * v.z);
}

// multiplication by another Matrix3<T>
template <typename T>
Matrix3<T> Matrix3<T>::operator *(const Matrix3<T> &m) const
{
    Matrix3<T> temp (Vector3<T>(a.x * m.a.x + a.y * m.b.x + a.z * m.c.x,
                                a.x * m.a.y + a.y * m.b.y + a.z * m.c.y,
                                a.x * m.a.z + a.y * m.b.z + a.z * m.c.z),
                     Vector3<T>(b.x * m.a.x + b.y * m.b.x + b.z * m.c.x,
                                b.x * m.a.y + b.y * m.b.y + b.z * m.c.y,
                                b.x * m.a.z + b.y * m.b.z + b.z * m.c.z),
                     Vector3<T>(c.x * m.a.x + c.y * m.b.x + c.z * m.c.x,
                                c.x * m.a.y + c.y * m.b.y + c.z * m.c.y,
                                c.x * m.a.z + c.y * m.b.z + c.z * m.c.z));
    return temp;
}

typedef Matrix3<float> Matrix3f;

#endif
