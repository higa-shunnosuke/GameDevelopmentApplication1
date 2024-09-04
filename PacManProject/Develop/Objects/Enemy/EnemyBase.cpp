#include "EnemyBase.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include "Ghost/Blinky.h"
#include "Ghost/Pinky.h"
#include "Ghost/Inky.h"
#include "Ghost/Clyde.h"
#include "../../Utility/InputManager.h"

EnemyBase::EnemyBase():
	adjacent_panel(),
	now_panel(),
	move_animation(),
	eyes_animation(),
	animation_time(0.0f),
	animation_count(1),
	time(0.0f),
	flash_count(0),is_flash(false),
	enemy_type(eEnemyType::blinky),
	velocity(0.0f),
	enemy_state(eEnemyState::WAIT),
	direction_state(eDirectionState::DOWN),
	player(nullptr),
	i(0),
	enemy(nullptr),
	x(0),y(0),
	go_x(-1),go_y(-1),
	dot_counter(0),
	dot_limit(0),
	is_speed_down(false),
	is_speed_up(false),
	is_turn(false),
	speed(0)
{

}

EnemyBase::~EnemyBase()
{

}

//初期化処理
void EnemyBase::Initialize()
{
	//ステージデータの読み込み

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

	//初期進行方向の設定
	SetDirection(eDirectionState::UP);
}

//更新処理
void EnemyBase::Update(float delta_second)
{
	//パネルの確認
	StageData::ConvertToIndex(location, y, x);

	//エネミー状態変更処理
	ChangeState();

	//移動量変更処理
	SetVelocity();

	//移動処理
	Movement(delta_second);

	//アニメーション制御
	AnimationControl(delta_second);
}

