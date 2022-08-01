#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("kasuga.png");
	textureHandle2_ = TextureManager::Load("uvChecker.png");
	textureHandle3_ = TextureManager::Load("kamata.ico");
	//3Dモデルの生成
	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

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


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	Enemy* enemy = new Enemy;
	enemy_ = enemy;
	enemy_->SetPlayer(player_);
	enemy_->Initialize(model_,textureHandle2_);
	
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q))
	{
		if (isDebugCamera)isDebugCamera = false;
		else              isDebugCamera = true;
	}
#endif

	num += 0.01;
	if (num >= 1.0f) num = 0.0f;
	

	player_->Update();
	if(enemy_!=nullptr) enemy_->Update();

	{//colliderManager

		colliderManager->ClearList();
		colliderManager->SetListCollider(player_);
		colliderManager->SetListCollider(enemy_);
		//bulletはそれ自体がlistなので特別
		const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			colliderManager->SetListCollider(bullet.get());
		}
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			colliderManager->SetListCollider(bullet.get());
		}

		colliderManager->CheckAllCollisions();
	}

	//カメラ
	if (isDebugCamera)//
	{
		debugCamera_->Update();
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.TransferMatrix();
		//viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.UpdateMatrix();
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
		viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z,isDebugCamera);
	
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
	player_->Draw(viewProjection_);
	if(enemy_!=nullptr) enemy_->Draw(viewProjection_);

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
