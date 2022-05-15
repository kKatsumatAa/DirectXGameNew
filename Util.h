#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <DirectXMath.h>

//行列セット系
//void Vector4xMatrix4(Vector4& v, const Matrix4& m4);
void Matrix4xMatrix4(Matrix4& m4, const Matrix4& other);
//void NormVector4(Vector4& v/*, const M4& m4*/);
void TransposeMatrix4(Matrix4& m4);

//変換行列セット系
void SetScaleMatrix(Matrix4& m4, const Vector3& v3);
void SetRotationMatrix(Matrix4& m4, const float& angle, const char& axis);
void SetTranslationMatrix(Matrix4& m4, const Vector3& v3);

//ラジアン系
const float pi = 3.141592;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
