#include "Bomb.h"
#include"DxLib.h"

//コンストラクタ
Bomb::Bomb() :frame_count(0), animation_max(0), count(0), vector(0.0), Is_hit(false)
{
	for (int i = 0; i < 4; i++)
	{
		animation[i] = NULL;
	}

	player = nullptr;
}

//デストラクタ
Bomb::~Bomb()
{}

//初期化処理
void Bomb::Initialize(int e_type)
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Bomb/1.png");

	//大きさの設定
	box_size = 30.0f;
	//動速度の設定

	//エラーチェック

	if (animation[0] == -1)
	{
		throw ("ボムの画像がありません\n");
	}
	
	//初期画像の設定
	image = animation[0];

	//移動速度の設定
	vector = Vector2D(0.0f, 1.0f);
}

//更新処理
void Bomb::Update()
{
	//移動処理
	Movement();

}

//描画処理
void Bomb::Draw() const
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, 1.5707963267949, image, TRUE, 0);

	__super::Draw();
}

//終了時処理
void Bomb::Finalize()
{
	//使用した画像を開放する
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//当たり判定通知処理
void Bomb::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
	Is_hit = true;
}

//削除判定通知処理
bool Bomb::Delete()
{
	bool ret = false;

	return ret;
}

//プレイヤーのポインタを受け取る
void Bomb::SetPlayer(Player* player)
{
	this->player = player;
}

//移動処理
void Bomb::Movement()
{
	//現在の位置座標に速さを加算する
	location += vector;
}
