#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"

enum 
{
	PLAYER,
	ENEMY
};

class Character
{
private:

	
protected:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	int attackCool = 240;
	int attackTime = 0;

	bool isAttack = false;

	int power = 1;

	int HP = 10;

	int attribute;

	bool isDead = false;

	float angle = 0;


public:
	virtual void Initialize(Model* model, const uint32_t textureHandle) = 0;
	void Update();
	void Draw(const ViewProjection& view);

	void Attack();

	bool GetIsAttack() { return isAttack; }
	void SetIsAttack(bool isAttack) { this->isAttack = isAttack; }

	int GetPower() { return power; }

	void Damage(int power) { HP -= power; }

	int GetAttribute() { return attribute; }

	bool GetIsDead() { return isDead; }

	void AddScale(const Vector3& scale) { worldTransform_.scale_ += scale; }

	void AddAngle(const Vector3& angle){ worldTransform_.rotation_ += angle; }

};