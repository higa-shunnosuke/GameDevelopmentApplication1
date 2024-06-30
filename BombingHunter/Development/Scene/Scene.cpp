#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Player/Bomb.h"
#include "../Objects/Player/Explosion.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Enemy/Evaluation.h"
#include "../Objects/Enemy/Bullet.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

#define SCORE_DATA ("Resource/dats/highscore.csv")

Player* p;				//プレイヤーのポインタ
int image;

//コンストラクタ
Scene::Scene():objects(), frame_count(0), time(0),
background_image(NULL),
background_sound(NULL),
timer_image(NULL),
score_image(NULL),
minus_image(NULL),
highscore_image(NULL),
throw_SE(NULL),
explosion_SE(NULL),
timeup_SE(NULL),
timeup(false)

{
	//x座標
	LocationX[0] = 0.0f;
	LocationX[1] = 640.0f;
	//y座標
	LocationY[0] = 150.0f;
	LocationY[1] = 240.0f;
	LocationY[2] = 330.0f;
	LocationY[3] = 410.0f;

	//敵の数のカウントを初期化
	for (int i = 0; i < 4; i++)
	{
		Enemy_count[i] = 0;
	}

	//数字画像の初期化
	for (int i = 0; i < 11; i++)
	{
		number[i] = NULL;
	}

	//ボムの数のカウントを初期化
	Bomb_count = 0;

	//スコアの初期化
	score = 0;
	highscore = 0;

	//ポーズフラグの初期化
	Is_pause = false;
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
	//画像の読込み
	background_image = LoadGraph("Resource/Images/background.png");
	timer_image = LoadGraph("Resource/Images/Evaluation/timer.png");
	score_image = LoadGraph("Resource/Images/Score/score.png");
	highscore_image = LoadGraph("Resource/Images/Score/highscore.png");
	minus_image = LoadGraph("Resource/Images/Score/-.png");
	LoadDivGraph("Resource/images/Score/numbers.png", 10, 5, 2, 160, 214, number);

	//音源の読み込み
	background_sound = LoadSoundMem("Resource/sounds/BGM_arrows.wav");
	throw_SE = LoadSoundMem("Resource/sounds/pan.wav");
	explosion_SE = LoadSoundMem("Resource/sounds/explosion.wav");
	timeup_SE = LoadSoundMem("Resource/sounds/BGM_timeup.wav");

	//ファイルパス
	FILE* fp = NULL;

	//読込みファイルを開く
	fopen_s(&fp, SCORE_DATA, "r");

	//エラーチェック
	if (fp == NULL)
	{
		throw("ファイルが読み込めません\n");
	}
	else
	{
		//ハイスコアを読み込む
		fscanf_s(fp,"%d",&highscore);

		//ファイルを閉じる
		fclose(fp);
	}

	//プレイヤーを生成する
	p = CreateObject<Player>(Vector2D(320.0f, 50.0f),TYPE::PLAYER);

	//敵の出現数の設定
	Enemy_count[0] = 0;
	Enemy_count[1] = 0;
	Enemy_count[2] = 0;
	Enemy_count[3] = 0;

	//ポーズフラグの初期化
	Is_pause = false;

	//フレームカウントの初期化
	frame_count = 0;

	//制限時間の初期化
	time = 60;

	image = number[0];

	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP);

}

