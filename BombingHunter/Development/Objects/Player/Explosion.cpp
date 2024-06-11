#include "Explosion.h"
#include "DxLib.h"

//コンストラクタ
Explosion::Explosion() :frame_count(0), animation_max(0), count(0), radian(0.0f), Is_anim(false)
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
void Explosion::Initialize(int object_type)
{
	//画像の読み込み
//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Explosion/1.png");
	animation[1] = LoadGraph("Resource/Images/Explosion/2.png");
	animation[2] = LoadGraph("Resource/Images/Explosion/3.png");

	//エラーチェック
	for (int i = 0; i < 3; i++)
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

	//大きさの設定
	box_size = 0.0f;

	//初期画像の初期化
	image = animation[0];

	//オブジェクトタイプの設定
	type = object_type;

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
	//爆破エフェクト画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

}

//終了時処理
void Explosion::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(animation[i]);
	}
}

// 削除判定通知処理
bool Explosion::Delete()
{
	bool ret = false;

	//アニメーションが終了したら
	if (Is_anim == true)
	{
		ret = true;
	}

	return ret;
}

//タイプ取得処理
int Explosion::GetType()
{
	return this->type;
}

//アニメーション制御
void Explosion::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 10)
	{
		//カウントのリセット
		frame_count = 0;

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