#include "Enemy.h"
#include "../../Utility/InputControl.h"
#include"DxLib.h"

//コンストラクタ
Enemy::Enemy() :animation_count(0), velocity(0.0)
{
	animation[0] = NULL;
	animation[1] = NULL;
}

//デストラクタ
Enemy::~Enemy()
{}

//初期化処理
void Enemy::Initialize()
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Fly-enemy/1.png");
	animation[1] = LoadGraph("Resource/Images/Fly-enemy/2.png");

	//エラーチェック
	if (animation[0] == -1 || animation[1] == -1)
	{
		throw ("ハネテキの画像がありません\n");
	}

	//向きの設定
	radian = 0.0;

	//大きさの設定
	box_size = 64.0f;

	//初期画像の設定
	image = animation[0];

	//移動速度の設定
	if (location.x <= 0.0f)
	{
		velocity = Vector2D(1.0f, 0.0f);
	}
	else if (location.x >= 640.0f)
	{
		velocity = Vector2D(-1.0f, 0.0f);
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
	if (velocity.x > 0.0f)
	{
		flip_flag = FALSE;
	}
	else
	{
		flip_flag = TRUE;
	}

	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.8, radian, image, TRUE, flip_flag);

	__super::Draw();
}

//終了時処理
void Enemy::Finalize()
{
	//使用した画像を開放する
	DeleteGraph(animation[0]);
	DeleteGraph(animation[1]);
}

//当たり判定通知処理
void Enemy::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
	velocity *= -1.0f;
}

//移動処理
void Enemy::Movement()
{
	//壁で反射する
	if (location.x > 640.0f)
	{
		velocity.x -= 0.1f;
	}
	if (location.x < 0.0f)
	{
		velocity.x += 0.1f;
	}

	//現在の位置座標に速さを加算する
	location += velocity;
}

//アニメーション制御
void Enemy::AnimeControl()
{
	//フレームカウントを加算する
	animation_count++;

	//６０フレーム目に到達したら
	if (animation_count >= 60)
	{
		//カウントのリセット
		animation_count = 0;

		//画像の切り替え
		if (image == animation[0])
		{
			image = animation[1];
		}
		else
		{
			image = animation[0];
		}
	}
}
