#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);

	velocity_ = velocity;
	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	worldTransform.Initialize();
	worldTransform.translation_ = position;

	//zに伸びた形状
	worldTransform.scale_.x = 0.5f;
	worldTransform.scale_.y = 0.5f;
	worldTransform.scale_.z = 3.0f;

	//yじくまわり
	worldTransform.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//y軸周りに-θ回す回転行列
	Matrix4 m4;
	SetRotationMatrix(m4, -worldTransform.rotation_.y, 'y');
	//velocityに回転行列を掛け算してvelocityZを求める（資料参照）
	Vector3 v = velocity_;
	Vector3xMatrix4(v, m4, false);//ここで上下の角度を反転
	//x軸周り角度
	worldTransform.rotation_.x = std::atan2(-v.y, v.z);

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void EnemyBullet::Update()
{
//ホーミング
	Vector3 toPlayer = player_->GetWorldPos() - worldTransform.translation_;

	toPlayer.Normalized();
	velocity_.Normalized();

	velocity_ = SlerpVector3(velocity_, toPlayer, 0.1f);

	worldTransform.translation_ += velocity_;

//角度をvectorにそろえる
	//yじくまわり
	worldTransform.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//y軸周りに-θ回す回転行列
	Matrix4 m4;
	SetRotationMatrix(m4, -worldTransform.rotation_.y, 'y');
	//velocityに回転行列を掛け算してvelocityZを求める（資料参照）
	Vector3 v = velocity_;
	Vector3xMatrix4(v, m4, false);//ここで上下の角度を反転
	//x軸周り角度
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
