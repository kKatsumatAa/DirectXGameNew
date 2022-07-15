#pragma once
#include"WorldTransform.h"
#include"Model.h"
//#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"

///<summary>
///敵
///</summary>
class Enemy
{
	//変数
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//行動フェーズ
	static enum class Phase
	{
		Approach,//接近する
		Leave,   //離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;
public:


	//関数
private:
	//メンバ関数ポインタ(メンバ関数のポインタを入れる)
	void (Enemy::* pFunc)();
	//メンバ関数ポインタのテーブル
	static void (Enemy::* spFuncTable[])();
public:
	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Approach();
	void Leave();
	void Draw(const ViewProjection& view);
};



const Vector3 approachSpeed = { 0,0,-1 };
const Vector3 leaveSpeed = { 0,0,1 };