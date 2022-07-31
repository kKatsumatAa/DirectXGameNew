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

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update()
{
	//弾を消す
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	const float xLimit = 35;
	const float yLimit = 19;

	//回転
	worldTransform_.rotation_.y += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A))*0.05f;

	//平行移動
	Vector3 move = { 0,0,0 };
	move.x = input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT);
	move.y = input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -xLimit);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, xLimit);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -yLimit);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, yLimit);

	UpdateWorldMatrix4(worldTransform_);

	debugText_->SetPos(50, 110);
	debugText_->Printf("playerPos:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	Attack();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	shotTime++;
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(view);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE) && shotTime >= shotCool)
	{
		shotTime = 0;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		Vector3xMatrix4(velocity, worldTransform_.matWorld_,false);

		//球を生成、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//球を登録
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPos()
{
	Vector3 pos;
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];

	return Vector3(pos);
}

void Player::OnCollision()
{
	//なにもしない
}
