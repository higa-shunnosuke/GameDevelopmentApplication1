#include"Player.h"
#include"../../Utility/InputControl.h"
#include"DxLib.h"

//コンストラクタ
Player::Player() :frame_count(0), filp_flag(FALSE)
{
	animation[0] = NULL;
	animation[1] = NULL;
}

//デストラクタ
Player::~Player()
{}

//初期化処理
void Player::Initialize()
{
	//画像の読み込み
	animation[0] = LoadGraph("Resource/Images/Tri-pilot1.png");
	animation[1] = LoadGraph("Resource/Images/Tri-pilot2.png");

	//エラーチェック
	if (animation[0] == -1 || animation[1] == -1)
	{
		throw ("トリパイロットの画像がありません\n");
	}

	//向きの設定
	radian = 0.0;

	//大きさの設定
	box_size = Vector2D(64.0f);

	//初期画像の設定
	image = animation[0];
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
	DrawRotaGraphF(location.x, location.y, 1.0, radian, image, TRUE, filp_flag);

	//デバック用
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

//移動処理
void Player::Movement()
{
	//移動の速さ
	Vector2D vector = 0.0f;

	//左右移動
	if (InputControl::GetKey(KEY_INPUT_LEFT))
	{
		vector.x += -1.0f;
		filp_flag = TRUE;
	}
	else if (InputControl::GetKey(KEY_INPUT_RIGHT))
	{
		vector.x += 1.0f;
		filp_flag = FALSE;
	}
	else
	{
		vector.x += 0.0f;
	}
	if (InputControl::GetKey(KEY_INPUT_UP))
	{
		vector.y += -1.0f;
	}
	else if (InputControl::GetKey(KEY_INPUT_DOWN))
	{
		vector.y += 1.0f;
	}
	else
	{
		vector.y += 0.0f;
	}

	//現在の位置座標に速さを加算する
	location += vector;
}

//アニメーション制御
void Player::AnimeControl()
{
	//フレームカウントを加算する
	frame_count++;

	//６０フレーム目に到達したら
	if (frame_count >= 60)
	{
		//カウントのリセット
		frame_count = 0;

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
