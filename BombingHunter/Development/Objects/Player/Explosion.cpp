#include "Explosion.h"
#include "DxLib.h"

//コンストラクタ
Explosion::Explosion(Vector2D location) :frame_count(0), animation_max(0), count(0)
{
	for (int i = 0; i < 3; i++)
	{
		animation[i] = NULL;
	}
}

//デストラクタ
Explosion::~Explosion()
{

}

//初期化処理
void Explosion::Initialize(int e_type)
{
	//画像の読み込み
//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Explosion/1.png");
	animation[1] = LoadGraph("Resource/Images/Explosion/2.png");
	animation[2] = LoadGraph("Resource/Images/Explosion/3.png");

	//エラーチェック
	for (int i = 0; i < 5; i++)
	{
		if (animation[i] == -1)
		{
			throw ("爆発の画像がありません\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}

	//初期画像の初期化
	image = animation[0];

	//向きの初期化
	radian = 0;

}

//更新処理
void Explosion::Update()
{
	//アニメーション制御
	AnimeControl();
}

//描画処理
void Explosion::Draw() const
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

}

//終了時処理
void Explosion::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//アニメーション制御
void Explosion::AnimeControl()
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