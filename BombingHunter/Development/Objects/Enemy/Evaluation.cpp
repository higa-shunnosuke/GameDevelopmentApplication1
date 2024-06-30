#include "Evaluation.h"
#include"DxLib.h"

//�R���X�g���N�^
Evaluation::Evaluation()
{
	SE = NULL;
}

//�f�X�g���N�^
Evaluation::~Evaluation()
{
	Finalize();
}

//����������
void Evaluation::Initialize(int object_type)
{
	switch (object_type)
	{
	case 8:
		//�摜�̓ǂݍ���
		image = LoadGraph("Resource/Images/Evaluation/Perfect.png");

		//�����̓ǂݍ���
		SE = LoadSoundMem("Resource/sounds/SE_perfect.wav");

		//�傫���̐ݒ�
		box_size = Vector2D(410.0f, 90);
		
		break;
	case 9:
		//�摜�̓ǂݍ���
		image = LoadGraph("Resource/Images/Evaluation/GOOD.png");

		//�����̓ǂݍ���
		SE = LoadSoundMem("Resource/sounds/SE_good.wav");

		//�傫���̐ݒ�
		box_size = Vector2D(400.0f, 140);
		break;
	case 10:
		//�摜�̓ǂݍ���
		image = LoadGraph("Resource/Images/Evaluation/OK.png");

		//�����̓ǂݍ���
		SE = LoadSoundMem("Resource/sounds/SE_ok.wav");

		//�傫���̐ݒ�
		box_size = Vector2D(400.0f, 260);
		break;
	case 11:
		//�摜�̓ǂݍ���
		image = LoadGraph("Resource/Images/Evaluation/BAD.png");

		//�����̓ǂݍ���
		SE = LoadSoundMem("Resource/sounds/SE_bad.wav");

		//�傫���̐ݒ�
		box_size = Vector2D(350.0f,120);
	default:
		break;
	}

	if (image == -1)
	{
		throw ("�]���̉摜������܂���\n");
	}
	
	type = object_type;

	PlaySoundMem(SE, DX_PLAYTYPE_BACK);

}

//�X�V����
void Evaluation::Update()
{
	
}

//�`�揈��
void Evaluation::Draw() const
{
	//�]���摜�̕`��
	DrawRotaGraphF(location.x, location.y, 0.7, radian, image, TRUE);

	__super::Draw();
}

//�I��������
void Evaluation::Finalize()
{
	//�g�p�����摜���J������
	DeleteGraph(image);
}