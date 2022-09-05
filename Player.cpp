#include "Player.h"

void Player::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.translation_ = { -10.0f,0,30.0f };
	UpdateWorldMatrix4(worldTransform_);

	attribute = PLAYER;

	power = 1;
	HP = 10;

	attackCool = 100;
}
