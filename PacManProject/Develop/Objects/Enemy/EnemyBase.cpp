#include "EnemyBase.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "../../Utility/InputManager.h"

EnemyBase::EnemyBase():
	move_animation(),
	eyes_animation(),
	animation_time(0.0f),
	animation_count(1),
	time(0.0f),
	flash_count(0),
	enemy_type(eEnemyType::blinky),
	velocity(0.0f),
	enemy_state(eEnemyState::WAIT),
	direction_state(eDirectionState::LEFT),
	player(nullptr),
	i(0)
{

}

EnemyBase::~EnemyBase()
{

}

//初期化処理
void EnemyBase::Initialize()
{
	// アニメーション画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	move_animation = rm->GetImages("Resource/Images/monster.png", 20, 20, 1, 32, 32);
	eyes_animation = rm->GetImages("Resource/Images/eyes.png", 4, 4, 1, 32, 32);

	// 当たり判定の設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::enemy;
	collision.hit_object_type.push_back(eObjectType::player);
	collision.hit_object_type.push_back(eObjectType::wall);
	collision.radius = (D_OBJECT_SIZE - 1.0f) / 2.0f;

	// 可動性の設定
	mobility = eMobilityType::Movable;
}

//更新処理
void EnemyBase::Update(float delta_second)
{
	//移動処理
	Movement(delta_second);

	// アニメーション制御
	AnimationControl(delta_second);

	/// エネミー状態変更処理
	ChangeState();
}

//更新処理
void EnemyBase::Draw(const Vector2D& screen_offset) const
{
	if (enemy_state != eEnemyState::ESCAPE)
	{
		// 親クラスの描画処理を呼び出す
		__super::Draw(screen_offset);
	}

	if (enemy_state != eEnemyState::FRIGHTENED)
	{
		// オフセット値を基に画像の描画を行う
		Vector2D graph_location = this->location + screen_offset;
		DrawRotaGraphF(graph_location.x, graph_location.y, 1.0, 0.0, eyes_animation[direction_state], TRUE);
	}
	
	switch (enemy_type)
	{
	case EnemyBase::blinky:
		DrawFormatString(10, 50, 0xff0000, "%d", i);
		break;
	case EnemyBase::pinky:
		DrawFormatString(10, 70, 0xffa0ff, "%d", i);
		break;
	case EnemyBase::inky:
		DrawFormatString(10, 90, 0x00ffff, "%d", i);
		break;
	case EnemyBase::clyde:
		DrawFormatString(10, 110, 0xffff00, "%d", i);
		break;
	}

	//状態描画処理
	DrawFormatString(10, 130, 0xffffff, "%d", enemy_state);
}

//終了時処理
void EnemyBase::Finalize()
{
	// 動的配列の解放
	move_animation.clear();
}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void EnemyBase::OnHitCollision(GameObjectBase* hit_object)
{
	if (enemy_state != eEnemyState::WAIT && enemy_state != eEnemyState::ESCAPE)
	{
		// 当たった、オブジェクトが壁だったら
		if (hit_object->GetCollision().object_type == eObjectType::wall)
		{
			// 当たり判定情報を取得して、カプセルがある位置を求める
			CapsuleCollision hc = hit_object->GetCollision();
			hc.point[0] += hit_object->GetLocation();
			hc.point[1] += hit_object->GetLocation();

			// 最近傍点を求める
			Vector2D near_point = NearPointCheck(hc, this->location);

			// Playerからnear_pointへの方向ベクトルを取得
			Vector2D dv2 = near_point - this->location;
			Vector2D dv = this->location - near_point;

			// めり込んだ差分
			float diff = (this->GetCollision().radius + hc.radius) - dv.Length();

			// diffの分だけ戻る
			location += dv.Normalize() * diff;
		}
	}

	//いじけ状態のとき
	if (enemy_state == eEnemyState::FRIGHTENED)
	{
		// 当たったオブジェクトがプレイヤーだったら
		if (hit_object->GetCollision().object_type == eObjectType::player)
		{
			enemy_state = eEnemyState::ESCAPE;
		}
	}
	
}

/// <summary>
/// エネミーの状態を取得する
/// </summary>
/// <returns>エネミーの状態</returns>
eEnemyState EnemyBase::GetEnemyState() const
{
	return enemy_state;
}

/// <summary>
/// プレイヤーのポインタ受け取り処理
/// </summary>
/// <param name="player">プレイヤーのポインタ</param>
void EnemyBase::SetPlayer(Player* player)
{
	this->player = player;
}

