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

	//�ڋ߃t�F�[�Y������
	InitializeApproach();
}

Enemy::~Enemy()
{
	delete state;
}

void Enemy::InitializeApproach()
{
	//���˃^�C�}�[��������
	shotTime = shotCool;
}

void Enemy::Update()
{
	//�e������
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		}
	);

	state->Update();

	UpdateWorldMatrix4(worldTransform_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	shotTime++;
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	Vector3xMatrix4(velocity, worldTransform_.matWorld_, false);

	//���𐶐��A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//����o�^
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ChangeState(EnemyState* state)
{
	//delete this->state;
	this->state = state;
	state->SetEnemy(this);
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
	//�ړ��i�x�N�g�������Z�j
	enemy->MoveTrans(approachSpeed);
	//����̈ʒu�ɒB�����痣�E
	if (enemy->GetTrans().z < 0.0f)
	{
		enemy->ChangeState(new EnemyStateLeave);
	}

	if (enemy->shotTime >= enemy->shotCool)
	{
		enemy->shotTime = 0;
		enemy->Fire();
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
