#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include "PlayerBullet.h"

/// <summary>
/// ���L����
/// </summary>
class Player
{
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	std::list< std::unique_ptr<PlayerBullet>> bullets_;

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
	void Attack();
};