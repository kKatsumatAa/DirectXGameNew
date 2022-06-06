//#pragma once
//#include"WorldTransform.h"
//#include"Model.h"
//#include"Input.h"
//#include"DebugText.h"
//#include"Util.h"
//#include "Assert.h"
//
///// <summary>
///// 自キャラ
///// </summary>
//class Player
//{
//private:
//	//ワールド変換データ
//	WorldTransform worldTransform_;
//	//モデル
//	Model* model_ = nullptr;
//	//テクスチャハンドル
//	uint32_t textureHandle_ = 0u;
//	Input* input_ = nullptr;
//	DebugText* debugText_ = nullptr;
//
//public:
//	void Initialize(Model* model, const uint32_t textureHandle);
//	void Update();
//	void Draw(const ViewProjection& view);
//};