//更新処理
void Scene::Update()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count == 60)
	{
		//カウントのリセット
		if (time > 0)
		{
			time--;
			frame_count = 0;
		}
		
	}

	//制限時間になったら
	if (time <= 0)
	{		
		if (timeup == false)
		{
			StopSoundMem(background_sound);
			//DeleteSoundMem(background_sound);
			PlaySoundMem(timeup_SE, DX_PLAYTYPE_BACK);
			Finalize();
			frame_count = 0;

			p = CreateObject<Player>(Vector2D(320.0f, 50.0f), TYPE::PLAYER);

			timeup = true;
		}

		if (frame_count == 180)
		{
			if (score >= 3000)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::PERFECT);
			}
			else if (score >= 1500)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::GOOD);
			}
			else if (score >= 1000)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::OK);
			}
			else
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::BAD);
			}
		}
		
		//スペースキーが押されたら、ボムを生成する
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			if (Bomb_count < 1)
			{
				CreateObject<Bomb>(objects[0]->GetLocation(), TYPE::BOMB)->SetPlayer(p);
				Bomb_count++;
				PlaySoundMem(throw_SE, DX_PLAYTYPE_BACK);
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
			for (int j = i + 1; j < objects.size(); j++)
			{
				//当たり判定チェック処理
				HitCheckObject(objects[i], objects[j]);
			}
		}

		//オブジェクトの削除判定チェック
		for (int i = 0; i < objects.size(); i++)
		{
			//削除判定チェック処理
			if (objects[i]->Delete() == true)
			{
				if (objects[i]->GetType() == 1)
				{
					Bomb_count--;

					//爆破エフェクトの生成
					CreateObject<Explosion>(objects[i]->GetLocation(), TYPE::EXPLOSION);
					PlaySoundMem(explosion_SE, DX_PLAYTYPE_BACK);
				}
				//オブジェクトの削除
				objects.erase(objects.begin() + i);
			}
		}
	}
	//制限時間が０じゃないのとき
	else
	{
		//それぞれのエネミーを出現数の上限まで生成する
		//ハーピーを生成する
		//生成を１秒待ってハーピーが重ならないようにする
		if (Enemy_count[0] < 2)
		{
			//生成する間隔をあける
			if (frame_count == 0)
			{
				//生成する確率を調整
				if (GetRand(100) <= 40)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(1) + 1]), TYPE::HARPY);
					Enemy_count[TYPE::HARPY - 4] += 1;
				}
			}
		}
		//ハネテキを生成する
		if (Enemy_count[1] < 5)
		{
			//生成する間隔をあける
			if (frame_count == 0)
			{
				//生成する確率を調整
				if (GetRand(100) <= 50)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(2)]), TYPE::FLY_ENEMY);
					Enemy_count[TYPE::FLY_ENEMY - 4] += 1;
				}
			}
		}
		//ハコテキを生成する
		if (Enemy_count[2] < 1)
		{
			//生成する間隔をあける
			if (frame_count == 0)
			{
				//生成する確率を調整
				if (GetRand(100) <= 20)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::BOX_ENEMY);
					Enemy_count[TYPE::BOX_ENEMY - 4] += 1;
				}
			}
		}
		//金のテキを生成する
		if (Enemy_count[3] < 1)
		{
			//生成する間隔をあける
			if (time % 30 == 0)
			{
				//生成する確率を調整
				if (GetRand(100) <= 1)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::GORLD_ENEMY);
					Enemy_count[TYPE::GORLD_ENEMY - 4] += 1;
				}
			}
		}
		//弾を生成する
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetType() == TYPE::BOX_ENEMY)
			{
				//生成する間隔をあける
				if (frame_count == 0)
				{
					//生成する確率を調整
					if (GetRand(10) <= 3)
					{
						CreateObject<Bullet>(Vector2D(objects[i]->GetLocation()), TYPE::BULLET)->SetPlayer(p);
					}
				}
			}
		}

		//スペースキーが押されたら、ボムを生成する
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			if (Bomb_count < 1)
			{
				CreateObject<Bomb>(objects[0]->GetLocation(), TYPE::BOMB)->SetPlayer(p);
				Bomb_count++;
				PlaySoundMem(throw_SE, DX_PLAYTYPE_BACK);
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
			for (int j = i + 1; j < objects.size(); j++)
			{
				//当たり判定チェック処理
				HitCheckObject(objects[i], objects[j]);
			}
		}
		
		//オブジェクトの削除判定チェック
		for (int i = 0; i < objects.size(); i++)
		{
			//削除判定チェック処理
			if (objects[i]->Delete() == true)
			{
				if (objects[i]->GetType() > 3)
				{
					Enemy_count[objects[i]->GetType() - 4 ]--;
				}
				else if (objects[i]->GetType() == 1)
				{
					Bomb_count--;

					//爆破エフェクトの生成
					CreateObject<Explosion>(objects[i]->GetLocation(), TYPE::EXPLOSION);
					PlaySoundMem(explosion_SE, DX_PLAYTYPE_BACK);
				}
				//オブジェクトの削除
				objects.erase(objects.begin() + i);
			}
		}
	}
}

