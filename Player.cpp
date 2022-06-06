#include "Player.h"


//void Player::Initialize(Model* model, const uint32_t textureHandle)
//{
//	assert(model);
//
//	model_ = model;
//	textureHandle_ = textureHandle;
//
//	//シングルトンインスタンスを取得
//	input_ = Input::GetInstance();
//	debugText_ = DebugText::GetInstance();
//
//	worldTransform_.Initialize();
//}
//
//void Player::Update()
//{
//	const float xLimit = 10.f;
//	const float yLimit = 5.f;
//
//	Vector3 move = { 0,0,0 };
//	move.x = input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT);
//	move.y = input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN);
//
//	worldTransform_.translation_.x += move.x;
//	worldTransform_.translation_.y += move.y;
//	
//	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -xLimit);
//	worldTransform_.translation_.x = min(worldTransform_.translation_.x, xLimit);
//	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -yLimit);
//	worldTransform_.translation_.y = min(worldTransform_.translation_.y, yLimit);
//
//	UpdateWorldMatrix4(worldTransform_);
//
//	debugText_->SetPos(50, 110);
//	debugText_->Printf("playerPos:(%f,%f,%f)",
//		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
//}
//
//void Player::Draw(const ViewProjection& view)
//{
//	model_->Draw(worldTransform_, view, textureHandle_);
//}
