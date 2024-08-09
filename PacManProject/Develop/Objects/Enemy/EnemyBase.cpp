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
	i(0),
	enemy(nullptr),
	x(0),y(0),
	dot_counter(0),
	dot_limit(0)
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
	//パネルの確認
	StageData::ConvertToIndex(location, y, x);

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
	
#if _DEBUG

	switch (enemy_type)
	{
	case EnemyBase::blinky:
		DrawFormatString(100, 40, 0xff0000, "%2d,%2d,%d", x,y,enemy_state);
		break;
	case EnemyBase::pinky:
		DrawFormatString(245, 40, 0xffa0ff, "%2d,%2d,%d", x,y, enemy_state);
		break;
	case EnemyBase::inky:
		DrawFormatString(390, 40, 0x00ffff, "%2d,%2d,%d", x,y, enemy_state);
		break;
	case EnemyBase::clyde:
		DrawFormatString(535, 40, 0xffa000, "%2d,%2d,%d", x,y, enemy_state);
		break;
	}
#endif
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
	/*if (enemy_state != eEnemyState::WAIT && enemy_state != eEnemyState::ESCAPE)
	{*/
		// 当たった、オブジェクトが壁だったら
		if (hit_object->GetCollision().object_type == eObjectType::wall)
		{
			// 当たり判定情報を取得して、カプセルがある位置を求める
			CapsuleCollision hc = hit_object->GetCollision();
			hc.point[0] += hit_object->GetLocation();
			hc.point[1] += hit_object->GetLocation();

			// 最近傍点を求める
			Vector2D near_point = NearPointCheck(hc, this->location);

			//Enemyからnear_pointへの方向ベクトルを取得
			Vector2D dv2 = near_point - this->location;
			Vector2D dv = this->location - near_point;

			// めり込んだ差分
			float diff = (this->GetCollision().radius + hc.radius) - dv.Length();

			// diffの分だけ戻る
			location += dv.Normalize() * diff;
		}
	/*}*/

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
void EnemyBase::SetType()
{
	StageData::ConvertToIndex(location, y, x);

	switch (x)
	{
	case 13:
		//アカベイ
		enemy_type = eEnemyType::blinky;
		enemy = dynamic_cast<EnemyBase*>(new Blinky());
		enemy->Initialize();
		// レイヤーの設定
		z_layer = 6;
		break;
	case 12:
		//アオスケ
		enemy_type = eEnemyType::inky;
		enemy = dynamic_cast<EnemyBase*>(new Inky());
		enemy->Initialize();
		z_layer = 8;
		break;
	case 16:
		//グズタ
		enemy_type = eEnemyType::clyde;
		enemy = dynamic_cast<EnemyBase*>(new Clyde());
		enemy->Initialize();
		z_layer = 9;
		break;
	case 14:
		//ピンキー
		enemy_type = eEnemyType::pinky;
		enemy = dynamic_cast<EnemyBase*>(new Pinky());
		enemy->Initialize();
		z_layer = 7;
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
	//待機状態を解除する
	switch (enemy_type)
	{
	case EnemyBase::blinky:
		break;
	case EnemyBase::pinky:
		break;
	case EnemyBase::inky:
		if (0 < player->GetFoodCount() - dot_counter)
		{
			dot_counter = player->GetFoodCount();

			if (dot_limit >= 29)
			{
				enemy_state = eEnemyState::SCATTER;
				dot_limit = 0;
			}
			else
			{
				dot_limit++;
			}
		}
		break;
	case EnemyBase::clyde:
		if (0 < player->GetFoodCount() - dot_counter)
		{
			dot_counter = player->GetFoodCount();

			if (dot_limit >= 30 + 59)
			{
				enemy_state = eEnemyState::SCATTER;
				dot_limit = 0;
			}
			else
			{
				dot_limit++;
			}
		}
		break;
	default:
		break;
	}

	
	//いじけ状態にする
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
		//死んだときはアニメーションしない
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
		WaitMovement();
		break;
	case SCATTER:
		TerritoryMovement();
		break;
	case CHASE:
		TrackingMovement();
		break;
	case FRIGHTENED:
		ScaredMovement();
		break;
	case ESCAPE:
		EscapeMovement();
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
void EnemyBase::WaitMovement()
{


}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::TerritoryMovement()
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::TrackingMovement()
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::ScaredMovement()
{

}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::EscapeMovement()
{

}

/// <summary>
/// 目標設定処理
/// </summary>
void EnemyBase::SetDestination()
{

}