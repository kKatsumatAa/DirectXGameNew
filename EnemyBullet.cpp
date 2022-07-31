#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	velocity_ = velocity;
	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");

	worldTransform.Initialize();
	worldTransform.translation_ = position;

	//z�ɐL�т��`��
	worldTransform.scale_.x = 0.5f;
	worldTransform.scale_.y = 0.5f;
	worldTransform.scale_.z = 3.0f;

	//y�����܂��
	worldTransform.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//y�������-�Ɖ񂷉�]�s��
	Matrix4 m4;
	SetRotationMatrix(m4, -worldTransform.rotation_.y, 'y');
	//velocity�ɉ�]�s����|���Z����velocityZ�����߂�i�����Q�Ɓj
	Vector3 v = velocity_;
	Vector3xMatrix4(v, m4, false);//�����ŏ㉺�̊p�x�𔽓]
	//x������p�x
	worldTransform.rotation_.x = std::atan2(-v.y, v.z);

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void EnemyBullet::Update()
{
//�z�[�~���O
	Vector3 toPlayer = player_->GetWorldPos() - worldTransform.translation_;

	toPlayer.Normalized();
	velocity_.Normalized();

	velocity_ = SlerpVector3(velocity_, toPlayer, 0.1f);

	worldTransform.translation_ += velocity_;

//�p�x��vector�ɂ��낦��
	//y�����܂��
	worldTransform.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//y�������-�Ɖ񂷉�]�s��
	Matrix4 m4;
	SetRotationMatrix(m4, -worldTransform.rotation_.y, 'y');
	//velocity�ɉ�]�s����|���Z����velocityZ�����߂�i�����Q�Ɓj
	Vector3 v = velocity_;
	Vector3xMatrix4(v, m4, false);//�����ŏ㉺�̊p�x�𔽓]
	//x������p�x
	worldTransform.rotation_.x = std::atan2(-v.y, v.z);

	UpdateWorldMatrix4(worldTransform);

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPos()
{
	return Vector3(worldTransform.translation_);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}
