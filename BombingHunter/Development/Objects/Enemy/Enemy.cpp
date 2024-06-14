#include"Enemy.h"
#include "Bullet.h"
#include"DxLib.h"


//�R���X�g���N�^
Enemy::Enemy() :frame_count(0),animation_max(0), count(0), vector(0.0),speed(0.0f),
Is_hit(false), Is_death(false)
{
	for (int i = 0; i < 5; i++)
	{
		animation[i] = NULL;
	}

	BlendMode = 255;
	flip_flag = FALSE;
}

//�f�X�g���N�^
Enemy::~Enemy()
{
	Finalize();
}

//����������
void Enemy::Initialize(int object_type)
{
	switch (object_type)
	{
	case 4:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Harpy/1.png");
		animation[1] = LoadGraph("Resource/Images/Harpy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 70.0f;
		//�ړ����x�̐ݒ�
		speed = 1.0f + (float)GetRand(1);
		//�G�̃^�C�v�̐ݒ�
		type = object_type;
		break;
	case 5:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Fly-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Fly-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 70.0f;
		//�ړ����x�̐ݒ�
		speed = 2.0f + (float)GetRand(2);
		//�G�̃^�C�v�̐ݒ�
		type = object_type;		break;
	case 6:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Box-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Box-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 70.0f;
		//�ړ����x�̐ݒ�
		speed = 2.0f;
		//�G�̃^�C�v�̐ݒ�
		type = object_type;
		break;
	case 7:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Gorld-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Gorld-enemy/2.png");
		animation[2] = LoadGraph("Resource/Images/Gorld-enemy/3.png");
		animation[3] = LoadGraph("Resource/Images/Gorld-enemy/4.png");
		animation[4] = LoadGraph("Resource/Images/Gorld-enemy/5.png");
		//�傫���̐ݒ�
		box_size = 40.0f;
		//�ړ����x�̐ݒ�
		speed = 2.5f;
		//�G�̃^�C�v�̐ݒ�
		type = object_type;
		break;
	default:
		break;
	}
	
	//�G���[�`�F�b�N
	for (int i = 0; i < 5; i++)
	{
		if (animation[i] == -1)
		{
			throw ("�n�l�e�L�̉摜������܂���\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}
	

	//�����̐ݒ�
	radian = 0.0;

	//�����摜�̐ݒ�
	image = animation[0];

	//�ړ����x�̐ݒ�
	if (location.x <= 0.0f)
	{
		vector = Vector2D(speed, 0.0f);
	}
	else if (location.x >= 640.0f)
	{
		vector = Vector2D(-speed, 0.0f);
	}
}

//�X�V����
void Enemy::Update()
{
	//�i�s�����ɂ���āA���]��Ԃ����肷��
	if (Is_hit != true)
	{
		if (vector.x > 0.0f)
		{
			flip_flag = FALSE;
		}
		else
		{
			flip_flag = TRUE;
		}
	}

	//�ړ�����
	Movement();
	
	//�A�j���[�V��������
	AnimeControl();
}

//�`�揈��
void Enemy::Draw() const
{
	if (Is_hit == true)
	{
		//�`�惂�[�h���A���t�@�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, BlendMode);
	}

	//�G�l�~�[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, flip_flag);

	//�`�惂�[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	__super::Draw();
}

//�I��������
void Enemy::Finalize()
{
	//�g�p�����摜���J������
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//�����蔻��ʒm����
void Enemy::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
	//�{���ɓ���������
	if (hit_object->GetType() < 2)
	{
		Is_hit = true;
		//�����蔻����Ȃ���
		box_size = Vector2D(0.0f);
		vector = Vector2D(1.0f,1.0f);
	}
}

//�폜����ʒm����
bool Enemy::Delete()
{
	bool ret = false;

	//��ʊO�ɍs������A���񂾂�
	if (location.x > 640.0f + box_size.x || location.x < 0.0f - box_size.x || Is_death == true)
	{
		ret = true;
	}
	
	return ret;
}

//�^�C�v�擾����
int Enemy::GetType()
{
	return this->type;
}

//�ړ�����
void Enemy::Movement()
{
	//���݂̈ʒu���W���X�V����
	location += vector;
}

//�A�j���[�V��������
void Enemy::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	if (Is_hit != true)
	{
		//�U�O�t���[���ڂɓ��B������
		if (frame_count >= 60)
		{
			//�J�E���g�̃��Z�b�g
			frame_count = 0;

			//�摜�̐؂�ւ�
			if (image == animation[animation_max - 1])
			{
				count = 0;
				image = animation[count];
			}
			else
			{
				count++;
				image = animation[count];
			}
		}
	}
	else if(frame_count % 5  == 0)
	{
		vector.x *= -1.0f;

		if (BlendMode <= 0)
		{
			Is_death = true;
		}
		else
		{
			BlendMode -= 50;
		}
	}
}
