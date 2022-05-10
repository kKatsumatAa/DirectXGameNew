#pragma once
#include "DirectXCommon.h"

class Vec3
{
private:
	

public:
	float x;
	float y;
	float z;
	float w;

	Vec3()
		:x(0), y(0), z(0), w(1.0f)
	{}
	Vec3(const float x, const float y, const float z, const float w)
		:x(x), y(y), z(z), w(w)
	{}

	//•Ö—˜ŠÖ”
	float GetLength() const;
	void Normalized();
	const Vec3 GetNormalized();
	//float Cross(const Vec3& other) const;
	float Dot(const Vec3& other) const;

	//ƒIƒoƒ‰‰Zq
	Vec3 operator+(const Vec3& other);
	Vec3& operator+=(const Vec3& other);
	Vec3 operator-(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3 operator*(const float& other);
	Vec3& operator*=(const float& other);
	Vec3 operator/(const float& other);
	Vec3& operator/=(const float& other);
};
