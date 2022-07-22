#include "Enemy.h"

void Enemy::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,5,100 };

	state = new EnemyStateApproach;
	state->SetEnemy(this);

	//接近フェーズ初期化
	InitializeApproach();
}

Enemy::~Enemy()
{
	delete state;
}

void Enemy::InitializeApproach()
{
	//発射タイマーを初期化
	//shotTime = shotCool;

	ShotResetTimer();
}

void Enemy::Update()
{
	//弾を消す
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);
	//timerを消す
	timedCalls_.remove_if([](std::unique_ptr<TimedCall>& time)
		{
			return time->IsFinished();
		}
	);

	state->Update();

	UpdateWorldMatrix4(worldTransform_);

	//弾
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	//timer
	for (std::unique_ptr<TimedCall>& time : timedCalls_)
	{
		time->Update();
	}
}

void Enemy::Fire()
{
	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	Vector3xMatrix4(velocity, worldTransform_.matWorld_, false);

	//球を生成、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//球を登録
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ChangeState(EnemyState* state)
{
	//delete this->state;
	this->state = state;
	state->SetEnemy(this);
}

void Enemy::ShotResetTimer()
{
	//発射
	Fire();

	//発射タイマーリセット
	timedCalls_.push_back(std::make_unique<TimedCall>
		(std::bind(&Enemy::ShotResetTimer, this), shotCool));
	//↑コンストラクタも起動↓
	//TimedCall::TimedCall(std::function<void()> f, uint32_t time)
	//①.shotresetTimerをfunctionに入れる
	//②.TimedCallのインスタンスを作成
	//③.それをリストに追加(push_buck)
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(view);
	}
}

Vector3 Enemy::GetTrans()
{
	return Vector3(worldTransform_.translation_);
}

void Enemy::MoveTrans(const Vector3& vec)
{
	worldTransform_.translation_ += vec;
}


//----------------------------------------------
void EnemyStateApproach::Update()
{
	//移動（ベクトルを加算）
	enemy->MoveTrans(approachSpeed);
	//既定の位置に達したら離脱
	if (enemy->GetTrans().z < 0.0f)
	{
		enemy->ChangeState(new EnemyStateLeave);
	}

	////生成処理は接近の時のみ
	//enemy->shotTime++;
	//if (enemy->shotTime >= enemy->shotCool)
	//{
	//	enemy->shotTime = 0;
	//	enemy->Fire();
	//}
}

//----------------------------------------------
void EnemyStateLeave::Update()
{
	//移動
	enemy->MoveTrans(leaveSpeed);
}

//----------------------------------------------
void EnemyState::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;
}
