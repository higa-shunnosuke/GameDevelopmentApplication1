#include "Player.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include "../Food/Food.h"

#define D_PLAYER_SPEED	(50.0f)

Player::Player() :
	move_animation(),
	dying_animation(),
	velocity(0.0f),
	player_state(ePlayerState::MOVE),
	now_direction_state(eDirectionState::LEFT),
	next_direction_state(eDirectionState::LEFT),
	food_count(0),
	animation_time(0.0f),
	animation_count(0),
	old_panel(ePanelID::NONE),
	is_power_up(false),
	is_destroy(false)
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	// アニメーション画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	move_animation = rm->GetImages("Resource/Images/pacman.png", 12, 12, 1, 32, 32);
	dying_animation = rm->GetImages("Resource/Images/dying.png", 11, 11, 1, 32, 32);

	// 当たり判定の設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::player;
	collision.hit_object_type.push_back(eObjectType::enemy);
	collision.hit_object_type.push_back(eObjectType::wall);
	collision.hit_object_type.push_back(eObjectType::food);
	collision.hit_object_type.push_back(eObjectType::power_food);
	collision.hit_object_type.push_back(eObjectType::special);
	collision.radius = (D_OBJECT_SIZE - 1.0f) / 2.0f;

	// レイヤーの設定
	z_layer = 5;

	// 可動性の設定
	mobility = eMobilityType::Movable;
}

void Player::Update(float delta_second)
{
	// プレイヤー状態によって、動作を変える
	switch(player_state)
	{
		case ePlayerState::IDLE:
			// 画像の設定
			image = move_animation[9];
			break;
		case ePlayerState::MOVE:
			// 移動処理
			Movement(delta_second);
			// アニメーション制御
			AnimationControl(delta_second);
			break;
		case ePlayerState::DIE:
			// 死亡中のアニメーション
			animation_time += delta_second;
			if(animation_time >= 0.07f)
			{
				animation_time = 0.0f;
				animation_count++;
				// 復活させる
				if(animation_count >= dying_animation.size())
				{
					player_state = ePlayerState::IDLE;
					animation_count = 0;
					is_destroy = true;
				}
			}
			image = dying_animation[animation_count];
			break;
		default:
			break;
	}
}

void Player::Draw(const Vector2D& screen_offset) const
{
	// 親クラスの描画処理を呼び出す
	__super::Draw(screen_offset);
}

void Player::Finalize()
{
	// 動的配列の解放
	move_animation.clear();
	dying_animation.clear();
}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void Player::OnHitCollision(GameObjectBase* hit_object)
{
	// 当たった、オブジェクトが壁だったら
	if(hit_object->GetCollision().object_type == eObjectType::wall)
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

	// 当たった、オブジェクトが通常餌だったら
	if(hit_object->GetCollision().object_type == eObjectType::food)
	{
		food_count++;
	}

	// 当たった、オブジェクトがパワー餌だったら
	if(hit_object->GetCollision().object_type == eObjectType::power_food)
	{
		food_count++;
		is_power_up = true;
	}

	// 当たったオブジェクトが敵だったら
	if(hit_object->GetCollision().object_type == eObjectType::enemy)
	{
		player_state = ePlayerState::DIE;
	}
}

/// <summary>
/// 餌を食べた数取得
/// </summary>
/// <returns>餌を食べた数</returns>
int Player::GetFoodCount() const
{
	return food_count;
}

/// <summary>
/// プレイヤーの状態を取得する
/// </summary>
/// <returns>プレイヤーの状態</returns>
ePlayerState Player::GetPlayerState() const
{
	return player_state;
}

/// <summary>
/// プレイヤーがパワーアップしてるか確認する
/// </summary>
/// <returns>プレイヤーの状態</returns>
bool Player::GetPowerUp() const
{
	return is_power_up;
}

/// <summary>
/// パワーダウンさせる
/// </summary>
void Player::SetPowerDown()
{
	is_power_up = false;
}

