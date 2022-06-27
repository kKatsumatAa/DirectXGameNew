#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	//delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {
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
	worldTransform2_.Initialize();
	worldTransform2_.scale_.z=10.f;
	worldTransform2_.translation_.x = -10.f;
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.eye = { 0,10,-10 };
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q))
	{
		if (isDebugCamera)isDebugCamera = false;
		else              isDebugCamera = true;
	}
#endif
	

	//player_->Update();
	//player2_->Update();

	
	//カメラ
	if (isDebugCamera)
	{
		debugCamera_->Update();
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.TransferMatrix();
		//viewProjection_.TransferMatrix();
	}
	else
	{
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
		//viewProjection_.TransferMatrix();
	}

	{
		Vector3 move = { 0,0,0 };
		move.x = input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT);
		move.y = input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN);

		worldTransform2_.translation_.x += move.x;
		worldTransform2_.translation_.z += move.y;

		UpdateWorldMatrix4(worldTransform2_);
	}

	//レイとの当たり判定
	Vector3 rayLength = { 0,0,worldTransform2_.scale_.z * 2.f };
	Vector3 farPos = { worldTransform2_.translation_.x + rayLength.x,
		worldTransform2_.translation_.y + rayLength.y,
		worldTransform2_.translation_.z + rayLength.z / 2 };
	Vector3 nearPos = { worldTransform2_.translation_.x,
		worldTransform2_.translation_.y - rayLength.y,
		worldTransform2_.translation_.z - rayLength.z / 2 };
	rayLength.Normalized();
	Vector3 objLength = worldTransform_.translation_ - nearPos;
	Vector3 dotPos = nearPos + rayLength * rayLength.Dot(objLength);
	Vector3 dotVec = worldTransform_.translation_ - dotPos;
	float dotLength = dotVec.GetLength();
	if (dotLength < worldTransform_.scale_.z*2.f
		&& (worldTransform_.translation_.z + worldTransform_.scale_.z > nearPos.z 
			&& worldTransform_.translation_.z - worldTransform_.scale_.z < farPos.z))
	{
		isRay = true;
	}
	else
	{
		isRay = false;
	}
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)",
		viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)",
		viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)%d",
		viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	if (isRay)
	{
		debugText_->SetPos(50, 110);
		debugText_->Printf("!!!!!HIT!!!!!");
	}

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
	//3dモデル描画
	//player_->Draw(viewProjection_);
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform2_, viewProjection_, textureHandle_);


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
