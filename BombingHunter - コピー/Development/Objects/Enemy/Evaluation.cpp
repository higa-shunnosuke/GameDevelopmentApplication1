#include "Evaluation.h"
#include"DxLib.h"

//コンストラクタ
Evaluation::Evaluation()
{
	SE = NULL;
}

//デストラクタ
Evaluation::~Evaluation()
{
	Finalize();
}

//初期化処理
void Evaluation::Initialize(int object_type)
{
	switch (object_type)
	{
	case 8:
		//画像の読み込み
		image = LoadGraph("Resource/Images/Evaluation/Perfect.png");

		//音源の読み込み
		SE = LoadSoundMem("Resource/sounds/SE_perfect.wav");

		//大きさの設定
		box_size = Vector2D(410.0f, 90);
		
		break;
	case 9:
		//画像の読み込み
		image = LoadGraph("Resource/Images/Evaluation/GOOD.png");

		//音源の読み込み
		SE = LoadSoundMem("Resource/sounds/SE_good.wav");

		//大きさの設定
		box_size = Vector2D(400.0f, 140);
		break;
	case 10:
		//画像の読み込み
		image = LoadGraph("Resource/Images/Evaluation/OK.png");

		//音源の読み込み
		SE = LoadSoundMem("Resource/sounds/SE_ok.wav");

		//大きさの設定
		box_size = Vector2D(400.0f, 260);
		break;
	case 11:
		//画像の読み込み
		image = LoadGraph("Resource/Images/Evaluation/BAD.png");

		//音源の読み込み
		SE = LoadSoundMem("Resource/sounds/SE_bad.wav");

		//大きさの設定
		box_size = Vector2D(350.0f,120);
	default:
		break;
	}

	if (image == -1)
	{
		throw ("評価の画像がありません\n");
	}
	
	type = object_type;

	PlaySoundMem(SE, DX_PLAYTYPE_BACK);

}

//更新処理
void Evaluation::Update()
{
	
}

//描画処理
void Evaluation::Draw() const
{
	//評価画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE);

	__super::Draw();
}

//終了時処理
void Evaluation::Finalize()
{
	//使用した画像を開放する
	DeleteGraph(image);
}