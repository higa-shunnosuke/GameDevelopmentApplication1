#include"Enemy.h"
#include"DxLib.h"

//コンストラクタ
Enemy::Enemy() :frame_count(0),animation_max(0), count(0), vector(0.0),speed(0.0f),type(0)
{
	for (int i = 0; i < 5; i++)
	{
		animation[i] = NULL;
	}
}

//デストラクタ
Enemy::~Enemy()
{
	Finalize();
}

//初期化処理
void Enemy::Initialize(int e_type)
{
	switch (e_type)
	{
	case 0:
		//画像の読み込み
		animation[0] = LoadGraph("Resource/Images/Harpy/1.png");
		animation[1] = LoadGraph("Resource/Images/Harpy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//大きさの設定
		box_size = 80.0f;
		//移動速度の設定
		speed = 1.0f + (float)GetRand(1);
		//敵のタイプの設定
		type = e_type;
		break;
	case 1:
		//画像の読み込み
		animation[0] = LoadGraph("Resource/Images/Fly-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Fly-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//大きさの設定
		box_size = 70.0f;
		//移動速度の設定
		speed = 1.0f + (float)GetRand(1);
		//敵のタイプの設定
		type = e_type;		break;
	case 2:
		//画像の読み込み
		animation[0] = LoadGraph("Resource/Images/Box-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Box-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//大きさの設定
		box_size = 70.0f;
		//移動速度の設定
		speed = 0.7f;
		//敵のタイプの設定
		type = e_type;
		break;
	case 3:
		//画像の読み込み
		animation[0] = LoadGraph("Resource/Images/Gorld-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Gorld-enemy/2.png");
		animation[2] = LoadGraph("Resource/Images/Gorld-enemy/3.png");
		animation[3] = LoadGraph("Resource/Images/Gorld-enemy/4.png");
		animation[4] = LoadGraph("Resource/Images/Gorld-enemy/5.png");
		//大きさの設定
		box_size = 50.0f;
		//移動速度の設定
		speed = 1.0f;
		//敵のタイプの設定
		type = e_type;
		break;
	default:
		break;
	}
	
	//エラーチェック
	for (int i = 0; i < 5; i++)
	{
		if (animation[i] == -1)
		{
			throw ("ハネテキの画像がありません\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}
	

	//向きの設定
	radian = 0.0;

	//初期画像の設定
	image = animation[0];

	//移動速度の設定
	if (location.x <= 0.0f)
	{
		vector = Vector2D(speed, 0.0f);
	}
	else if (location.x >= 640.0f)
	{
		vector = Vector2D(-speed, 0.0f);
	}
}

//更新処理
void Enemy::Update()
{
	//移動処理
	Movement();
	//アニメーション制御
	AnimeControl();
}

//描画処理
void Enemy::Draw() const
{
	int flip_flag;		//画像反転フラグ

	//進行方向によって、反転状態を決定する
	if (vector.x > 0.0f)
	{
		flip_flag = FALSE;
	}
	else
	{
		flip_flag = TRUE;
	}

	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, flip_flag);

	__super::Draw();
}

//終了時処理
void Enemy::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//当たり判定通知処理
void Enemy::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
}

//削除判定通知処理
bool Enemy::Delete()
{
	bool ret = false;

	//壁で反射する
	if (location.x > 640.0f + box_size.x)
	{
		ret = true;
	}
	if (location.x < 0.0f - box_size.x)
	{
		ret = true;
	}

	return ret;
}

//プレイヤーのポインタを受け取る
void Enemy::SetPlayer(Player* player)
{
	this->player = player;
}


//タイプ取得処理
int Enemy::GetType()
{
	return type;
}

//移動処理
void Enemy::Movement()
{
	//現在の位置座標に速さを加算する
	location += vector;
}

//アニメーション制御
void Enemy::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 60)
	{
		//カウントのリセット
		frame_count = 0;

		//画像の切り替え
		if (image == animation[animation_max-1])
		{
			count = 0;
			image = animation[count];
		}
		else
		{
			count++;
			image = animation[count];
		}
	}
}
