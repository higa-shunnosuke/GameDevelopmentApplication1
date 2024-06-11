#include "Explosion.h"
#include "DxLib.h"

//�R���X�g���N�^
Explosion::Explosion() :frame_count(0), animation_max(0), count(0), radian(0.0f), Is_anim(false)
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
void Explosion::Initialize(int object_type)
{
	//�摜�̓ǂݍ���
//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/Explosion/1.png");
	animation[1] = LoadGraph("Resource/Images/Explosion/2.png");
	animation[2] = LoadGraph("Resource/Images/Explosion/3.png");

	//�G���[�`�F�b�N
	for (int i = 0; i < 3; i++)
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

	//�傫���̐ݒ�
	box_size = 0.0f;

	//�����摜�̏�����
	image = animation[0];

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	type = object_type;

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
	//���j�G�t�F�N�g�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

}

//�I��������
void Explosion::Finalize()
{
	//�g�p�����摜���J������
	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(animation[i]);
	}
}

// �폜����ʒm����
bool Explosion::Delete()
{
	bool ret = false;

	//�A�j���[�V�������I��������
	if (Is_anim == true)
	{
		ret = true;
	}

	return ret;
}

//�^�C�v�擾����
int Explosion::GetType()
{
	return this->type;
}

//�A�j���[�V��������
void Explosion::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count >= 10)
	{
		//�J�E���g�̃��Z�b�g
		frame_count = 0;

		//�摜�̐؂�ւ�
		if (image == animation[animation_max - 1])
		{
			count = 0;
			//�A�j���[�V�����I��
			Is_anim = true;
		}
		else
		{
			count++;
			image = animation[count];
		}
	}

}