//描画処理
void Scene::Draw() const
{
	//背景画像の描画
	DrawRotaGraphF(320.0f, 240.0f, 0.7, 0, background_image, TRUE, 0);

	//シーンに存在するオブジェクトの描画処理
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	//スコア画像の描画
	DrawRotaGraphF(150.0f, 465.0f, 1.1, 0, score_image, TRUE, 0);
	//ハイスコア画像の描画
	DrawRotaGraphF(350.0f, 465.0f, 1.2, 0, highscore_image, TRUE, 0);
	//タイマー画像の描画
	DrawRotaGraphF(20.0f, 465.0f, 0.5, 0, timer_image, TRUE, 0);

	UIDraw();
}

//終了時処理
void Scene::Finalize()
{
	//ファイルパス
	FILE* fp = NULL;

	if (score > highscore)
	{

		//読込みファイルを開く
		fopen_s(&fp, SCORE_DATA, "w");

		//エラーチェック
		if (fp == NULL)
		{
			throw("ファイルが読み込めません\n");
		}
		else
		{
			//ハイスコアを読み込む
			fprintf(fp,"%d",score);

			//ファイルを閉じる
			fclose(fp);
		}
	}

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

		//プレイヤーが弾に当たると制限時間を減らす
		if (a->GetType() == TYPE::PLAYER && b->GetType() == TYPE::BULLET)
		{
			time -= 2;
		}
		//ハーピーがボムに当たるとスコアを減らす
		if (a->GetType() == TYPE::HARPY && b->GetType() == TYPE::BOMB)
		{
			score -= 100;
		}
		//ハネテキがボムに当たるとスコアを増やす
		if (a->GetType() == TYPE::FLY_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 30;
		}
		//ハコテキがボムに当たるとスコアを増やす
		if (a->GetType() == TYPE::BOX_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 200;
		}
		//金のテキがボムに当たるとスコアを増やす
		if (a->GetType() == TYPE::GORLD_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 1500;
		}
	}
}

//UI描画処理
void Scene::UIDraw() const
{
	//スコアから計算
	int s1;			//スコア
	int i;			//カウント用の変数

	//スコアがマイナスだったとき
	if (score < 0)
	{
		s1 = -score;
		i = 1;
	}
	else
	{
		s1 = score;
		i = 0;
	}

	int s2 = s1;		//上１桁を省いた値
	int s3 = 0;			//上一桁の位の値
	int digit = 0;		//桁の数
	int num;			//番号
	

	//スコアの桁数を確認
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//小さい位から順に省いていく
	}

	//スコアを描画
	while (digit > -1)
	{
		//上１桁を省いた値
		s2 = s1 % (int)pow(10, digit);

		//上一桁の位の値
		s3 = s1 - s2;

		//上一桁の数
		num = s3 / (int)pow(10, digit);

		//マイナスを描画
		if (score < 0)
		{
			DrawRotaGraphF(185.0f, 465.0f, 1.0, 0, minus_image, TRUE, 0);
		}

		//上一桁の数を描画
		DrawRotaGraphF(185.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);
		
		//位をずらす
		digit--;
		s1 = s2;
		i++;
	}
	
	//ハイスコアを計算
	s1 = highscore;
	s2 = s1;
	digit = 0;
	i = 0;

	//ハイスコアの桁数を確認
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//小さい位から順に省いていく
	}

	//ハイスコアを描画
	while (digit > -1)
	{
		//上１桁を省いた値
		s2 = s1 % (int)pow(10, digit);

		//上一桁の位の値
		s3 = s1 - s2;

		//上一桁の数
		num = s3 / (int)pow(10, digit);

		//上一桁の数を描画
		DrawRotaGraphF(405.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);
		
		//位をずらす
		digit--;
		s1 = s2;
		i++;
	}

	//タイムを計算
	s1 = time;
	s2 = s1;
	digit = 0;
	i = 0;

	//タイムの桁数を確認
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//小さい位から順に省いていく
	}

	//タイムを描画
	while (digit > -1)
	{
		//上１桁を省いた値
		s2 = s1 % (int)pow(10, digit);

		//上一桁の位の値
		s3 = s1 - s2;

		//上一桁の数
		num = s3 / (int)pow(10, digit);

		//上一桁の数を描画
		DrawRotaGraphF(45.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);

		//位をずらす
		digit--;
		s1 = s2;
		i++;
	}
}