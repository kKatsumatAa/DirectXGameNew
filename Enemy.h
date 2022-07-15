#pragma once
#include"WorldTransform.h"
#include"Model.h"
//#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"

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
	//�s���t�F�[�Y
	static enum class Phase
	{
		Approach,//�ڋ߂���
		Leave,   //���E����
	};
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
public:


	//�֐�
private:
	//�����o�֐��|�C���^(�����o�֐��̃|�C���^������)
	void (Enemy::* pFunc)();
	//�����o�֐��|�C���^�̃e�[�u��
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