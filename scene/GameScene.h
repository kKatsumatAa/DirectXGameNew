#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Util.h"
#include "Player.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollision();


	//
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;

	float num = 0.0f;
	

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle3_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	float viewAngle = AngletoRadi(90.f);

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCamera = false;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
