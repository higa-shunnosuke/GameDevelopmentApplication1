#include "Bullet.h"
#include"DxLib.h"

//コンストラクタ
Bullet::Bullet() :frame_count(0), animation_max(0), count(0), vector(0.0)
{
	for (int i = 0; i < 4; i++)
	{
		animation[i] = NULL;
	}

	player = nullptr;
}

//デストラクタ
Bullet::~Bullet()
{}

//初期化処理
void Bullet::Initialize(int e_type)
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/bullet/1.png");
	animation[1] = LoadGraph("Resource/Images/bullet/2.png");
	animation[2] = LoadGraph("Resource/Images/bullet/3.png");
	animation[3] = LoadGraph("Resource/Images/bullet/4.png");

	//大きさの設定
	box_size = 30.0f;
	//動速度の設定
	
	//エラーチェック
	for (int i = 0; i < 4; i++)
	{
		if (animation[i] == -1)
		{
			throw ("バレットの画像がありません\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}

	//初期画像の設定
	image = animation[0];

	//移動速度の設定
	vector = Vector2D(1.0f, 0.0f);
}

//更新処理
void Bullet::Update()
{
	//移動処理
	Movement();

	//アニメーション制御
	AnimeControl();
}

//描画処理
void Bullet::Draw() const
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//終了時処理
void Bullet::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//当たり判定通知処理
void Bullet::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
	vector *= 0.0f;
}

//削除判定通知処理
bool Bullet::Delete()
{
	bool ret = false;

	//壁で反射する
	if (location.x > 640.0f)
	{
		ret = true;
	}
	if (location.x < 0.0f)
	{
		ret = true;
	}

	return ret;
}

//プレイヤーのポインタを受け取る
void Bullet::SetPlayer(Player* player)
{
	this->player = player;
}

//移動処理
void Bullet::Movement()
{
	if (player->GetLocation().x > location.x)
	{
		vector.x += 1.0f;
	}
	else
	{
		vector.x -= 1.0f;
	}
	
	if (player->GetLocation().y > location.y)
	{
		vector.y += 1.0f;
	}
	else
	{
		vector.y -= 1.0f;
	}

	//現在の位置座標に速さを加算する
	location += vector;
}

//アニメーション制御
void Bullet::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 60)
	{
		//カウントのリセット
		frame_count = 0;

		//画像の切り替え
		if (image == animation[animation_max - 1])
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
