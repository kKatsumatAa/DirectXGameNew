#pragma once
#include"Vector3.h"

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider
{
private:
	float radius_ = 2.0f;

public:
	float GetRadius();
	void SetRadius(const float& radius);

	//�Փˎ��ɌĂ΂��
	virtual void OnCollision() = 0;
	//world���W���Q�b�g
	virtual Vector3 GetWorldPos() = 0;
};

