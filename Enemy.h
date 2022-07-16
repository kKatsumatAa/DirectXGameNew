#pragma once
#include"WorldTransform.h"
#include"Model.h"
//#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"

class Enemy;

class EnemyState
{
protected:
	Enemy* enemy;

public:
	void SetEnemy(Enemy* enemy);
	virtual void Update() = 0;
};

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
	EnemyState* state;
public:


	//関数
private:

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	~Enemy();

	void Update();
	void ChangeState(EnemyState* state);
	void Draw(const ViewProjection& view);

	Vector3 GetTrans();
	void MoveTrans(const Vector3& vec);
};

class EnemyStateApproach :public EnemyState
{
public:
	void Update();
};

class EnemyStateLeave :public EnemyState
{
public:
	void Update();
};


const Vector3 approachSpeed = { 0,0,-1 };
const Vector3 leaveSpeed = { -1,1,0 };