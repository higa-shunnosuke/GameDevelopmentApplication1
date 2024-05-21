#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Enemy/Enemy.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

//コンストラクタ
Scene::Scene():objects(),spawn_count(0), image(NULL),type(0)
{
	//x座標
	LocationX[0] = 0.0f;
	LocationX[1] = 640.0f;
	//y座標
	LocationY[0] = 150.0f;
	LocationY[1] = 250.0f;
	LocationY[2] = 350.0f;
	LocationY[3] = 450.0f;

	//敵の数のカウントの初期化

	Enemy_count[0] = 0;
	Enemy_count[1] = 0;
	Enemy_count[2] = 0;
	Enemy_count[3] = 0;
}

//デストラクタ
Scene::~Scene()
{
	//忘れ防止
	Finalize();
}

//初期化処理
void Scene::Initialize()
{
	image = LoadGraph("Resource/Images/background.png");

	//プレイヤーを生成する
	CreateObject<Player>(Vector2D(320.0f, 50.0f),0);
	//エネミーを生成する
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[0]), 0);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[1]), 1);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[2]), 2);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[3]), 3);

}

//更新処理
void Scene::Update()
{
	if (InputControl::GetKeyDown(KEY_INPUT_Z))
	{
		if (Enemy_count[type] < 4)
		{
			CreateObject<Enemy>(Vector2D(LocationX[1], LocationY[type]), type);
			Enemy_count[type] += 1;
			type++;
		}
	}
		
	//シーンに存在するオブジェクトの更新処理
	for (GameObject* obj:objects)
	{
		obj->Update();
	}

	//オブジェクト同士の当たり判定チェック
	for (int i = 0; i < objects.size(); i++)
	{
		//当たり判定チェック処理
		HitCheckObject(objects[0], objects[i]);
	}
}

//描画処理
void Scene::Draw() const
{
	//背景画像の描画
	DrawRotaGraphF(320.0f, 240.0f, 0.8, 0, image, TRUE, 0);

	//シーンに存在するオブジェクトの描画処理
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(10, 10 + i * 20, 0x00, "カウント%d：%d",i, Enemy_count[i]);
	}
	DrawFormatString(10, 90, 0x00, "タイプ：%d", type);
}

//終了時処理
void Scene::Finalize()
{
	//動的配列が空なら処理を終了する
	if (objects.empty())
	{
		return;
	}

	//各オブジェクトを削除する
	for (GameObject* obj:objects)
	{
		obj->Finalize();
		delete obj;
	}

	//動的配列の解放
	objects.clear();
}

//当たり判定チェック処理（矩形の中心で当たり判定をとる）
void Scene::HitCheckObject(GameObject* a, GameObject* b)
{
	//２つのオブジェクトの距離を取得
	Vector2D diff = a->GetLocation() - b->GetLocation();

	//２つのオブジェクトの当たり判定の大きさを取得
	Vector2D box_size = (a->GetBoxSize() + b->GetBoxSize()) / 2.0f;

	//距離より大きさが大きい場合、Hit判定とする
	if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
	{
		//当たったことをオブジェクトに通知する
		a->OnHitCollision(b);
		b->OnHitCollision(a);
	}
}