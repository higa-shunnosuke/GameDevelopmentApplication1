#include "Bomb.h"
#include"DxLib.h"

//�R���X�g���N�^
Bomb::Bomb() :frame_count(0), animation_max(0), count(0), vector(0.0)
{
	for (int i = 0; i < 4; i++)
	{
		animation[i] = NULL;
	}

	player = nullptr;

}

//�f�X�g���N�^
Bomb::~Bomb()
{}

//����������
void Bomb::Initialize(int e_type)
{
	//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/Bomb/1.png");
	animation[1] = LoadGraph("Resource/Images/Bomb/2.png");
	animation[2] = LoadGraph("Resource/Images/Bomb/3.png");
	animation[3] = LoadGraph("Resource/Images/Bomb/4.png");

	//�傫���̐ݒ�
	box_size = 30.0f;
	//�����x�̐ݒ�

	//�G���[�`�F�b�N
	for (int i = 0; i < 4; i++)
	{
		if (animation[i] == -1)
		{
			throw ("�{���̉摜������܂���\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}

	//�����摜�̐ݒ�
	image = animation[0];

	//�ړ����x�̐ݒ�
	vector = Vector2D(0.1f, 0.0f);

}

//�X�V����
void Bomb::Update()
{
	//�ړ�����
	Movement();

	//�A�j���[�V��������
	AnimeControl();
}

//�`�揈��
void Bomb::Draw() const
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//�I��������
void Bomb::Finalize()
{
	//�g�p�����摜���J������
	for (int i = 0; i < 5; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//�����蔻��ʒm����
void Bomb::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
	vector *= 0.0f;
}

//�폜����ʒm����
bool Bomb::Delete()
{
	bool ret = false;

	//�ǂŔ��˂���
	if (location.x > 640.0f)
	{
		ret = true;
	}
	if (location.x < 0.0f)
	{
		ret = true;
	}
	if (location.y < 0.0f)
	{
		ret = true;
	}

	return ret;
}

//�v���C���[�̃|�C���^���󂯎��
void Bomb::SetPlayer(Player* player)
{
	this->player = player;
}

//�ړ�����
void Bomb::Movement()
{
	if (player->GetLocation().x > location.x)
	{
		vector.x += 0.1f;
	}
	else
	{
		vector.x -= 0.1f;
	}

	if (player->GetLocation().y > location.y)
	{
		vector.y += 0.1f;
	}
	else
	{
		vector.y -= 0.1f;
	}

	//���݂̈ʒu���W�ɑ��������Z����
	location += vector;
}

//�A�j���[�V��������
void Bomb::AnimeControl()
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
