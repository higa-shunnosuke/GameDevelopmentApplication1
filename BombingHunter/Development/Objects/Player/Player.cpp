#include"Player.h"
#include"../../Utility/InputControl.h"
#include"../../Utility/ResourceManager.h"
#include"DxLib.h"

//�R���X�g���N�^
Player::Player():frame_count(0),flip_flag(FALSE),direction(0)
{
	Is_hit = false;
}

//�f�X�g���N�^
Player::~Player()
{

}

//����������
void Player::Initialize(int object_type)
{
	//�A�j���[�V�����摜��ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	tmp = rm->GetImages("Resource/Images/Tri-pilot/1.png");
	animation.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Tri-pilot/2.png");
	animation.push_back(tmp[0]);

	//�G���[�`�F�b�N
	if (animation[0]==-1||animation[1]==-1)
	{
		throw ("�g���p�C���b�g�̉摜������܂���\n");
	}

	//�����̐ݒ�
	radian = 0.0;

	//�傫���̐ݒ�
	box_size = 60.0f;

	//�����摜�̐ݒ�
	image = animation[0];

	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	type = object_type;
}

//�X�V����
void Player::Update()
{
	//�ړ�����
	Movement();
	//�A�j���[�V��������
	AnimeControl();

}

//�`�揈��
void Player::Draw() const
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, flip_flag);

	__super::Draw();
}

//�I��������
void Player::Finalize()
{
	//�g�p�����摜���J������
	DeleteGraph(animation[0]);
	DeleteGraph(animation[1]);
}

//�����蔻��ʒm����
void Player::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
}

//�ʒu���擾����
int Player::GetDirection() const
{
	return this->direction;
}

//�ړ�����
void Player::Movement()
{
	//�ړ��̑���
	Vector2D vector = 0.0f;

	//���E�ړ�
	if (InputControl::GetKey(KEY_INPUT_LEFT))
	{
		vector.x += -2.0f;
		flip_flag = TRUE;
		direction = -1;
	}
	else if (InputControl::GetKey(KEY_INPUT_RIGHT))
	{
		vector.x += 2.0f;
		flip_flag = FALSE;
		direction = 1;
	}
	else
	{
		vector.x += 0.0f;
		direction = 0;
	}
	
	//�ǂ̏���
	if (location.x < box_size.x / 2.0f)
	{
		vector.x += 2.0f;
	}
	if (location.x > 640.0f - box_size.x / 2.0f)
	{
		vector.x -= 2.0f;
	}
	
	//���݂̈ʒu���W���X�V����
	location += vector;
}

//�A�j���[�V��������
void Player::AnimeControl()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count>=60)
	{
		//�J�E���g�̃��Z�b�g
		frame_count = 0;

		//�摜�̐؂�ւ�
		if (image==animation[0])
		{
			image = animation[1];
		}
		else
		{
			image = animation[0];
		}
	}
}
