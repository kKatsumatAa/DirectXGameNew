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

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};