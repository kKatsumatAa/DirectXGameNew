#pragma once
#include"WorldTransform.h"
#include"Model.h"
//#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include"EnemyBullet.h"
#include"TimedCall.h"
#include"Player.h"
#include"Collider.h"

//自機クラスの前方宣言(インクルードする代わりに)
class Player;

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
class Enemy : public Collider
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
	//弾
	std::list< std::unique_ptr<EnemyBullet>> bullets_;
	static const int shotCool = 60;
	std::list<std::unique_ptr<TimedCall>> timedCalls_;

	//自キャラ
	Player* player_ = nullptr;
public:
	//int shotTime = 0;
	

	//関数
private:
	

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	~Enemy();
	void InitializeApproach();

	void Update();
	void Fire();
	void ChangeState(EnemyState* state);
	/// <summary>
	/// 弾発射、タイマーリセット
	/// </summary>
	/// <param name="view"></param>
	void ShotResetTimer();
	void Draw(const ViewProjection& view);

	Vector3 GetTrans();
	void MoveTrans(const Vector3& vec);

	void RemoveTimeCall();

	void SetPlayer(Player* player) { player_ = player;}

	Vector3 GetWorldPos() override;

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision() override;

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()
	{
		return bullets_;
	}
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


const Vector3 approachSpeed = { 0,0,-0.2 };
const Vector3 leaveSpeed = { -0.2,0.2,0 };