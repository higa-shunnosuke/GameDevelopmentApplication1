#include "Bomb.h"
#include"DxLib.h"

//�R���X�g���N�^
Bomb::Bomb() :frame_count(0), animation_max(0), count(0), vector(0.0), Is_hit(false)
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

	//�傫���̐ݒ�
	box_size = 30.0f;
	//�����x�̐ݒ�

	//�G���[�`�F�b�N

	if (animation[0] == -1)
	{
		throw ("�{���̉摜������܂���\n");
	}
	
	//�����摜�̐ݒ�
	image = animation[0];

	//�ړ����x�̐ݒ�
	vector = Vector2D(0.0f, 1.0f);
}

//�X�V����
void Bomb::Update()
{
	//�ړ�����
	Movement();

}

//�`�揈��
void Bomb::Draw() const
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, 1.5707963267949, image, TRUE, 0);

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
	Is_hit = true;
}

//�폜����ʒm����
bool Bomb::Delete()
{
	bool ret = false;

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
	//���݂̈ʒu���W�ɑ��������Z����
	location += vector;
}
