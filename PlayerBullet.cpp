#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("kamata.ico");

	worldTransform.Initialize();
	worldTransform.translation_ = position;
}

void PlayerBullet::Update()
{
	UpdateWorldMatrix4(worldTransform);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}
