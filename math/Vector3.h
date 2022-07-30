#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
  public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

  public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成
	  
	float GetLength() const;
	void Normalized();
	Vector3 GetNormalized() const;
	Vector3 Cross(const Vector3& other) const;
	float Dot(const Vector3& other) const;

	//オバロ演算子
	Vector3 operator+();
	Vector3& operator+=(const Vector3& other);
	Vector3 operator-();
	Vector3& operator-=(const Vector3& other);
	Vector3 operator*(const float& other);
	Vector3& operator*=(const float& other);
	Vector3 operator/(const float& other);
	Vector3& operator/=(const float& other);
};

//二項演算子オーバーロード
const Vector3 operator +(const Vector3& v1, const Vector3& v2);
const Vector3 operator -(const Vector3& v1, const Vector3& v2);
const Vector3 operator *(const Vector3& v, float s);
const Vector3 operator *(float s, const Vector3& v);
const Vector3 operator /(const Vector3& v, float s);