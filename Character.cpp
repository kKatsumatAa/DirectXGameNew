#include "Character.h"


//void Character::Initialize(Model* model, const uint32_t textureHandle)
//{
//	//assert(model);
//
//	//model_ = model;
//	//textureHandle_ = textureHandle;
//
//	////シングルトンインスタンスを取得
//	//input_ = Input::GetInstance();
//	//debugText_ = DebugText::GetInstance();
//
//	//worldTransform_.Initialize();
//}

void Character::Update()
{
	if (worldTransform_.scale_.x > 1.0f)
	{
		worldTransform_.scale_ -= {0.05f, 0.05f, 0.05f};
	}
	
	if (worldTransform_.rotation_.z > 0.0f)
	{
		worldTransform_.rotation_ -= {0, 0, 0.02f};
	}
	else if (worldTransform_.rotation_.z < 0.0f)
	{
		worldTransform_.rotation_ += {0, 0, 0.02f};
	}

	attackTime++;

	if (attackTime >= attackCool)
	{
		Attack();
		attackTime = 0;
	}

	if (HP <= 0) isDead = true;

	UpdateWorldMatrix4(worldTransform_);
}

void Character::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
}

void Character::Attack()
{
	isAttack = true;
}