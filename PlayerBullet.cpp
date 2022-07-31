#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	velocity_ = velocity;
	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("kamata.ico");

	worldTransform.Initialize();
	worldTransform.translation_ = position;

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void PlayerBullet::Update()
{
	worldTransform.translation_ += velocity_;

	UpdateWorldMatrix4(worldTransform);

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPos()
{
	return Vector3(worldTransform.translation_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}
