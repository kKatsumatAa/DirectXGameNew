#include "Vec3.h"

float Vec3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

void Vec3::Normalized()
{
    *this /= GetLength();
}

const Vec3 Vec3::GetNormalized()
{
    Vec3 v;
    v = *this / GetLength();

    return v;
}

//float Vec3::Cross(const Vec3& other)const
//{
//    return x * other.y - y * other.x;
//}

float Vec3::Dot(const Vec3& other)const
{
    return x * other.x + y * other.y + z * other.z;
}
//--------------------------------------------------------------


Vec3 Vec3::operator+(const Vec3& other)
{
    Vec3 v = *this;
    v.x += other.x;
    v.y += other.y;
    v.z += other.z;

    return Vec3(v);
}

Vec3& Vec3::operator+=(const Vec3& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}

Vec3 Vec3::operator-(const Vec3& other)
{
    Vec3 v = *this;
    v.x -= other.x;
    v.y -= other.y;
    v.z -= other.z;

    return Vec3(v);
}

Vec3& Vec3::operator-=(const Vec3& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;

    return *this;
}

Vec3 Vec3::operator*(const float& other)
{
    Vec3 v = *this;
    v.x *= other;
    v.y *= other;
    v.z *= other;

    return Vec3(v);
}

Vec3& Vec3::operator*=(const float& other)
{
    this->x *= other;
    this->y *= other;
    this->z *= other;

    return *this;
}

Vec3 Vec3::operator/(const float& other)
{
    Vec3 v = *this;
    v.x /= other;
    v.y /= other;
    v.z /= other;

    return Vec3(v);
}

Vec3& Vec3::operator/=(const float& other)
{
    this->x /= other;
    this->y /= other;
    this->z /= other;

    return *this;
}

Vec3& Vec3::operator=(const Vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

    return *this;
}
