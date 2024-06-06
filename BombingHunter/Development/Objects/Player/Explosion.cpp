#include "Explosion.h"
#include "DxLib.h"

//�R���X�g���N�^
Explosion::Explosion(Vector2D location) :frame_count(0), animation_max(0), count(0)
{
	for (int i = 0; i < 3; i++)
	{
		animation[i] = NULL;
	}
}

//�f�X�g���N�^
Explosion::~Explosion()
{

}

//����������
void Explosion::Initialize(int e_type)
{
	//�摜�̓ǂݍ���
//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/Explosion/1.png");
	animation[1] = LoadGraph("Resource/Images/Explosion/2.png");
	animation[2] = LoadGraph("Resource/Images/Explosion/3.png");

	//�G���[�`�F�b�N
	for (int i = 0; i < 5; i++)
	{
		if (animation[i] == -1)
		{
			throw ("�����̉摜������܂���\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}

	//�����摜�̏�����
	image = animation[0];

	//�����̏�����
	radian = 0;

}

//�X�V����
void Explosion::Update()
{
	//�A�j���[�V��������
	AnimeControl();
}

//�`�揈��
void Explosion::Draw() const
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

}

//�I��������
void Explosion::Finalize()
{
	//�g�p�����摜���J������
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//�A�j���[�V��������
void Explosion::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

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