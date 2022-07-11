#include "Enemy.h"

void Enemy::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,100 };
}

void Enemy::Update()
{
	

	switch (phase_)
	{
	case Phase::Approach:
	/*default:*/
		//移動（ベクトルを加算）
		worldTransform_.translation_ += approachSpeed;
		//既定の位置に達したら離脱
		if (worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		//移動
		worldTransform_.translation_ += leaveSpeed;
		break;
	}
	// worldTransform_.translation_ += {0, 0, -1.0f};
	UpdateWorldMatrix4(worldTransform_);
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
}
