#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	velocity_ = velocity;
	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("scope.png");

	worldTransform.Initialize();
	worldTransform.translation_ = position;
}

void EnemyBullet::Update()
{
	worldTransform.translation_ += velocity_;

	UpdateWorldMatrix4(worldTransform);

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}
