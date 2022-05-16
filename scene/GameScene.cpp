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
	delete debugCamera_;
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

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("kasuga.png");
	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);


	for (WorldTransform& i : worldTransforms_)
	{
		i.Initialize();

		//変換行列
		//スケール
		i.scale_ = { 1,1,1 };
		Matrix4 matScale;
		SetScaleMatrix(matScale, i.scale_);

		//回転
		i.rotation_ =
		{ rotDist(engine), rotDist(engine), rotDist(engine) };
		//回転行列
		Matrix4 matRot, matRotX, matRotY, matRotZ;
		SetRotationMatrix(matRotX, i.rotation_.x, 'x');
		SetRotationMatrix(matRotY, i.rotation_.y, 'y');
		SetRotationMatrix(matRotZ, i.rotation_.z, 'z');
		matRot = matRotZ * matRotX * matRotY;

		//平行移動
		i.translation_ =
		{ posDist(engine),posDist(engine),posDist(engine) };
		Matrix4 matTrans = MathUtility::Matrix4Identity();
		SetTranslationMatrix(matTrans, i.translation_);
		i.matWorld_ = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
		};
		Matrix4xMatrix4(i.matWorld_, matScale * matRot * matTrans);

		i.TransferMatrix();
	}

	//viewProjection_.eye = { 0,0,-10 };
	viewProjection_.target = { 10,0,0 };
	viewProjection_.up = { cosf(pi / 4.f),sinf(pi / 4.f),0 };
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	/*debugCamera_->Update();*/

	const float kEyeSpeed = 0.2f;
	if (input_->PushKey(DIK_W)) viewProjection_.eye.z += kEyeSpeed;
	else if (input_->PushKey(DIK_S)) viewProjection_.eye.z -= kEyeSpeed;

	if (input_->PushKey(DIK_A)) viewProjection_.target.x -= kEyeSpeed;
	else if (input_->PushKey(DIK_D)) viewProjection_.target.x += kEyeSpeed;

	//up回転
	const float kUprotSpeed = 0.05f;
	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUprotSpeed;
		viewAngle = fmodf(viewAngle, pi * 2.f);
	}
	viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)",
		viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)",
		viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)",
		viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	for (int x = 0; x < 31; x++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -30.f + 2.f * (float)x, 0, -30.f }, { -30.f + 2.f * (float)x, 0, 30.f }, { 0,0,0,1 });
	}

	for (int z = 0; z < 31; z++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d({ -30.f, 0, -30.f + 2.f * (float)z }, { 30.f, 0, -30.0f + 2.f * (float)z }, { 0,0,0,1 });
	}
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3dモデル描画
	for (WorldTransform& worldTransform_ : worldTransforms_)
	{
		model_->Draw(worldTransform_, viewProjection_, textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
