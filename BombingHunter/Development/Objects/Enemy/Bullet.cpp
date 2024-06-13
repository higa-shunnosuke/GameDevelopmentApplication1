#include "Bullet.h"
#include "DxLib.h"
#include "math.h"


//�R���X�g���N�^
Bullet::Bullet() :frame_count(0), animation_max(0), count(0), vector(0.0), Is_hit(false), Is_anim(false)
{
	for (int i = 0; i < 4; i++)
	{
		animation[i] = NULL;
	}
	//�|�C���^�̏�����
	player = nullptr;

	//�p�x�̏�����
	radian = 0.0f;
}

//�f�X�g���N�^
Bullet::~Bullet()
{}

//����������
void Bullet::Initialize(int object_type)
{
	//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/bullet/1.png");
	animation[1] = LoadGraph("Resource/Images/bullet/2.png");
	animation[2] = LoadGraph("Resource/Images/bullet/3.png");
	animation[3] = LoadGraph("Resource/Images/bullet/4.png");

	//�傫���̐ݒ�
	box_size = 30.0f;
	
	//�G���[�`�F�b�N
	for (int i = 0; i < 4; i++)
	{
		if (animation[i] == -1)
		{
			throw ("�o���b�g�̉摜������܂���\n");
		}
		else if (animation[i] != 0)
		{
			animation_max++;
		}
	}

	//�����摜�̐ݒ�
	image = animation[0];

	//�I�u�W�F�N�g�^�C�v�̏�����
	type = object_type;

	//�ړ����x�̏�����
	vector = Vector2D(0.0f);

}

//�X�V����
void Bullet::Update()
{
	//�ړ�����
	Movement();

	//�A�j���[�V��������
	AnimeControl();
}

//�`�揈��
void Bullet::Draw() const
{
	//�o���b�g�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//�I��������
void Bullet::Finalize()
{
	//�g�p�����摜���J������
	for (int i = 0; i < 4; i++)
	{
		DeleteGraph(animation[i]);
	}
}

//�����蔻��ʒm����
void Bullet::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
	if (hit_object->GetType() == 0)
	{
		Is_hit = true;

		//�e�̓������~�߂�
		vector = Vector2D(0.0f);
	}
}

//�폜����ʒm����
bool Bullet::Delete()
{
	bool ret = false;

	//��ʊO�ɍs������,�A�j���[�V�������I�������
	if (location.y < 0.0f || Is_anim == true)
	{
		ret = true;
	}

	return ret;
}

//�v���C���[�̃|�C���^���󂯎��A���łɐi�s������ݒ肷��
void Bullet::SetPlayer(Player* player)
{
	this->player = player;

	//�v���C���[�Ƃ̋������擾
	Vector2D deff = player->GetLocation() - location;
	
	//�v���C���[�܂ł̊p�x���Z�o
	radian = atan2f(deff.y, deff.x);
	vector = Vector2D(cosf(radian), sinf(radian));

}

//�^�C�v�擾����
int Bullet::GetType()
{
	return this->type;
}

//�ړ�����
void Bullet::Movement()
{
	if (Is_hit != true)
	{
		//���݂̈ʒu���W�ɑ��������Z����
		location += vector;
	}
}

//�A�j���[�V��������
void Bullet::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count >= 10)
	{
		//�J�E���g�̃��Z�b�g
		frame_count = 0;

		if (Is_hit == true)
		{
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
}
