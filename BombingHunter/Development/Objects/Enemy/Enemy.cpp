#include"Enemy.h"
#include"DxLib.h"

//�R���X�g���N�^
Enemy::Enemy() :frame_count(0),animation_max(0), count(0), vector(0.0),speed(0.0f),type(0)
{
	for (int i = 0; i < 5; i++)
	{
		animation[i] = NULL;
	}
}

//�f�X�g���N�^
Enemy::~Enemy()
{
	Finalize();
}

//����������
void Enemy::Initialize(int e_type)
{
	switch (e_type)
	{
	case 0:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Harpy/1.png");
		animation[1] = LoadGraph("Resource/Images/Harpy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 80.0f;
		//�ړ����x�̐ݒ�
		speed = 1.0f + (float)GetRand(1);
		//�G�̃^�C�v�̐ݒ�
		type = e_type;
		break;
	case 1:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Fly-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Fly-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 70.0f;
		//�ړ����x�̐ݒ�
		speed = 1.0f + (float)GetRand(1);
		//�G�̃^�C�v�̐ݒ�
		type = e_type;		break;
	case 2:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Box-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Box-enemy/2.png");
		animation[2] = NULL;
		animation[3] = NULL;
		animation[4] = NULL;
		//�傫���̐ݒ�
		box_size = 70.0f;
		//�ړ����x�̐ݒ�
		speed = 0.7f;
		//�G�̃^�C�v�̐ݒ�
		type = e_type;
		break;
	case 3:
		//�摜�̓ǂݍ���
		animation[0] = LoadGraph("Resource/Images/Gorld-enemy/1.png");
		animation[1] = LoadGraph("Resource/Images/Gorld-enemy/2.png");
		animation[2] = LoadGraph("Resource/Images/Gorld-enemy/3.png");
		animation[3] = LoadGraph("Resource/Images/Gorld-enemy/4.png");
		animation[4] = LoadGraph("Resource/Images/Gorld-enemy/5.png");
		//�傫���̐ݒ�
		box_size = 50.0f;
		//�ړ����x�̐ݒ�
		speed = 1.0f;
		//�G�̃^�C�v�̐ݒ�
		type = e_type;
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
	//�ړ�����
	Movement();
	//�A�j���[�V��������
	AnimeControl();
}

//�`�揈��
void Enemy::Draw() const
{
	int flip_flag;		//�摜���]�t���O

	//�i�s�����ɂ���āA���]��Ԃ����肷��
	if (vector.x > 0.0f)
	{
		flip_flag = FALSE;
	}
	else
	{
		flip_flag = TRUE;
	}

	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, flip_flag);

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
}

//�폜����ʒm����
bool Enemy::Delete()
{
	bool ret = false;

	//�ǂŔ��˂���
	if (location.x > 640.0f + box_size.x)
	{
		ret = true;
	}
	if (location.x < 0.0f - box_size.x)
	{
		ret = true;
	}

	return ret;
}

//�v���C���[�̃|�C���^���󂯎��
void Enemy::SetPlayer(Player* player)
{
	this->player = player;
}


//�^�C�v�擾����
int Enemy::GetType()
{
	return type;
}

//�ړ�����
void Enemy::Movement()
{
	//���݂̈ʒu���W�ɑ��������Z����
	location += vector;
}

//�A�j���[�V��������
void Enemy::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count >= 60)
	{
		//�J�E���g�̃��Z�b�g
		frame_count = 0;

		//�摜�̐؂�ւ�
		if (image == animation[animation_max-1])
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
