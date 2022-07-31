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

	CheckAllCollision();
	
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
		//const float kEyeSpeed = 0.2f;
		//if (input_->PushKey(DIK_W)) viewProjection_.eye.z += kEyeSpeed;
		//else if (input_->PushKey(DIK_S)) viewProjection_.eye.z -= kEyeSpeed;

		//if (input_->PushKey(DIK_A)) viewProjection_.target.x -= kEyeSpeed;
		//else if (input_->PushKey(DIK_D)) viewProjection_.target.x += kEyeSpeed;

		////up回転
		//const float kUprotSpeed = 0.05f;
		//if (input_->PushKey(DIK_SPACE))
		//{
		//	viewAngle += kUprotSpeed;
		//	viewAngle = fmodf(viewAngle, pi * 2.f);
		//}
		//viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };

		viewProjection_.UpdateMatrix();
		//viewProjection_.TransferMatrix();
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

	/*Vector3 v1 = { 10.0f,0,0 };
	Vector3 v2 = { 0,10.0f,0 };
	Vector3 v = { 0,0,0 };

	debugText_->SetPos(50, 130);
	debugText_->Printf("vecLerp:(%f,%f,%f)%d",
		SlerpVector3(v1, v2, (num)).x,
		SlerpVector3(v1, v2, (num)).y,
		SlerpVector3(v1, v2, (num)).z);

	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, v1, { 0,1,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,0,0 }, v2, { 0,0,1,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d(v1, v2, { 1,1,1,1 });
	Vector3 p = SlerpVector3(v1, v2, (num));
	Vector3 p2 = LerpVector3(v1, v2, (num));
	PrimitiveDrawer::GetInstance()->DrawLine3d(v, p, { 1,1,1,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d(v, p2, { 1,1,0.5f,1 });*/


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

void GameScene::CheckAllCollision()
{
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		CheckCollisionPair(player_, bullet.get());
	}
#pragma endregion


#pragma region 自キャラと敵の当たり判定
	CheckCollisionPair(player_, enemy_);
	
#pragma endregion


#pragma region 自bulletと敵の当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		CheckCollisionPair(enemy_, bullet.get());
	}
#pragma endregion

#pragma region 自bulletと敵bulletの当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletP : playerBullets)
	{
		for (const std::unique_ptr<EnemyBullet>& bulletE : enemyBullets)
		{
			CheckCollisionPair(bulletP.get(), bulletE.get());
		}
	}
#pragma endregion
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();
	
	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
