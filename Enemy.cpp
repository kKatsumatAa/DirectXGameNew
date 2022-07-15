#include "Enemy.h"

void Enemy::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,100 };

	//pFunc = &Enemy::Approach;
	phase_ = Phase::Approach;
}

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,//0
	&Enemy::Leave    //1
};

void Enemy::Update()
{
	////�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	//(this->*pFunc)();
	//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//switch (phase_)
	//{
	//case Phase::Approach:
	///*default:*/
	//	//�ړ��i�x�N�g�������Z�j
	//	worldTransform_.translation_ += approachSpeed;
	//	//����̈ʒu�ɒB�����痣�E
	//	if (worldTransform_.translation_.z < 0.0f)
	//	{
	//		phase_ = Phase::Leave;
	//	}
	//	break;

	//case Phase::Leave:
	//	//�ړ�
	//	worldTransform_.translation_ += leaveSpeed;
	//	break;
	//}
	//// worldTransform_.translation_ += {0, 0, -1.0f};
	UpdateWorldMatrix4(worldTransform_);
}

void Enemy::Approach()
{
	//�ړ��i�x�N�g�������Z�j
	worldTransform_.translation_ += approachSpeed;
	//����̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	//�ړ�
	worldTransform_.translation_ += leaveSpeed;
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
}
