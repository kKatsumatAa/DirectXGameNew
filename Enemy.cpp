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
	worldTransform_.translation_ += {0, 0, -1.0f};
	UpdateWorldMatrix4(worldTransform_);
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
}
