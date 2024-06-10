#include"Player.h"
#include"../../Utility/InputControl.h"
#include"DxLib.h"

//コンストラクタ
Player::Player():frame_count(0),flip_flag(FALSE),direction(0)
{
	animation[0] = NULL;
	animation[1] = NULL;
	Is_hit = false;
}

//デストラクタ
Player::~Player()
{}

//初期化処理
void Player::Initialize(int object_type)
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Tri-pilot/1.png");
	animation[1] = LoadGraph("Resource/Images/Tri-pilot/2.png");

	//エラーチェック
	if (animation[0]==-1||animation[1]==-1)
	{
		throw ("トリパイロットの画像がありません\n");
	}

	//向きの設定
	radian = 0.0;

	//大きさの設定
	box_size = 64.0f;

	//初期画像の設定
	image = animation[0];

	//オブジェクトタイプの設定
	type = object_type;
}

//更新処理
void Player::Update()
{
	//移動処理
	Movement();
	//アニメーション制御
	AnimeControl();

}

//描画処理
void Player::Draw() const
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, flip_flag);

	__super::Draw();
}

//終了時処理
void Player::Finalize()
{
	//使用した画像を開放する
	DeleteGraph(animation[0]);
	DeleteGraph(animation[1]);
}

//当たり判定通知処理
void Player::OnHitCollision(GameObject* hit_object)
{
	//当たった時の処理
}

//位置情報取得処理
Vector2D Player::GetLocation() const
{
	return this->location;
}

//位置情報取得処理
int Player::GetDirection() const
{
	return this->direction;
}

//タイプ取得処理
int Player::GetType()
{
	return this->type;
}

//移動処理
void Player::Movement()
{
	//移動の速さ
	Vector2D vector = 0.0f;

	//左右移動
	if (InputControl::GetKey(KEY_INPUT_LEFT))
	{
		vector.x += -2.0f;
		flip_flag = TRUE;
		direction = -1;
	}
	else if (InputControl::GetKey(KEY_INPUT_RIGHT))
	{
		vector.x += 2.0f;
		flip_flag = FALSE;
		direction = 1;
	}
	else
	{
		vector.x += 0.0f;
		direction = 0;
	}
	
	//壁の処理
	if (location.x < box_size.x / 2.0f)
	{
		vector.x += 2.0f;
	}
	if (location.x > 640.0f - box_size.x / 2.0f)
	{
		vector.x -= 2.0f;
	}
	
	//上下移動
	if (InputControl::GetKey(KEY_INPUT_UP))
	{
		vector.y += -2.0f;
	}
	else if (InputControl::GetKey(KEY_INPUT_DOWN))
	{
		vector.y += 2.0f;
	}
	else
	{
		vector.y += 0.0f;
	}
	
	//現在の位置座標を更新する
	location += vector;
}

//アニメーション制御
void Player::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count>=60)
	{
		//カウントのリセット
		frame_count = 0;

		//画像の切り替え
		if (image==animation[0])
		{
			image = animation[1];
		}
		else
		{
			image = animation[0];
		}
	}
}
