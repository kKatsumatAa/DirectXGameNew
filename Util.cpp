#include "Util.h"

//void Vector4xMatrix4(Vector4& v, const Matrix4& m4)
//{
//	float v4[2][4] = {
//		{ v.x,v.y,v.z,v.w },
//		{0,0,0,0}
//	};
//
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			v4[1][i] += v4[0][j] * m4.m[j][i];
//		}
//	}
//
//	v = { v4[1][0],v4[1][1] ,v4[1][2] ,v4[1][3] };
//}

void Matrix4xMatrix4(Matrix4& m4, const Matrix4& other)
{
	Matrix4 m;

	for (int ic = 0; ic < 4; ic++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m.m[ic][i] += m4.m[ic][j] * other.m[j][i];
			}
		}
	}

	m4 = m;
}

//void NormVector4(Vector4& v/*, const M4& m4*/)
//{
//	Vector4xMatrix4(v, normalM);
//}

void TransposeMatrix4(Matrix4& m4)
{
	Matrix4 M = m4;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m4.m[i][j] = M.m[j][i];
		}
	}
}

void SetScaleMatrix(Matrix4& m4, const Vector3& v3)
{
	m4 = {
		v3.x,0,0,0,
		0,v3.y,0,0,
		0,0,v3.z,0,
		0,0,0,1
	};
}

void SetRotationMatrix(Matrix4& m4, const float& angle, const char& axis)
{
	if (axis == 'x' || axis == 'X')
	{
		m4 = {
			1,0,0,0,
			0,cosf(angle),sinf(angle),0,
			0,-sinf(angle),cosf(angle),0,
			0,0,0,1
		};
	}

	if (axis == 'y' || axis == 'Y')
	{
		m4 = {
			cosf(angle),0,-sinf(angle),0,
			0,1,0,0,
			sinf(angle),0,cosf(angle),0,
			0,0,0,1
		};
	}

	if (axis == 'z' || axis == 'Z')
	{
		m4 = {
			cosf(angle),sinf(angle),0,0,
			-sinf(angle),cosf(angle),0,0,
			0,0,1,0,
			0,0,0,1
		};
	}
}

void SetTranslationMatrix(Matrix4& m4, const Vector3& v3)
{
	m4 = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		v3.x,v3.y,v3.z,1
	};
}

//-------------------------------------------------

float AngletoRadi(const float& angle)
{
	return angle * pi / 180;
}

float RaditoAngle(const float& radian)
{
	return radian / pi * 180;
}
