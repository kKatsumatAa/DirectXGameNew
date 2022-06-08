#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "Util.h"

#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	/*delete player_;*/
}

void GameScene::Initialize() {
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> rotDist(0.0f, pi * 2);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	//float value = dist(engine);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("kasuga.png");


	//モデル初期化
	model_ = Model::Create();

	viewProjection_.eye = { 0, 20, -30 };
	viewProjection_.target = { 0, 0, 0 };

	//ビュープロジェクション初期化
	viewProjection_.Initialize();

	for (WorldTransform& i : worldTransforms_)
	{
		i.Initialize();
		i.rotation_ = { rotDist(engine),rotDist(engine) ,rotDist(engine) };
		i.translation_ = { posDist(engine),posDist(engine) ,posDist(engine) };
		UpdateWorldMatrix4(i);
	}

	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
}

void GameScene::Update() {
	
	if (input_->TriggerKey(DIK_Q))
	{
		if (mode) mode = false;
		else      mode = true;
	}

	if (!mode)
	{
		//終点
		Vector3 endPoint = { 0, 0, 0 };
		//正面ベクトル
		Vector3 vecFront = { 0, 0, 0 };
		//大きさ
		const float frontLength = 10.0f;
		//正規化したベクトル
		Vector3 normalFLength = { 0, 0, 0 };

		const float frontSpeed = 0.5f;

		if (input_->PushKey(DIK_A)) {
			worldTransform_.rotation_.y -= frontSpeed / 10;
		}if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += frontSpeed / 10;
		}

		//正面ベクトルの終点を、オブジェクトの回転に合わせて一緒に回転
		endPoint = {
		  worldTransform_.translation_.x + sinf(worldTransform_.rotation_.y) * frontLength,
		  worldTransform_.translation_.y,
		  worldTransform_.translation_.z + cosf(worldTransform_.rotation_.y) * frontLength };

		//正面ベクトルの成分を計算
		vecFront = {
		  endPoint.x - worldTransform_.translation_.x,
		  endPoint.y - worldTransform_.translation_.y,
		  endPoint.z - worldTransform_.translation_.z };

		//成分を正規化
		normalFLength = { vecFront.x / frontLength, vecFront.y / frontLength, vecFront.z / frontLength };


		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_ = {
			  worldTransform_.translation_.x + normalFLength.x * 0.1f,
			  worldTransform_.translation_.y + normalFLength.y * 0.1f,
			  worldTransform_.translation_.z + normalFLength.z * 0.1f };
		}if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_ = {
			  worldTransform_.translation_.x - normalFLength.x * 0.1f,
			  worldTransform_.translation_.y - normalFLength.y * 0.1f,
			  worldTransform_.translation_.z - normalFLength.z * 0.1f };
		}
	}
	else
	{
		if (input_->PushKey(DIK_RIGHT))cameraTargetAngle += AngletoRadi(2);
		if (input_->PushKey(DIK_LEFT))cameraTargetAngle -= AngletoRadi(2);
		viewProjection_.eye.x = cosf(cameraTargetAngle) * cameraDistans;
		viewProjection_.eye.z = sinf(cameraTargetAngle) * cameraDistans;

		Vector3 v;
		Vector3 v2={0,1,0};

		v = viewProjection_.target - viewProjection_.eye;
		v.y = 0;
		v.Normalized();

		v2 = v2.Cross(v);

		if (input_->PushKey(DIK_W))worldTransform_.translation_ += v * 0.5f;
		if (input_->PushKey(DIK_S))worldTransform_.translation_ -= v * 0.5f;
		if (input_->PushKey(DIK_D))worldTransform_.translation_ += v2 * 0.5f;
		if (input_->PushKey(DIK_A))worldTransform_.translation_ -= v2 * 0.5f;
	}

	viewProjection_.UpdateMatrix();
	//行列の再計算
	UpdateWorldMatrix4(worldTransform_);

	debugText_->SetPos(50, 130);
	debugText_->Printf("cameraWASD mode:%d\n", mode);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	for (WorldTransform& i : worldTransforms_)
	{
		model_->Draw(i, viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/*sprite_->Draw();*/

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
