#pragma once
#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include <cassert>
#include "Util.h"
#include "Collider.h"

class PlayerBullet : public Collider
{
private:
	WorldTransform worldTransform;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//����
	static const int32_t lifeTime = 60 * 5;
	int32_t deathTimer_ = lifeTime;
	bool isDead_ = false;
public:
	Vector3 velocity_;

public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPos() override;

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision() override;
};