//更新処理
void EnemyBase::Draw(const Vector2D& screen_offset) const
{
	//ぼでーの描画
	if (enemy_state != eEnemyState::ESCAPE)
	{
		// 親クラスの描画処理を呼び出す
		__super::Draw(screen_offset);
	}

	//目の描画
	if (enemy_state != eEnemyState::FRIGHTENED)
	{
		// オフセット値を基に画像の描画を行う
		Vector2D graph_location = this->location + screen_offset;
		DrawRotaGraphF(graph_location.x, graph_location.y, 1.1, 0.0, eyes_animation[direction_state], TRUE);
	}
	
#if _DEBUG

	//目標パネルの座標
	int py = (int)((go_y + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f + D_OBJECT_SIZE * 3.0f);
	int px = (int)((go_x + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);

	//現在パネルと状態と目標パネルの描画
	switch (enemy_type)
	{
	case EnemyBase::blinky:
		DrawFormatString(100, 40, 0xff0000, "%2d,%2d,%d", x,y, enemy_state);
		DrawCircle(px, py, 10, 0xff0000, TRUE);
		break;
	case EnemyBase::pinky:
		DrawFormatString(245, 40, 0xffa0ff, "%2d,%2d,%d", x,y, enemy_state);
		DrawCircle(px, py, 10, 0xffa0ff, TRUE);
		break;
	case EnemyBase::inky:
		DrawFormatString(390, 40, 0x00ffff, "%2d,%2d,%d", x,y, enemy_state);
		DrawCircle(px, py, 10, 0x00ffff, TRUE);
		break;
	case EnemyBase::clyde:
		DrawFormatString(535, 40, 0xffa000, "%2d,%2d,%d", x,y, enemy_state);
		DrawCircle(px, py, 10, 0xffa000, TRUE);
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
	case 14:
		//ピンキー
		enemy_type = eEnemyType::pinky;
		enemy = dynamic_cast<EnemyBase*>(new Pinky());
		enemy->Initialize();
		z_layer = 7;
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
	default:
		break;
	}

	//初期画像設定
	image = move_animation[enemy_type * 2];
}

/// <summary>
/// エネミー状態変更処理
/// </summary>
void EnemyBase::ChangeState()
{	
	//待機状態を解除する
	if (enemy_state == eEnemyState::WAIT)
	{
		switch (enemy_type)
		{
		case EnemyBase::blinky:
			SetDestination();
			if (x == 13 && y == 11)
			{
				SetDirection(eDirectionState::LEFT);
			}
			break;
		case EnemyBase::pinky:
			if (dot_limit >= 0)
			{
				SetDestination();
			}
			break;
		case EnemyBase::inky:
			if (0 < player->GetFoodCount() - dot_counter)
			{
				dot_counter = player->GetFoodCount();

				if (dot_limit >= 29)
				{
					SetDestination();
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
					SetDestination();
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
	}

	//いじけ状態にする
	if (player->GetPowerUp() == true && enemy_state != eEnemyState::ESCAPE && enemy_state != eEnemyState::WAIT)
	{
		if (enemy_state != eEnemyState::FRIGHTENED)
		{
			image = move_animation[16];
		}
		enemy_state = eEnemyState::FRIGHTENED;
	}

	// 入力状態の取得
	InputManager* input = InputManager::GetInstance();

	//点滅フラグ処理
	if (input->GetKeyDown(KEY_INPUT_SPACE))
	{
		if (enemy_state == eEnemyState::FRIGHTENED)
		{
			if (is_flash == false)
			{
				is_flash = true;
			}
			else
			{
				is_flash = false;
			}
		}
		else if(enemy_state == eEnemyState::SCATTER)
		{
			enemy_state = eEnemyState::CHASE;
		}
		else if (enemy_state == eEnemyState::CHASE)
		{
			enemy_state = eEnemyState::SCATTER;
		}
	}

	//いじけ状態から回復する
	if (flash_count > 7)
	{
		image = move_animation[enemy_type * 2];
		enemy_state = eEnemyState::SCATTER;
		player->SetPowerDown();
		is_flash = false;
		flash_count = 0;
	}
}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void EnemyBase::OnHitCollision(GameObjectBase* hit_object)
{
	//いじけ状態のとき
	if (enemy_state == eEnemyState::FRIGHTENED)
	{
		// 当たったオブジェクトがプレイヤーだったら
		if (hit_object->GetCollision().object_type == eObjectType::player)
		{
			is_speed_up = true;
			enemy_state = eEnemyState::ESCAPE;
			SetDestination();
		}
	}

}

/// <summary>
/// 移動量設定処理
/// </summary>
void EnemyBase::SetVelocity()
{

	if (is_speed_down == true)
	{
		speed = 1.0f * 0.7f;
	}
	else if (is_speed_up == true)
	{
		speed = 1.0f * 3.0f;
	}
	else
	{
		speed = 1.0f;
	}
}

/// <summary>
/// 方向設定処理
/// </summary>
void EnemyBase::SetDirection(eDirectionState direction)
{
	//現在の座標（int）
	int ly = (int)location.y;
	int lx = (int)location.x;

	//現在パネルの中心座標（int）
	int py = (int)((y + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);
	int px = (int)((x + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);

	//直角に曲がる処理
	switch (direction)
	{
	case EnemyBase::UP:
		if (lx == px)
		{
			direction_state = eDirectionState::UP;
		}
		break;
	case EnemyBase::RIGHT:
		if (ly == py)
		{
			direction_state = eDirectionState::RIGHT;
		}
		break;
	case EnemyBase::DOWN:
		if (lx == px)
		{
			direction_state = eDirectionState::DOWN;
		}
		break;
	case EnemyBase::LEFT:
		if (ly == py)
		{
			direction_state = eDirectionState::LEFT;
		}
		break;
	default:
		break;
	}
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void EnemyBase::Movement(float delta_second)
{
	eAdjacentDirection direction[4];//隣接するパネル情報を確認する用の変数
	eAdjacentDirection back = eAdjacentDirection::DOWN;		//後ろの方向を保存する用の変数
	int x[4], y[4];					//隣接するパネルの添え字保存用変数
	int l = 999;					//参照するパネルから目標パネルまでの２点間の距離
	eDirectionState next_direction;	//次に進む方向
	unsigned char old_panel;		//前フレームのパネル

	// エネミー状態によって、動作を変える
	switch (enemy_state)
	{
	case WAIT:
		WaitMovement();
		break;
	case SCATTER:
		ScatterMovement();
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

	//後ろの方向を設定
	switch (direction_state)
	{
	case EnemyBase::UP:
		back = eAdjacentDirection::DOWN;
		break;
	case EnemyBase::RIGHT:
		back = eAdjacentDirection::LEFT;
		break;
	case EnemyBase::DOWN:
		back = eAdjacentDirection::UP;
		break;
	case EnemyBase::LEFT:
		back = eAdjacentDirection::RIGHT;
		break;
	default:
		break;
	}

	//方向を配列に変換 & 隣接するパネルの位置を保存
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			direction[i] = eAdjacentDirection::RIGHT;
			x[i] = this->x + 1;
			y[i] = this->y;
			break;
		case 1:
			direction[i] = eAdjacentDirection::DOWN;
			x[i] = this->x;
			y[i] = this->y + 1;
			break;
		case 2:
			direction[i] = eAdjacentDirection::LEFT;
			x[i] = this->x - 1;
			y[i] = this->y;
			break;
		case 3:
			direction[i] = eAdjacentDirection::UP;
			x[i] = this->x;
			y[i] = this->y - 1;
			break;
		default:
			break;
		}
	}

	//隣接するパネル情報を取得
	adjacent_panel = StageData::GetAdjacentPanelData(location);

	//現在パネルの更新
	old_panel = now_panel;
	now_panel = StageData::GetPanelData(location);

	//パネルを移動したらターンフラグを初期化する
	if (old_panel != now_panel)
	{
		is_turn = false;
	}

	if (now_panel == ePanelID::BRANCH)
	{
		for (int i = 0; i < 4; i++)
		{
			//後ろに行けないようにする
			if (direction[i] == back)
			{
				continue;
			}
			else
			{
				//隣接するパネルが壁でないなら
				if (adjacent_panel[direction[i]] != ePanelID::WALL)
				{
					//目標パネルまでの距離が１番短い方向を保存する
					if (l >= (((go_x - x[i]) * (go_x - x[i])) + ((go_y - y[i]) * (go_y - y[i]))))
					{
						//距離を更新
						l = (((go_x - x[i]) * (go_x - x[i])) + ((go_y - y[i]) * (go_y - y[i])));

						//方向を更新
						switch (i)
						{
						case 0:
							next_direction = eDirectionState::RIGHT;
							break;
						case 1:
							next_direction = eDirectionState::DOWN;
							break;
						case 2:
							next_direction = eDirectionState::LEFT;
							break;
						case 3:
							next_direction = eDirectionState::UP;
							break;
						default:
							break;
						}
					}
				}
			}
		}

		//移動方向を変更する
		if (is_turn == false)
		{
			SetDirection(next_direction);
		}

		//BRANCHの中では１回まで曲がれるようにする
		if (next_direction == direction_state)
		{
			is_turn = true;
		}
	}

	switch (direction_state)
	{
	case EnemyBase::UP:
		velocity.x = 0.0f;
		velocity.y = -speed;
		break;
	case EnemyBase::RIGHT:
		velocity.x = speed;
		velocity.y = 0.0f;
		break;
	case EnemyBase::DOWN:
		velocity.x = 0.0f;
		velocity.y = speed;
		break;
	case EnemyBase::LEFT:
		velocity.x = -speed;
		velocity.y = 0.0f;
		break;
	default:
		break;
	}

	// 移動量 * 速さ * 時間 で移動先を決定する
	location += velocity * 100.0f * delta_second;

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

/// <summary>
/// 移動処理
/// </summary>
void EnemyBase::WaitMovement()
{
	//目的地が決まっていないとき
	if (go_x == -1 && go_y == -1)
	{
		adjacent_panel = StageData::GetAdjacentPanelData(location);

		//待機処理
		if (adjacent_panel[eAdjacentDirection::UP] == ePanelID::WALL)
		{
			SetDirection(eDirectionState::DOWN);
		}
		else if (adjacent_panel[eAdjacentDirection::DOWN] == ePanelID::WALL)
		{
			SetDirection(eDirectionState::UP);
		}
	}
	//出口に向かう
	else
	{
		//横方向処理
		if (go_x - x > 0)
		{
			SetDirection(eDirectionState::RIGHT);
		}
		else if (go_x - x < 0)
		{
			SetDirection(eDirectionState::LEFT);
		}
		else
		{
			//縦方向処理
			if (go_y - y < 0)
			{
				SetDirection(eDirectionState::UP);
				//減速させる
				is_speed_down = true;
			}
			else
			{
				is_speed_down = false;
				SetDirection(eDirectionState::LEFT);
				if (direction_state == eDirectionState::LEFT)
				{
					enemy_state = eEnemyState::SCATTER;
				}
			}
		}
	}
}

/// <summary>
/// 移動処理
/// </summary>
void EnemyBase::ScatterMovement()
{
	//縄張りを設定
	SetDestination();
}

/// <summary>
/// 移動処理
/// </summary>
void EnemyBase::TrackingMovement()
{
	//各エネミーの追跡処理
	enemy->TrackingMovement();
}

/// <summary>
/// 移動処理
/// </summary>
void EnemyBase::ScaredMovement()
{

}

/// <summary>
/// 移動処理
/// </summary>
void EnemyBase::EscapeMovement()
{
	std::map<eAdjacentDirection, ePanelID> adjacent_panel;	//隣接するパネルのデータ
	int x, y;												//現在座標の添え字

	//隣接するパネル情報を取得
	adjacent_panel = StageData::GetAdjacentPanelData(location);
	//現在座標の添え字を取得
	StageData::ConvertToIndex(location, y, x);

	//ゲートの上まで来たら下に行く
	if (adjacent_panel[eAdjacentDirection::DOWN] == ePanelID::GATE)
	{
		SetDirection(eDirectionState::DOWN);
	}

	if (y == 11 && x == 13)
	{
		SetDirection(eDirectionState::DOWN);
	}

	//巣の中に入ったら待機状態にする
	if (y == 15 && x == 13)
	{
		is_speed_up = false;
		enemy_state = eEnemyState::WAIT;
	}
}

/// <summary>
/// 目標設定処理
/// </summary>
void EnemyBase::SetDestination()
{
	switch (enemy_state)
	{
	case WAIT:
		go_x = 13;
		go_y = 11;
		break;
	case SCATTER:
		switch (enemy_type)
		{
		case EnemyBase::blinky:
			go_x = 26;
			go_y = 0;
			break;
		case EnemyBase::pinky:
			go_x = 1;
			go_y = 0;
			break;
		case EnemyBase::inky:
			go_x = 26;
			go_y = 30;
			break;
		case EnemyBase::clyde:
			go_x = 1;
			go_y = 30;
			break;
		}
		break;
	case ESCAPE:
		go_x = 13;
		go_y = 11;
		break;
	default:
		break;
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

		//画像の設定
		//死んだときはアニメーションしない
		if (enemy_state != eEnemyState::ESCAPE)
		{
			//いじけ状態のとき
			if (enemy_state == eEnemyState::FRIGHTENED)
			{
				//点滅アニメーション
				if (is_flash == true)
				{
					if (image == move_animation[16])
					{
						image = move_animation[19];
						flash_count++;
					}
					else if (image == move_animation[19])
					{
						image = move_animation[16];
					}
					else if (image == move_animation[17])
					{
						image = move_animation[18];
						flash_count++;
					}
					else if (image == move_animation[18])
					{
						image = move_animation[17];
					}
				}
				else
				{
					//いじけアニメーション
					image = move_animation[16 + animation_count];
				}
			}
			//いじけ状態ではないとき
			else
			{
				image = move_animation[enemy_type * 2 + animation_count];
			}
		}
	}
}