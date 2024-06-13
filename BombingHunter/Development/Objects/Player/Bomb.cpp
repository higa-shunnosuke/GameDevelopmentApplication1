#include "Bomb.h"
#include"DxLib.h"


//コンストラクタ
Bomb::Bomb() :frame_count(0), animation_max(0), count(0), vector(0.0f), Is_hit(false)
{
	animation[0] = NULL;

	player = nullptr;
	explosion = nullptr;
}

//デストラクタ
Bomb::~Bomb()
{}

//初期化処理
void Bomb::Initialize(int object_type)
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Bomb/1.png");

	//大きさの設定
	box_size = Vector2D(40.0f,60.0f);

	//エラーチェック
	if (animation[0] == -1)
	{
		throw ("ボムの画像がありません\n");
	}
	
	//初期画像の初期化
	image = animation[0];

	//オブジェクトタイプの初期化
	type = object_type;

	//向きの初期化
	radian = 0;

	//移動方向の初期化
	vector = Vector2D(0.0f, 0.0f);

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
	//ボム画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//終了時処理
void Bomb::Finalize()
{
	//使用した画像を開放する
	DeleteGraph(animation[0]);
	
}

//当たり判定通知処理
void Bomb::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
	if (hit_object->GetType() > 3)
	{
		Is_hit = true;
	}
}

//削除判定通知処理
bool Bomb::Delete()
{
	bool ret = false;

	//床にぶつかったら
	if (location.y > 350.0f + box_size.y || Is_hit == true)
	{
		ret = true;
	}

	return ret;
}

//タイプ取得処理
int Bomb::GetType()
{
	return this->type;
}

//プレイヤーのポインタを受け取る、ついでに進行方向を設定する
void Bomb::SetPlayer(Player* player)
{
	this->player = player;

	//移動方向の設定
	if (player->GetDirection() == 0)
	{
		radian = 1.5707963267949;	//90°
		vector = Vector2D(0.0f, 2.5f);
	}
	else if (player->GetDirection() == 1)
	{
		radian = 0.78539816339745;	//45°
		vector = Vector2D(3.0f);
	}
	else
	{
		radian = 2.3561944901923;	//135°
		vector = Vector2D(-3.0f, 3.0f);
	}
}

//移動処理
void Bomb::Movement()
{
	//壁に当たったら真下に落ちる
	if (location.x > 640.0f)
	{
		radian = 1.5707963267949;
		vector.x = 0.0f;
	}
	if (location.x < 0.0f)
	{
		radian = 1.5707963267949;
		vector.x = 0.0f;
	}

	////敵に当たったらその場で止まる
	if (Is_hit == true)
	{
		vector = Vector2D(0.0f);
	}

	//現在の位置座標を更新する
	location += vector;
}
