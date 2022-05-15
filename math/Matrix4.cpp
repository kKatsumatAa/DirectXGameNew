#include "Matrix4.h"

Matrix4 Matrix4::operator*(const Matrix4& m2) const
{
	Matrix4 m4;

	for (int ic = 0; ic < 4; ic++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m4.m[ic][i] += m[ic][j] * m2.m[j][i];
			}
		}
	}

	return m4;
}
