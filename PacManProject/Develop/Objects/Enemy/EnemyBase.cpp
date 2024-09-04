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

//����������
void EnemyBase::Initialize()
{
	//�X�e�[�W�f�[�^�̓ǂݍ���

	// �A�j���[�V�����摜�̓ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	move_animation = rm->GetImages("Resource/Images/monster.png", 20, 20, 1, 32, 32);
	eyes_animation = rm->GetImages("Resource/Images/eyes.png", 4, 4, 1, 32, 32);

	// �����蔻��̐ݒ�
	collision.is_blocking = true;
	collision.object_type = eObjectType::enemy;
	collision.hit_object_type.push_back(eObjectType::player);
	collision.hit_object_type.push_back(eObjectType::wall);
	collision.radius = (D_OBJECT_SIZE - 1.0f) / 2.0f;

	// �����̐ݒ�
	mobility = eMobilityType::Movable;

	//�����i�s�����̐ݒ�
	SetDirection(eDirectionState::UP);
}

//�X�V����
void EnemyBase::Update(float delta_second)
{
	//�p�l���̊m�F
	StageData::ConvertToIndex(location, y, x);

	//�G�l�~�[��ԕύX����
	ChangeState();

	//�ړ��ʕύX����
	SetVelocity();

	//�ړ�����
	Movement(delta_second);

	//�A�j���[�V��������
	AnimationControl(delta_second);
}

