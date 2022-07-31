#pragma once
#include"Vector3.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
private:
	float radius_ = 2.0f;

public:
	float GetRadius();
	void SetRadius(const float& radius);

	//衝突時に呼ばれる
	virtual void OnCollision() = 0;
	//world座標をゲット
	virtual Vector3 GetWorldPos() = 0;
};

