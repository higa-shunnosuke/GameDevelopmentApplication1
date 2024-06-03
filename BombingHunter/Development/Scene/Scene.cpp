#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Enemy/Bullet.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

bool Is_pause;			//ポーズフラグ
Player* p;

//コンストラクタ
Scene::Scene():objects(), frame_count(0), time(60), image(NULL)
{
	//x座標
	LocationX[0] = 0.0f;
	LocationX[1] = 640.0f;
	//y座標
	LocationY[0] = 150.0f;
	LocationY[1] = 240.0f;
	LocationY[2] = 330.0f;
	LocationY[3] = 410.0f;

	//敵の数のカウントの初期化
	for (int i = 0; i < 4; i++)
	{
		Enemy_count[i] = 0;
	}
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
	p = CreateObject<Player>(Vector2D(320.0f, 50.0f),0);
	//CreateObject<Bullet>(Vector2D(320.0f, 240.0f), 0)->SetPlayer(p);

	//敵の出現数の設定
	Enemy_count[0] = 2;
	Enemy_count[1] = 5;
	Enemy_count[2] = 1;
	Enemy_count[3] = 1;

	//ポーズフラグの初期化
	Is_pause = false;
}

//更新処理
void Scene::Update()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 60)
	{
		//カウントのリセット
		frame_count = 0;
		time--;
	}

	//ポーズ状態のとき
	if (Is_pause == true)
	{
		//スペースキーを押すとポーズを解除する
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			Is_pause = false;
		}
		
		//Aキーを押すとリスタートする
		if (InputControl::GetKeyDown(KEY_INPUT_A))
		{
			Finalize();
			Initialize();
		}
	}
	//ポーズ状態じゃないのとき
	else
	{
		//スペースキーを押すとポーズにする
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			Is_pause = true;
		}

		//それぞれのエネミーを出現数の上限まで生成する
		//ハーピーを生成する
		//生成を１秒待ってハーピーが重ならないようにする
		if (Enemy_count[0] > 0)
		{
			//生成する確率を調整
			if (frame_count == 0)
			{
				if (GetRand(100) <= 40)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(1) + 1]), 0);
					Enemy_count[0] -= 1;
				}
			}
		}
		//ハネテキを生成する
		if (Enemy_count[1] > 0)
		{
			//生成する確率を調整
			if (GetRand(100) <= 20)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(2)]), 1);
				Enemy_count[1] -= 1;
			}
		}
		//ハコテキを生成する
		if (Enemy_count[2] > 0)
		{
			//生成する確率を調整
			if (GetRand(100) <= 50)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), 2);
				Enemy_count[2] -= 1;
			}
		}
		//金のテキを生成する
		if (Enemy_count[3] > 0)
		{
			//生成する確率を調整
			if (GetRand(100) <= 1)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), 3);
				Enemy_count[3] -= 1;
			}
		}
		
		//シーンに存在するオブジェクトの更新処理
		for (GameObject* obj : objects)
		{
			obj->Update();
		}

		//オブジェクト同士の当たり判定チェック
		for (int i = 0; i < objects.size(); i++)
		{
			//当たり判定チェック処理
			HitCheckObject(objects[0], objects[i]);
		}
		
		//オブジェクトの削除判定チェック
		for (int i = 0; i < objects.size(); i++)
		{
			//削除判定チェック処理
			if (objects[i]->Delete() == true)
			{
				Enemy_count[objects[i]->GetType()]++;
				objects.erase(objects.begin() + i);
			}
		}
	}
}

//描画処理
void Scene::Draw() const
{
	//背景画像の描画
	DrawRotaGraphF(320.0f, 240.0f, 0.7, 0, image, TRUE, 0);

	//シーンに存在するオブジェクトの描画処理
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(10, 10 + i * 20, 0x00, "カウント%d：%d",i, Enemy_count[i]);
	}
	DrawFormatString(10, 90, 0x00, "フレーム：%d", frame_count);
	DrawFormatString(10, 110, 0x00, "時間：%d", time);

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