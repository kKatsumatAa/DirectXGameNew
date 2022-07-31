#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "WorldTransform.h"


//行列セット系
void Vector4xMatrix4(Vector4& v, const Matrix4& m4);
void Vector3xMatrix4(Vector3& v, const Matrix4& m4 ,const bool w);
void Matrix4xMatrix4(Matrix4& m4, const Matrix4& other);
//void NormVector4(Vector4& v/*, const M4& m4*/);
void TransposeMatrix4(Matrix4& m4);

//変換行列セット系
void SetScaleMatrix(Matrix4& m4, const Vector3& v3);
void SetRotationMatrix(Matrix4& m4, const float& angle, const char& axis);
void SetTranslationMatrix(Matrix4& m4, const Vector3& v3);
/// <summary>
/// 第一引数が親自身でも処理できる
/// </summary>
/// <param name="world"></param>
/// <param name="worldParent"></param>
void UpdateWorldMatrix4(WorldTransform& world);

//ラジアン系
const float pi = 3.141592f;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
float GetRadianVector3(const Vector3& v1, const Vector3& v2);

//線形補完
Vector3 LerpVector3(const Vector3& v1, const Vector3& v2, float t);
Vector3 SlerpVector3(const Vector3& v1, const Vector3& v2, float t);

//衝突
bool CollisionCircleCircle(const Vector3& pos1, const float& r1,
	const Vector3& pos2, const float& r2);