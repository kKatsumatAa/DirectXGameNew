#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
private:
	
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

public:
	//ワールド変換データ
	WorldTransform worldTransform_;

	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
};