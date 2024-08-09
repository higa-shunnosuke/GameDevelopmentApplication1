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

//����������
void EnemyBase::Initialize()
{
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
}

//�X�V����
void EnemyBase::Update(float delta_second)
{
	//�p�l���̊m�F
	StageData::ConvertToIndex(location, y, x);

	//�ړ�����
	Movement(delta_second);

	// �A�j���[�V��������
	AnimationControl(delta_second);

	/// �G�l�~�[��ԕύX����
	ChangeState();
}

//�X�V����
void EnemyBase::Draw(const Vector2D& screen_offset) const
{
	if (enemy_state != eEnemyState::ESCAPE)
	{
		// �e�N���X�̕`�揈�����Ăяo��
		__super::Draw(screen_offset);
	}

	if (enemy_state != eEnemyState::FRIGHTENED)
	{
		// �I�t�Z�b�g�l����ɉ摜�̕`����s��
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

//�I��������
void EnemyBase::Finalize()
{
	// ���I�z��̉��
	move_animation.clear();
}

/// <summary>
/// �����蔻��ʒm����
/// </summary>
/// <param name="hit_object">���������Q�[���I�u�W�F�N�g�̃|�C���^</param>
void EnemyBase::OnHitCollision(GameObjectBase* hit_object)
{
	/*if (enemy_state != eEnemyState::WAIT && enemy_state != eEnemyState::ESCAPE)
	{*/
		// ���������A�I�u�W�F�N�g���ǂ�������
		if (hit_object->GetCollision().object_type == eObjectType::wall)
		{
			// �����蔻������擾���āA�J�v�Z��������ʒu�����߂�
			CapsuleCollision hc = hit_object->GetCollision();
			hc.point[0] += hit_object->GetLocation();
			hc.point[1] += hit_object->GetLocation();

			// �ŋߖT�_�����߂�
			Vector2D near_point = NearPointCheck(hc, this->location);

			//Enemy����near_point�ւ̕����x�N�g�����擾
			Vector2D dv2 = near_point - this->location;
			Vector2D dv = this->location - near_point;

			// �߂荞�񂾍���
			float diff = (this->GetCollision().radius + hc.radius) - dv.Length();

			// diff�̕������߂�
			location += dv.Normalize() * diff;
		}
	/*}*/

	//��������Ԃ̂Ƃ�
	if (enemy_state == eEnemyState::FRIGHTENED)
	{
		// ���������I�u�W�F�N�g���v���C���[��������
		if (hit_object->GetCollision().object_type == eObjectType::player)
		{
			enemy_state = eEnemyState::ESCAPE;
		}
	}
	
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
	case 14:
		//�s���L�[
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
/// �G�l�~�[��ԕύX����
/// </summary>
void EnemyBase::ChangeState()
{	
	//�ҋ@��Ԃ���������
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

	
	//��������Ԃɂ���
	if (player->GetPowerUp() == true && enemy_state != eEnemyState::ESCAPE)
	{
		enemy_state = eEnemyState::FRIGHTENED;
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
		// �摜�̐ݒ�
		//���񂾂Ƃ��̓A�j���[�V�������Ȃ�
		if (enemy_state != eEnemyState::ESCAPE)
		{
			//��������Ԃ̂Ƃ�
			if (enemy_state == eEnemyState::FRIGHTENED)
			{
				image = move_animation[16 + animation_count];
			}
			//��������Ԃł͂Ȃ��Ƃ�
			else
			{
				image = move_animation[enemy_type * 2 + animation_count];
			}
		}
	
	}
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::Movement(float delta_second)
{
	// �G�l�~�[��Ԃɂ���āA�����ς���
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

	// ���͏�Ԃ̎擾
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

		// �i�s�����̈ړ��ʂ�ǉ�
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

		// �ړ��� * ���� * ���� �ňړ�������肷��
		location += velocity * 50.0f * delta_second;

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
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::WaitMovement()
{


}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::TerritoryMovement()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::TrackingMovement()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::ScaredMovement()
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::EscapeMovement()
{

}

/// <summary>
/// �ڕW�ݒ菈��
/// </summary>
void EnemyBase::SetDestination()
{

}