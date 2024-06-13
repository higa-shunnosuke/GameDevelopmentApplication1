#include "Bomb.h"
#include"DxLib.h"


//�R���X�g���N�^
Bomb::Bomb() :frame_count(0), animation_max(0), count(0), vector(0.0f), Is_hit(false)
{
	animation[0] = NULL;

	player = nullptr;
	explosion = nullptr;
}

//�f�X�g���N�^
Bomb::~Bomb()
{}

//����������
void Bomb::Initialize(int object_type)
{
	//�摜�̓ǂݍ���
	animation[0] = LoadGraph("Resource/Images/Bomb/1.png");

	//�傫���̐ݒ�
	box_size = Vector2D(40.0f,60.0f);

	//�G���[�`�F�b�N
	if (animation[0] == -1)
	{
		throw ("�{���̉摜������܂���\n");
	}
	
	//�����摜�̏�����
	image = animation[0];

	//�I�u�W�F�N�g�^�C�v�̏�����
	type = object_type;

	//�����̏�����
	radian = 0;

	//�ړ������̏�����
	vector = Vector2D(0.0f, 0.0f);

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
	//�{���摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE, 0);

	__super::Draw();
}

//�I��������
void Bomb::Finalize()
{
	//�g�p�����摜���J������
	DeleteGraph(animation[0]);
	
}

//�����蔻��ʒm����
void Bomb::OnHitCollision(GameObject* hit_object)
{
	//�����������̏���
	if (hit_object->GetType() > 3)
	{
		Is_hit = true;
	}
}

//�폜����ʒm����
bool Bomb::Delete()
{
	bool ret = false;

	//���ɂԂ�������
	if (location.y > 350.0f + box_size.y || Is_hit == true)
	{
		ret = true;
	}

	return ret;
}

//�^�C�v�擾����
int Bomb::GetType()
{
	return this->type;
}

//�v���C���[�̃|�C���^���󂯎��A���łɐi�s������ݒ肷��
void Bomb::SetPlayer(Player* player)
{
	this->player = player;

	//�ړ������̐ݒ�
	if (player->GetDirection() == 0)
	{
		radian = 1.5707963267949;	//90��
		vector = Vector2D(0.0f, 2.5f);
	}
	else if (player->GetDirection() == 1)
	{
		radian = 0.78539816339745;	//45��
		vector = Vector2D(3.0f);
	}
	else
	{
		radian = 2.3561944901923;	//135��
		vector = Vector2D(-3.0f, 3.0f);
	}
}

//�ړ�����
void Bomb::Movement()
{
	//�ǂɓ���������^���ɗ�����
	if (location.x > 640.0f)
	{
		radian = 1.5707963267949;
		vector.x = 0.0f;
	}
	if (location.x < 0.0f)
	{
		radian = 1.5707963267949;
		vector.x = 0.0f;
	}

	////�G�ɓ��������炻�̏�Ŏ~�܂�
	if (Is_hit == true)
	{
		vector = Vector2D(0.0f);
	}

	//���݂̈ʒu���W���X�V����
	location += vector;
}
