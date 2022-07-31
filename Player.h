#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include "PlayerBullet.h"
#include "Collider.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider
{
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	std::list< std::unique_ptr<PlayerBullet>> bullets_;
	static const int shotCool = 10;
	 int shotTime = 0;

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
	void Attack();

	Vector3 GetWorldPos() override;

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision() override;

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets()
	{
		return bullets_;
	}
};