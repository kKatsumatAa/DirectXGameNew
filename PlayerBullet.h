#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include <cassert>
#include "Util.h"

class PlayerBullet
{
private:
	WorldTransform worldTransform;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//Žõ–½
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
};