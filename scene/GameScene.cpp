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
	worldTransforms_[0].translation_ = { 0,2.5f,0 };
	worldTransforms_[1].translation_ = { -2.5f,-2.5f,0 };
	worldTransforms_[2].translation_ = { 2.5f,-2.5f,0 };

	for (size_t i = 0; i < _countof(worldTransforms_); i++)
	{
		//ワールドトランスフォームの初期化
		worldTransforms_[i].Initialize();
		UpdateWorldMatrix4(worldTransforms_[i]);
	}

	viewProjection_.eye = { 0,0,-20.f };
	viewProjection_.target = { 0,0,0 };
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE))//動く準備
	{
		toNum++;
		if (toNum >= _countof(worldTransforms_))
		{
			toNum = 0;
		}
		toVec = worldTransforms_[toNum].translation_ - viewProjection_.target;
		toVec= toVec.GetNormalized()*0.1f;
		move = true;
	}
	if (move)//動かす
	{
		viewProjection_.target += toVec;
	}

	Vector3 compareVec = worldTransforms_[toNum].translation_ - viewProjection_.target;//通り過ぎてるか比較用のベクトル
	if (compareVec.x * toVec.x < 0 || compareVec.y * toVec.y < 0 || compareVec.z * toVec.z < 0)//x,y,zどれかの符号が違っていたら通り過ぎている
	{
		viewProjection_.target = worldTransforms_[toNum].translation_;//止める
		move = false;
	}
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(":(%f,%f,%f)", worldTransforms_[0].translation_.x,
		worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
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
	for (size_t i = 0; i < _countof(worldTransforms_); i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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
