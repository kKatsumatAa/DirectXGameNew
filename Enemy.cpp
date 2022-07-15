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
	////メンバ関数ポインタに入っている関数を呼び出す
	//(this->*pFunc)();
	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//switch (phase_)
	//{
	//case Phase::Approach:
	///*default:*/
	//	//移動（ベクトルを加算）
	//	worldTransform_.translation_ += approachSpeed;
	//	//既定の位置に達したら離脱
	//	if (worldTransform_.translation_.z < 0.0f)
	//	{
	//		phase_ = Phase::Leave;
	//	}
	//	break;

	//case Phase::Leave:
	//	//移動
	//	worldTransform_.translation_ += leaveSpeed;
	//	break;
	//}
	//// worldTransform_.translation_ += {0, 0, -1.0f};
	UpdateWorldMatrix4(worldTransform_);
}

void Enemy::Approach()
{
	//移動（ベクトルを加算）
	worldTransform_.translation_ += approachSpeed;
	//既定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	//移動
	worldTransform_.translation_ += leaveSpeed;
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
}
