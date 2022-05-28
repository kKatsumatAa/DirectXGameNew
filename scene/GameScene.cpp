#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

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

	// 親(Root)
		//ワールドトランスフォーム初期化
	worldTransforms_[PartId::kRoot].Initialize();

	//脊髄
	// x,y,zの位置を設定
	worldTransforms_[PartId::kSpine].translation_ = { 0, 2.f, 0 };//ローカル座標
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].Initialize();

	//上半身
	//胸
	worldTransforms_[PartId::kChest].translation_ = { 0, 2.0f, 0 }; //ローカル座標
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].Initialize();
	//頭
	worldTransforms_[PartId::kHead].translation_ = { 0, 2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].Initialize();
	//左手
	worldTransforms_[PartId::kArmL].translation_ = { 2.f, 0, 0 }; //ローカル座標
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].Initialize();
	//右手
	worldTransforms_[PartId::kArmR].translation_ = { -2.f, 0, 0 }; //ローカル座標
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].Initialize();
	//左手2
	worldTransforms_[PartId::kArmL2].translation_ = { 0, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kArmL2].parent_ = &worldTransforms_[PartId::kArmL];
	worldTransforms_[PartId::kArmL2].Initialize();
	//右手2						
	worldTransforms_[PartId::kArmR2].translation_ = { -0, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kArmR2].parent_ = &worldTransforms_[PartId::kArmR];
	worldTransforms_[PartId::kArmR2].Initialize();


	//下半身
	//尻
	worldTransforms_[PartId::kHip].translation_ = { 0, -2.0f, 0 }; //ローカル座標
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].Initialize();
	//左足
	worldTransforms_[PartId::kLegL].translation_ = { 2.f, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].Initialize();
	//右足
	worldTransforms_[PartId::kLegR].translation_ = { -2.f, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].Initialize();
	//左足2
	worldTransforms_[PartId::kLegL2].translation_ = { 0, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kLegL2].parent_ = &worldTransforms_[PartId::kLegL];
	worldTransforms_[PartId::kLegL2].Initialize();
	//右足2						
	worldTransforms_[PartId::kLegR2].translation_ = { 0, -2.f, 0 }; //ローカル座標
	worldTransforms_[PartId::kLegR2].parent_ = &worldTransforms_[PartId::kLegR];
	worldTransforms_[PartId::kLegR2].Initialize();


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	{
		//移動
		{
			Vector3 move = { (float)(input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT)),
				(float)(input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN)),
				0 };
			worldTransforms_[PartId::kRoot].translation_ += move;
		}
		{//回転
			worldTransforms_[0].rotation_ += { 0,
				(float)(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * 0.1f,
				0 };
			/*worldTransforms_[0].rotation_ += {
				(float)(input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * 0.1f,
					0, 0 };*/

			//自動回転

			if (input_->PushKey(DIK_W))
			{
				if (worldTransforms_[kArmL].rotation_.x >= maxRote
					|| worldTransforms_[kArmL].rotation_.x <= minRote)
				{
					roteSpeed = -roteSpeed;
				}
				worldTransforms_[kArmL].rotation_ += roteSpeed;
				worldTransforms_[kLegR].rotation_ += roteSpeed;
				worldTransforms_[kArmR].rotation_ -= roteSpeed;
				worldTransforms_[kLegL].rotation_ -= roteSpeed;
			}
			

			worldTransforms_[kHead].rotation_ += {0.1f, 0.1f, 0.1f};
			f += 0.1f;
			worldTransforms_[kHead].translation_.y = 4.5f + sinf(f) * 2;
		}
		
		for (size_t i = 0; i < kNumPartId; i++)
		{
			if (worldTransforms_[i].rotation_.x != 0.f && !input_->PushKey(DIK_W) && kArmL<=i)//自動で手足が戻る処理
			{
				if (worldTransforms_[i].rotation_.x > 0)
				{
					worldTransforms_[i].rotation_.x -= 0.05f;
					if (worldTransforms_[i].rotation_.x < 0) worldTransforms_[i].rotation_.x = 0;
				}

				if (worldTransforms_[i].rotation_.x < 0)
				{
					worldTransforms_[i].rotation_.x += 0.05f;
					if (worldTransforms_[i].rotation_.x > 0) worldTransforms_[i].rotation_.x = 0;
				}
			}

			UpdateWorldMatrix4(worldTransforms_[i]);
		}
	}

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("ROOTtranslation:(%f,%f,%f)", worldTransforms_[0].translation_.x,
		worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("angle:(%f)", RaditoAngle(worldTransforms_[0].rotation_.y));
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
	for (size_t i = 2; i < kNumPartId; i++)
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
