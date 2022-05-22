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

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			worldTransforms_[i][j].translation_ = { -10.f + 2.5f * i,-10.f + 2.5f * j,0 };

			//ワールドトランスフォームの初期化
			worldTransforms_[i][j].Initialize();
			UpdateWorldMatrix4(worldTransforms_[i][j]);
		}
	}

	viewProjection_.eye = { 0,0,-50.f };
	viewProjection_.target = { 0,0,0 };
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	viewProjection_.target.x += (input_->PushKey(DIK_D) - input_->PushKey(DIK_A));
	viewProjection_.target.y += (input_->PushKey(DIK_W) - input_->PushKey(DIK_S));

	viewProjection_.fovAngleY += ((input_->PushKey(DIK_DOWN) - input_->PushKey(DIK_UP))*0.01f);
	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, pi);
	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);

	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x,
		viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("fovAngleY:(%f)", viewProjection_.fovAngleY);
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
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			model_->Draw(worldTransforms_[i][j], viewProjection_, textureHandle_);
		}
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
