#include "Util.h"

void CrossVec3M4(Vec3& v, const M4& m4)
{
	float v3[2][4] = {
		{ v.x,v.y,v.z,v.w },
		{0,0,0,0}
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			v3[1][i] += v3[0][j] * m4.m[j][i];
		}
	}

	v = { v3[1][0],v3[1][1] ,v3[1][2] ,v3[1][3] };
}

void NormVec3(Vec3& v, const M4& m4)
{
}
