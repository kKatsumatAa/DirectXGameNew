#pragma once

class M4
{
private:
	

public:
	//çsóÒ
	float m[4][4];

	//ä÷êî
	M4(const float (*other)[4]);
	void Normalize();

};

const float normal[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
};

const float papa[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,4,0},
		{0,0,0,5}
};