bool Player::GetDestroy() const
{
	return is_destroy;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void Player::Movement(float delta_second)
{
	// 移動量から移動方向を更新
	if(Vector2D::Distance(old_location, location) == 0.0f)
	{
		// 移動がなければ direction_state を変更する
		velocity = 0.0f;
		now_direction_state = next_direction_state;
		next_direction_state = eDirectionState::NONE;
	}
	else
	{
		// 移動方向に移動していなければ direction_state を変更する
		switch(now_direction_state)
		{
			case eDirectionState::UP:
			case eDirectionState::DOWN:
			{
				float diff = location.y - old_location.y;
				if(((now_direction_state == eDirectionState::UP) && (diff < 0.0f)) ||
				   ((now_direction_state == eDirectionState::DOWN) && (0.0f < diff)))
				{
					// 移動方向に移動してるので break
					break;
				}

				velocity.y = 0.0f;
				now_direction_state = next_direction_state;
				next_direction_state = eDirectionState::NONE;
			}
			break;

			case eDirectionState::LEFT:
			case eDirectionState::RIGHT:
			{

				float diff = location.x - old_location.x;
				if(((now_direction_state == eDirectionState::LEFT) && (diff < 0.0f)) ||
				   ((now_direction_state == eDirectionState::RIGHT) && (0.0f < diff)))
				{
					// 移動方向に移動してるので break
					break;
				}

				velocity.x = 0.0f;
				now_direction_state = next_direction_state;
				next_direction_state = eDirectionState::NONE;
			}
			break;

			default:// 何もしない
				break;
		}
	}

	// 入力状態の取得
	InputManager* input = InputManager::GetInstance();

	// 現在パネルの状態を確認
	ePanelID panel = StageData::GetPanelData(location);

	// 入力から移動方向を設定
	if(input->GetKeyDown(KEY_INPUT_UP) || input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		switch(now_direction_state)
		{
			case eDirectionState::UP:
				old_panel = ePanelID::NONE;
			case eDirectionState::DOWN:
			case eDirectionState::NONE:
				now_direction_state = eDirectionState::UP;
				break;

			default:
				next_direction_state = eDirectionState::UP;
		}	
	}
	else if(input->GetKeyDown(KEY_INPUT_DOWN) || input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		switch(now_direction_state)
		{
			case eDirectionState::DOWN:
				old_panel = ePanelID::NONE;
			case eDirectionState::UP:
			case eDirectionState::NONE:
				now_direction_state = eDirectionState::DOWN;
				break;

			default:
				next_direction_state = eDirectionState::DOWN;
		}
	}
	else if(input->GetKeyDown(KEY_INPUT_LEFT) || input->GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		switch(now_direction_state)
		{
			case eDirectionState::LEFT:
				old_panel = ePanelID::NONE;
			case eDirectionState::RIGHT:
			case eDirectionState::NONE:
				now_direction_state = eDirectionState::LEFT;
				break;

			default:
				next_direction_state = eDirectionState::LEFT;
		}
	}
	else if(input->GetKeyDown(KEY_INPUT_RIGHT) || input->GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
	{
		switch(now_direction_state)
		{
			case eDirectionState::RIGHT:
				old_panel = ePanelID::NONE;
			case eDirectionState::LEFT:
			case eDirectionState::NONE:
				now_direction_state = eDirectionState::RIGHT;
				break;

			default:
				next_direction_state = eDirectionState::RIGHT;
		}
	}

	// 進行方向の移動量を追加
	switch(now_direction_state)
	{
		case Player::UP:
			velocity.y = -1.0f;
			break;
		case Player::DOWN:
			velocity.y = 1.0f;
			break;
		case Player::LEFT:
			velocity.x = -1.0f;
			break;
		case Player::RIGHT:
			velocity.x = 1.0f;
			break;
		default:
			velocity = 0.0f;
			now_direction_state = next_direction_state;
			next_direction_state = Player::NONE;
			break;
	}

	// 先行入力の移動量を追加
	if((panel != ePanelID::NONE)
	   && (old_panel != panel))
	{
		switch(next_direction_state)
		{
			case Player::UP:
				velocity.y = -1.0f;
				break;
			case Player::RIGHT:
				velocity.x = 1.0f;
				break;
			case Player::DOWN:
				velocity.y = 1.0f;
				break;
			case Player::LEFT:
				velocity.x = -1.0f;
				break;
			default:
				break;
		}
	}

	// 前回座標の更新
	old_location = location;

	// 前回パネルの更新
	old_panel = panel;
	
	// 移動量 * 速さ * 時間 で移動先を決定する
	location += velocity * D_PLAYER_SPEED * delta_second;

	// 画面外に行ったら、反対側にワープさせる
	if(location.x < 0.0f)
	{
		old_location.x = 672.0f;
		location.x = 672.0f - collision.radius;
		velocity.y = 0.0f;
	}
	if(672.0f < location.x)
	{
		old_location.x = 0.0f;
		location.x = collision.radius;
		velocity.y = 0.0f;
	}
}

/// <summary>
/// アニメーション制御
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void Player::AnimationControl(float delta_second)
{
	// 移動中のアニメーション
	animation_time += delta_second;
	if (animation_time >= (1.0f / 16.0f))
	{
		animation_time = 0.0f;
		animation_count++;
		if (animation_count >= 4)
		{
			animation_count = 0;
		}
		// 画像の設定
		int dir_num = (int)now_direction_state;
		if (0 <= dir_num && dir_num < 4)
		{
			image = move_animation[(dir_num * 3) + animation_num[animation_count]];
		}

	}
}