//�X�V����
void EnemyBase::Draw(const Vector2D& screen_offset) const
{
	//�ڂŁ[�̕`��
	if (enemy_state != eEnemyState::ESCAPE)
	{
		// �e�N���X�̕`�揈�����Ăяo��
		__super::Draw(screen_offset);
	}

	//�ڂ̕`��
	if (enemy_state != eEnemyState::FRIGHTENED)
	{
		// �I�t�Z�b�g�l����ɉ摜�̕`����s��
		Vector2D graph_location = this->location + screen_offset;
		DrawRotaGraphF(graph_location.x, graph_location.y, 1.1, 0.0, eyes_animation[direction_state], TRUE);
	}
	
#if _DEBUG

	//�ڕW�p�l���̍��W
	int py = (int)((go_y + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f + D_OBJECT_SIZE * 3.0f);
	int px = (int)((go_x + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);

	//���݃p�l���Ə�ԂƖڕW�p�l���̕`��
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

//�I��������
void EnemyBase::Finalize()
{
	// ���I�z��̉��
	move_animation.clear();
}

/// <summary>
/// �G�l�~�[�̏�Ԃ��擾����
/// </summary>
/// <returns>�G�l�~�[�̏��</returns>
eEnemyState EnemyBase::GetEnemyState() const
{
	return enemy_state;
}

/// <summary>
/// �v���C���[�̃|�C���^�󂯎�菈��
/// </summary>
/// <param name="player">�v���C���[�̃|�C���^</param>
void EnemyBase::SetPlayer(Player* player)
{
	this->player = player;
}

/// <summary>
/// �G�l�~�[��ސݒ菈��
/// </summary>
/// <param name="type">�G�l�~�[�̎��</param>
void EnemyBase::SetType()
{
	StageData::ConvertToIndex(location, y, x);

	switch (x)
	{
	case 13:
		//�A�J�x�C
		enemy_type = eEnemyType::blinky;
		enemy = dynamic_cast<EnemyBase*>(new Blinky());
		enemy->Initialize();
		// ���C���[�̐ݒ�
		z_layer = 6;
		break;
	case 14:
		//�s���L�[
		enemy_type = eEnemyType::pinky;
		enemy = dynamic_cast<EnemyBase*>(new Pinky());
		enemy->Initialize();
		z_layer = 7;
		break;
	case 12:
		//�A�I�X�P
		enemy_type = eEnemyType::inky;
		enemy = dynamic_cast<EnemyBase*>(new Inky());
		enemy->Initialize();
		z_layer = 8;
		break;
	case 16:
		//�O�Y�^
		enemy_type = eEnemyType::clyde;
		enemy = dynamic_cast<EnemyBase*>(new Clyde());
		enemy->Initialize();
		z_layer = 9;
		break;
	default:
		break;
	}

	//�����摜�ݒ�
	image = move_animation[enemy_type * 2];
}

/// <summary>
/// �G�l�~�[��ԕύX����
/// </summary>
void EnemyBase::ChangeState()
{	
	//�ҋ@��Ԃ���������
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

	//��������Ԃɂ���
	if (player->GetPowerUp() == true && enemy_state != eEnemyState::ESCAPE && enemy_state != eEnemyState::WAIT)
	{
		if (enemy_state != eEnemyState::FRIGHTENED)
		{
			image = move_animation[16];
		}
		enemy_state = eEnemyState::FRIGHTENED;
	}

	// ���͏�Ԃ̎擾
	InputManager* input = InputManager::GetInstance();

	//�_�Ńt���O����
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

	//��������Ԃ���񕜂���
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
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void EnemyBase::OnHitCollision(GameObjectBase* hit_object)
{
	//��������Ԃ̂Ƃ�
	if (enemy_state == eEnemyState::FRIGHTENED)
	{
		// ���������I�u�W�F�N�g���v���C���[��������
		if (hit_object->GetCollision().object_type == eObjectType::player)
		{
			is_speed_up = true;
			enemy_state = eEnemyState::ESCAPE;
			SetDestination();
		}
	}

}

/// <summary>
/// �ړ��ʐݒ菈��
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
/// �����ݒ菈��
/// </summary>
void EnemyBase::SetDirection(eDirectionState direction)
{
	//���݂̍��W�iint�j
	int ly = (int)location.y;
	int lx = (int)location.x;

	//���݃p�l���̒��S���W�iint�j
	int py = (int)((y + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);
	int px = (int)((x + 1) * D_OBJECT_SIZE - D_OBJECT_SIZE / 2.0f);

	//���p�ɋȂ��鏈��
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
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::Movement(float delta_second)
{
	eAdjacentDirection direction[4];//�אڂ���p�l�������m�F����p�̕ϐ�
	eAdjacentDirection back = eAdjacentDirection::DOWN;		//���̕�����ۑ�����p�̕ϐ�
	int x[4], y[4];					//�אڂ���p�l���̓Y�����ۑ��p�ϐ�
	int l = 999;					//�Q�Ƃ���p�l������ڕW�p�l���܂ł̂Q�_�Ԃ̋���
	eDirectionState next_direction;	//���ɐi�ޕ���
	unsigned char old_panel;		//�O�t���[���̃p�l��

	// �G�l�~�[��Ԃɂ���āA�����ς���
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

	//���̕�����ݒ�
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

	//������z��ɕϊ� & �אڂ���p�l���̈ʒu��ۑ�
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

	//�אڂ���p�l�������擾
	adjacent_panel = StageData::GetAdjacentPanelData(location);

	//���݃p�l���̍X�V
	old_panel = now_panel;
	now_panel = StageData::GetPanelData(location);

	//�p�l�����ړ�������^�[���t���O������������
	if (old_panel != now_panel)
	{
		is_turn = false;
	}

	if (now_panel == ePanelID::BRANCH)
	{
		for (int i = 0; i < 4; i++)
		{
			//���ɍs���Ȃ��悤�ɂ���
			if (direction[i] == back)
			{
				continue;
			}
			else
			{
				//�אڂ���p�l�����ǂłȂ��Ȃ�
				if (adjacent_panel[direction[i]] != ePanelID::WALL)
				{
					//�ڕW�p�l���܂ł̋������P�ԒZ��������ۑ�����
					if (l >= (((go_x - x[i]) * (go_x - x[i])) + ((go_y - y[i]) * (go_y - y[i]))))
					{
						//�������X�V
						l = (((go_x - x[i]) * (go_x - x[i])) + ((go_y - y[i]) * (go_y - y[i])));

						//�������X�V
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

		//�ړ�������ύX����
		if (is_turn == false)
		{
			SetDirection(next_direction);
		}

		//BRANCH�̒��ł͂P��܂ŋȂ����悤�ɂ���
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

	// �ړ��� * ���� * ���� �ňړ�������肷��
	location += velocity * 100.0f * delta_second;

	// ��ʊO�ɍs������A���Α��Ƀ��[�v������
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
/// �ړ�����
/// </summary>
void EnemyBase::WaitMovement()
{
	//�ړI�n�����܂��Ă��Ȃ��Ƃ�
	if (go_x == -1 && go_y == -1)
	{
		adjacent_panel = StageData::GetAdjacentPanelData(location);

		//�ҋ@����
		if (adjacent_panel[eAdjacentDirection::UP] == ePanelID::WALL)
		{
			SetDirection(eDirectionState::DOWN);
		}
		else if (adjacent_panel[eAdjacentDirection::DOWN] == ePanelID::WALL)
		{
			SetDirection(eDirectionState::UP);
		}
	}
	//�o���Ɍ�����
	else
	{
		//����������
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
			//�c��������
			if (go_y - y < 0)
			{
				SetDirection(eDirectionState::UP);
				//����������
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
/// �ړ�����
/// </summary>
void EnemyBase::ScatterMovement()
{
	//�꒣���ݒ�
	SetDestination();
}

/// <summary>
/// �ړ�����
/// </summary>
void EnemyBase::TrackingMovement()
{
	//�e�G�l�~�[�̒ǐՏ���
	enemy->TrackingMovement();
}

/// <summary>
/// �ړ�����
/// </summary>
void EnemyBase::ScaredMovement()
{

}

/// <summary>
/// �ړ�����
/// </summary>
void EnemyBase::EscapeMovement()
{
	std::map<eAdjacentDirection, ePanelID> adjacent_panel;	//�אڂ���p�l���̃f�[�^
	int x, y;												//���ݍ��W�̓Y����

	//�אڂ���p�l�������擾
	adjacent_panel = StageData::GetAdjacentPanelData(location);
	//���ݍ��W�̓Y�������擾
	StageData::ConvertToIndex(location, y, x);

	//�Q�[�g�̏�܂ŗ����牺�ɍs��
	if (adjacent_panel[eAdjacentDirection::DOWN] == ePanelID::GATE)
	{
		SetDirection(eDirectionState::DOWN);
	}

	if (y == 11 && x == 13)
	{
		SetDirection(eDirectionState::DOWN);
	}

	//���̒��ɓ�������ҋ@��Ԃɂ���
	if (y == 15 && x == 13)
	{
		is_speed_up = false;
		enemy_state = eEnemyState::WAIT;
	}
}

/// <summary>
/// �ڕW�ݒ菈��
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
/// �������Ԑ���
/// </summary>
void EnemyBase::TimeControl()
{

}

/// <summary>
/// �A�j���[�V��������
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::AnimationControl(float delta_second)
{
	// �ړ����̃A�j���[�V����
	animation_time += delta_second;
	if (animation_time >= (0.1f))
	{
		animation_time = 0.0f;
		animation_count++;

		if (animation_count >= 2)
		{
			animation_count = 0;
		}

		//�摜�̐ݒ�
		//���񂾂Ƃ��̓A�j���[�V�������Ȃ�
		if (enemy_state != eEnemyState::ESCAPE)
		{
			//��������Ԃ̂Ƃ�
			if (enemy_state == eEnemyState::FRIGHTENED)
			{
				//�_�ŃA�j���[�V����
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
					//�������A�j���[�V����
					image = move_animation[16 + animation_count];
				}
			}
			//��������Ԃł͂Ȃ��Ƃ�
			else
			{
				image = move_animation[enemy_type * 2 + animation_count];
			}
		}
	}
}