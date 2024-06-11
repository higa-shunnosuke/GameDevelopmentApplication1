#include "Bullet.h"
#include "DxLib.h"
#include "math.h"

//コンストラクタ
Bullet::Bullet() :frame_count(0), animation_max(0), count(0), vector(0.0), Is_hit(false), Is_anim(false)
{
	for (int i = 0; i < 4; i++)
	{
		animation[i] = NULL;
	}
	//ポインタの初期化
	player = nullptr;
}

//デストラクタ
Bullet::~Bullet()
{}

//初期化処理
void Bullet::Initialize(int object_type)
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/bullet/1.png");
	animation[1] = LoadGraph("Resource/Images/bullet/2.png");
	animation[2] = LoadGraph("Resource/Images/bullet/3.png");
	animation[3] = LoadGraph("Resource/Images/bullet/4.png");

	//大きさの設定
	box_size = 30.0f;
	
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

	//初期画像の初期化
	image = animation[0];

	//オブジェクトタイプの初期化
	type = object_type;

	//移動速度の初期化
	vector = Vector2D(0.0f, 0.0f);
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
	//バレット画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//終了時処理
void Bullet::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 4; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//当たり判定通知処理
void Bullet::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
	if (hit_object->GetType() > 0)
	{
		Is_hit = false;
	}
	else
	{
		Is_hit = true;

		//弾の動きを止める
		vector = Vector2D(0.0f);
	}
}

//削除判定通知処理
bool Bullet::Delete()
{
	bool ret = false;

	//画面外に行ったら
	if (location.y < 0.0f || Is_anim == true)
	{
		ret = true;
	}

	return ret;
}

//プレイヤーのポインタを受け取る、ついでに進行方向を設定する
void Bullet::SetPlayer(Player* player)
{
	this->player = player;

	vector.x = -1.0f * cos(atan(player->GetLocation().x));
	vector.y = -1.0f * sin(atan(player->GetLocation().y));
}

//タイプ取得処理
int Bullet::GetType()
{
	return this->type;
}

//移動処理
void Bullet::Movement()
{
	//現在の位置座標に速さを加算する
	location += vector;
}

//アニメーション制御
void Bullet::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 10)
	{
		//カウントのリセット
		frame_count = 0;

		if (Is_hit == true)
		{
			//画像の切り替え
			if (image == animation[animation_max - 1])
			{
				count = 0;
				//アニメーション終了
				Is_anim = true;
			}
			else
			{
				count++;
				image = animation[count];
			}
		}		
	}
}
