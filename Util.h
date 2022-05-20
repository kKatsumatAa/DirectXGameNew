#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "WorldTransform.h"


//�s��Z�b�g�n
//void Vector4xMatrix4(Vector4& v, const Matrix4& m4);
void Matrix4xMatrix4(Matrix4& m4, const Matrix4& other);
//void NormVector4(Vector4& v/*, const M4& m4*/);
void TransposeMatrix4(Matrix4& m4);

//�ϊ��s��Z�b�g�n
void SetScaleMatrix(Matrix4& m4, const Vector3& v3);
void SetRotationMatrix(Matrix4& m4, const float& angle, const char& axis);
void SetTranslationMatrix(Matrix4& m4, const Vector3& v3);
/// <summary>
/// ���������e���g�ł������ł���
/// </summary>
/// <param name="world"></param>
/// <param name="worldParent"></param>
void UpdateWorldMatrix4(WorldTransform& world);

//���W�A���n
const float pi = 3.141592f;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