/// <summary>
/// エネミー種類設定処理
/// </summary>
/// <param name="type">エネミーの種類</param>
void EnemyBase::SetType(int type)
{
	switch (type)
	{
	case 0:
		//アカベイ
		enemy_type = eEnemyType::blinky;
		// レイヤーの設定
		z_layer = 6;

		break;
	case 1:
		//アオスケ
		enemy_type = eEnemyType::inky;
		z_layer = 8;

		break;
	case 2:
		//グズタ
		enemy_type = eEnemyType::clyde;
		z_layer = 7;

		break;
	case 3:
		//ピンキー
		enemy_type = eEnemyType::pinky;
		z_layer = 9;

		break;
	default:
		break;
	}
}

/// <summary>
/// エネミー状態変更処理
/// </summary>
void EnemyBase::ChangeState()
{
	if (player->GetPowerUp() == true && enemy_state != eEnemyState::ESCAPE)
	{
		enemy_state = eEnemyState::FRIGHTENED;
	}
}

/// <summary>
/// 持ち時間制御
/// </summary>
void EnemyBase::TimeControl()
{

}

/// <summary>
/// アニメーション制御
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::AnimationControl(float delta_second)
{
	// 移動中のアニメーション
	animation_time += delta_second;
	if (animation_time >= (0.1f))
	{
		animation_time = 0.0f;
		animation_count++;

		if (animation_count >= 2)
		{
			animation_count = 0;
		}
		// 画像の設定
		if (enemy_state != eEnemyState::ESCAPE)
		{
			//いじけ状態のとき
			if (enemy_state == eEnemyState::FRIGHTENED)
			{
				image = move_animation[16 + animation_count];
			}
			//いじけ状態ではないとき
			else
			{
				image = move_animation[enemy_type * 2 + animation_count];
			}
		}
	
	}
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::Movement(float delta_second)
{
	// エネミー状態によって、動作を変える
	switch (enemy_state)
	{
	case WAIT:
		WaitMovement(delta_second);
		break;
	case SCATTER:
		TerritoryMovement(delta_second);
		break;
	case CHASE:
		TrackingMovement(delta_second);
		break;
	case FRIGHTENED:
		ScaredMovement(delta_second);
		break;
	case ESCAPE:
		EscapeMovement(delta_second);
		break;
	default:
		break;
	}

	// 入力状態の取得
	InputManager* input = InputManager::GetInstance();

	if (input->GetKeyDown(KEY_INPUT_SPACE))
	{
		if (i < 4)
		{
			i++;
		}
		else
		{
			i = 0;
		}
	}


	if (enemy_type == i)
	{	
		if (input->GetKeyDown(KEY_INPUT_W))
		{
			direction_state = eDirectionState::UP;
		}
		else if (input->GetKeyDown(KEY_INPUT_S))
		{
			direction_state = eDirectionState::DOWN;
		}
		else if (input->GetKeyDown(KEY_INPUT_A))
		{
			direction_state = eDirectionState::LEFT;
		}
		else if (input->GetKeyDown(KEY_INPUT_D))
		{
			direction_state = eDirectionState::RIGHT;
		}

		// 進行方向の移動量を追加
		switch (direction_state)
		{
		case eDirectionState::UP:
			velocity.y = -1.0f;
			velocity.x = 0.0f;
			break;
		case eDirectionState::DOWN:
			velocity.y = 1.0f;
			velocity.x = 0.0f;
			break;
		case eDirectionState::LEFT:
			velocity.x = -1.0f;
			velocity.y = 0.0f;
			break;
		case eDirectionState::RIGHT:
			velocity.x = 1.0f;
			velocity.y = 0.0f;
			break;
		}

		// 移動量 * 速さ * 時間 で移動先を決定する
		location += velocity * 50.0f * delta_second;

		// 画面外に行ったら、反対側にワープさせる
		if (location.x < 0.0f)
		{
			location.x = 672.0f - collision.radius;
			velocity.y = 0.0f;
		}
		if (672.0f < location.x)
		{
			location.x = collision.radius;
			velocity.y = 0.0f;
		}
	}
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::WaitMovement(float delta_second)
{
	

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::TerritoryMovement(float delta_second)
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::TrackingMovement(float delta_second)
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::ScaredMovement(float delta_second)
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::EscapeMovement(float delta_second)
{

}

/// <summary>
/// 目標設定処理
/// </summary>
void EnemyBase::SetDestination()
{

}