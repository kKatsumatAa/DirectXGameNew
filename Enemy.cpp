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
}

Enemy::~Enemy()
{
	delete state;
}

void Enemy::Update()
{
	state->Update();

	UpdateWorldMatrix4(worldTransform_);
}

void Enemy::ChangeState(EnemyState* state)
{
	delete this->state;
	this->state = state;
	state->SetEnemy(this);
}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
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
	//�ړ��i�x�N�g�������Z�j
	enemy->MoveTrans(approachSpeed);
	//����̈ʒu�ɒB�����痣�E
	if (enemy->GetTrans().z < 0.0f)
	{
		enemy->ChangeState(new EnemyStateLeave);
	}
}

//----------------------------------------------
void EnemyStateLeave::Update()
{
	//�ړ�
	enemy->MoveTrans(leaveSpeed);
}

//----------------------------------------------
void EnemyState::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;
}
