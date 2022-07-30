#include "Vector3.h"
#include<DirectXMath.h>

float Vector3::GetLength() const
{
    return sqrtf(x * x + y * y + z * z);
}

void Vector3::Normalized()
{
    *this /= GetLength();
}

Vector3 Vector3::GetNormalized() const
{
    Vector3 v;
    v = *this / GetLength();

    return v;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
    Vector3 v;

    v.x = y * other.z - z * other.y;
    v.y = z * other.x - x * other.z;
    v.z = x * other.y - y * other.x;

    return v;
}

float Vector3::Dot(const Vector3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

//--------------------------------------------------------

Vector3 Vector3::operator+()
{
    return *this;
}

//Vector3& Vector3::operator+=(const Vector3& other)
//{
//    this->x += other.x;
//    this->y += other.y;
//    this->z += other.z;
//
//    return *this;
//}

Vector3 Vector3::operator-()
{
    return { -x,-y,-z };
}

//Vector3& Vector3::operator-=(const Vector3& other)
//{
//    this->x -= other.x;
//    this->y -= other.y;
//    this->z -= other.z;
//
//    return *this;
//}

Vector3 Vector3::operator*(const float& other)
{
    Vector3 v = *this;
    v.x *= other;
    v.y *= other;
    v.z *= other;

    return Vector3(v);
}

Vector3& Vector3::operator*=(const float& other)
{
    this->x *= other;
    this->y *= other;
    this->z *= other;

    return *this;
}

Vector3 Vector3::operator/(const float& other)
{
    Vector3 v = *this;
    v.x /= other;
    v.y /= other;
    v.z /= other;

    return Vector3(v);
}

Vector3& Vector3::operator/=(const float& other)
{
    this->x /= other;
    this->y /= other;
    this->z /= other;

    return *this;
}


//-------------------------------------

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    Vector3 temp(v1);
    return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    Vector3 temp(v1);
    return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
    Vector3 temp(v);
    return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
    Vector3 temp(v);
    return temp *= s;
}

const Vector3 operator/(const Vector3& v, float s)
{
    Vector3 temp(v);
    return temp /= s;
}
