#pragma once
#include"WorldTransform.h"
#include"Model.h"
//#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include"EnemyBullet.h"

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
///�G
///</summary>
class Enemy
{
	//�ϐ�
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	EnemyState* state;
	//�e
	std::list< std::unique_ptr<EnemyBullet>> bullets_;
	
	
public:
	int shotTime = 0;
	static const int shotCool = 60;

	//�֐�
private:

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	~Enemy();
	void InitializeApproach();

	void Update();
	void Fire();
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


const Vector3 approachSpeed = { 0,0,-0.2 };
const Vector3 leaveSpeed = { -0.2,0.2,0 };