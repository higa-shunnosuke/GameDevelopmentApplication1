#include "Enemy.h"
#include "../../Utility/InputControl.h"
#include"DxLib.h"

//�R���X�g���N�^
Enemy::Enemy() :animation_count(0), velocity(0.0)
{
	animation[0] = NULL;
	animation[1] = NULL;
}

//�f�X�g���N�^
Enemy::~Enemy()
{}

//����������
void Enemy::Initialize()
{
	//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/Fly-enemy/1.png");
	animation[1] = LoadGraph("Resource/Images/Fly-enemy/2.png");

	//�G���[�`�F�b�N
	if (animation[0] == -1 || animation[1] == -1)
	{
		throw ("�n�l�e�L�̉摜������܂���\n");
	}

	//�����̐ݒ�
	radian = 0.0;

	//�傫���̐ݒ�
	box_size = 64.0f;

	//�����摜�̐ݒ�
	image = animation[0];

	//�ړ����x�̐ݒ�
	if (location.x <= 0.0f)
	{
		velocity = Vector2D(1.0f, 0.0f);
	}
	else if (location.x >= 640.0f)
	{
		velocity = Vector2D(-1.0f, 0.0f);
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
	if (velocity.x > 0.0f)
	{
		flip_flag = FALSE;
	}
	else
	{
		flip_flag = TRUE;
	}

	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.8, radian, image, TRUE, flip_flag);

	__super::Draw();
}

//�I��������
void Enemy::Finalize()
{
	//�g�p�����摜���J������
	DeleteGraph(animation[0]);
	DeleteGraph(animation[1]);
}

//�����蔻��ʒm����
void Enemy::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
	velocity *= -1.0f;
}

//�ړ�����
void Enemy::Movement()
{
	//�ǂŔ��˂���
	if (location.x > 640.0f)
	{
		velocity.x -= 0.1f;
	}
	if (location.x < 0.0f)
	{
		velocity.x += 0.1f;
	}

	//���݂̈ʒu���W�ɑ��������Z����
	location += velocity;
}

//�A�j���[�V��������
void Enemy::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	animation_count++;

	//�U�O�t���[���ڂɓ��B������
	if (animation_count >= 60)
	{
		//�J�E���g�̃��Z�b�g
		animation_count = 0;

		//�摜�̐؂�ւ�
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
