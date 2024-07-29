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

	//��ԕ`�揈��
	DrawFormatString(10, 130, 0xffffff, "%d", enemy_state);
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
	if (enemy_state != eEnemyState::WAIT && enemy_state != eEnemyState::ESCAPE)
	{
		// ���������A�I�u�W�F�N�g���ǂ�������
		if (hit_object->GetCollision().object_type == eObjectType::wall)
		{
			// �����蔻������擾���āA�J�v�Z��������ʒu�����߂�
			CapsuleCollision hc = hit_object->GetCollision();
			hc.point[0] += hit_object->GetLocation();
			hc.point[1] += hit_object->GetLocation();

			// �ŋߖT�_�����߂�
			Vector2D near_point = NearPointCheck(hc, this->location);

			// Player����near_point�ւ̕����x�N�g�����擾
			Vector2D dv2 = near_point - this->location;
			Vector2D dv = this->location - near_point;

			// �߂荞�񂾍���
			float diff = (this->GetCollision().radius + hc.radius) - dv.Length();

			// diff�̕������߂�
			location += dv.Normalize() * diff;
		}
	}

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
void EnemyBase::SetType(int type)
{
	switch (type)
	{
	case 0:
		//�A�J�x�C
		enemy_type = eEnemyType::blinky;
		// ���C���[�̐ݒ�
		z_layer = 6;

		break;
	case 1:
		//�A�I�X�P
		enemy_type = eEnemyType::inky;
		z_layer = 8;

		break;
	case 2:
		//�O�Y�^
		enemy_type = eEnemyType::clyde;
		z_layer = 7;

		break;
	case 3:
		//�s���L�[
		enemy_type = eEnemyType::pinky;
		z_layer = 9;

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
void EnemyBase::WaitMovement(float delta_second)
{
	

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::TerritoryMovement(float delta_second)
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::TrackingMovement(float delta_second)
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::ScaredMovement(float delta_second)
{

}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void EnemyBase::EscapeMovement(float delta_second)
{

}

/// <summary>
/// �ڕW�ݒ菈��
/// </summary>
void EnemyBase::SetDestination()
{

}