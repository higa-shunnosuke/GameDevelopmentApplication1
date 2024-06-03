#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Enemy/Bullet.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

bool Is_pause;			//�|�[�Y�t���O
Player* p;

//�R���X�g���N�^
Scene::Scene():objects(), frame_count(0), time(60), image(NULL)
{
	//x���W
	LocationX[0] = 0.0f;
	LocationX[1] = 640.0f;
	//y���W
	LocationY[0] = 150.0f;
	LocationY[1] = 240.0f;
	LocationY[2] = 330.0f;
	LocationY[3] = 410.0f;

	//�G�̐��̃J�E���g�̏�����
	for (int i = 0; i < 4; i++)
	{
		Enemy_count[i] = 0;
	}
}

//�f�X�g���N�^
Scene::~Scene()
{
	//�Y��h�~
	Finalize();
}

//����������
void Scene::Initialize()
{
	image = LoadGraph("Resource/Images/background.png");

	//�v���C���[�𐶐�����
	p = CreateObject<Player>(Vector2D(320.0f, 50.0f),0);
	//CreateObject<Bullet>(Vector2D(320.0f, 240.0f), 0)->SetPlayer(p);

	//�G�̏o�����̐ݒ�
	Enemy_count[0] = 2;
	Enemy_count[1] = 5;
	Enemy_count[2] = 1;
	Enemy_count[3] = 1;

	//�|�[�Y�t���O�̏�����
	Is_pause = false;
}

//�X�V����
void Scene::Update()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count >= 60)
	{
		//�J�E���g�̃��Z�b�g
		frame_count = 0;
		time--;
	}

	//�|�[�Y��Ԃ̂Ƃ�
	if (Is_pause == true)
	{
		//�X�y�[�X�L�[�������ƃ|�[�Y����������
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			Is_pause = false;
		}
		
		//A�L�[�������ƃ��X�^�[�g����
		if (InputControl::GetKeyDown(KEY_INPUT_A))
		{
			Finalize();
			Initialize();
		}
	}
	//�|�[�Y��Ԃ���Ȃ��̂Ƃ�
	else
	{
		//�X�y�[�X�L�[�������ƃ|�[�Y�ɂ���
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			Is_pause = true;
		}

		//���ꂼ��̃G�l�~�[���o�����̏���܂Ő�������
		//�n�[�s�[�𐶐�����
		//�������P�b�҂��ăn�[�s�[���d�Ȃ�Ȃ��悤�ɂ���
		if (Enemy_count[0] > 0)
		{
			//��������m���𒲐�
			if (frame_count == 0)
			{
				if (GetRand(100) <= 40)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(1) + 1]), 0);
					Enemy_count[0] -= 1;
				}
			}
		}
		//�n�l�e�L�𐶐�����
		if (Enemy_count[1] > 0)
		{
			//��������m���𒲐�
			if (GetRand(100) <= 20)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(2)]), 1);
				Enemy_count[1] -= 1;
			}
		}
		//�n�R�e�L�𐶐�����
		if (Enemy_count[2] > 0)
		{
			//��������m���𒲐�
			if (GetRand(100) <= 50)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), 2);
				Enemy_count[2] -= 1;
			}
		}
		//���̃e�L�𐶐�����
		if (Enemy_count[3] > 0)
		{
			//��������m���𒲐�
			if (GetRand(100) <= 1)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), 3);
				Enemy_count[3] -= 1;
			}
		}
		
		//�V�[���ɑ��݂���I�u�W�F�N�g�̍X�V����
		for (GameObject* obj : objects)
		{
			obj->Update();
		}

		//�I�u�W�F�N�g���m�̓����蔻��`�F�b�N
		for (int i = 0; i < objects.size(); i++)
		{
			//�����蔻��`�F�b�N����
			HitCheckObject(objects[0], objects[i]);
		}
		
		//�I�u�W�F�N�g�̍폜����`�F�b�N
		for (int i = 0; i < objects.size(); i++)
		{
			//�폜����`�F�b�N����
			if (objects[i]->Delete() == true)
			{
				Enemy_count[objects[i]->GetType()]++;
				objects.erase(objects.begin() + i);
			}
		}
	}
}

//�`�揈��
void Scene::Draw() const
{
	//�w�i�摜�̕`��
	DrawRotaGraphF(320.0f, 240.0f, 0.7, 0, image, TRUE, 0);

	//�V�[���ɑ��݂���I�u�W�F�N�g�̕`�揈��
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(10, 10 + i * 20, 0x00, "�J�E���g%d�F%d",i, Enemy_count[i]);
	}
	DrawFormatString(10, 90, 0x00, "�t���[���F%d", frame_count);
	DrawFormatString(10, 110, 0x00, "���ԁF%d", time);

}

//�I��������
void Scene::Finalize()
{
	//���I�z�񂪋�Ȃ珈�����I������
	if (objects.empty())
	{
		return;
	}

	//�e�I�u�W�F�N�g���폜����
	for (GameObject* obj:objects)
	{
		obj->Finalize();
		delete obj;
	}

	//���I�z��̉��
	objects.clear();
}

//�����蔻��`�F�b�N�����i��`�̒��S�œ����蔻����Ƃ�j
void Scene::HitCheckObject(GameObject* a, GameObject* b)
{
	//�Q�̃I�u�W�F�N�g�̋������擾
	Vector2D diff = a->GetLocation() - b->GetLocation();

	//�Q�̃I�u�W�F�N�g�̓����蔻��̑傫�����擾
	Vector2D box_size = (a->GetBoxSize() + b->GetBoxSize()) / 2.0f;

	//�������傫�����傫���ꍇ�AHit����Ƃ���
	if ((fabsf(diff.x) < box_size.x) && (fabsf(diff.y) < box_size.y))
	{
		//�����������Ƃ��I�u�W�F�N�g�ɒʒm����
		a->OnHitCollision(b);
		b->OnHitCollision(a);
	}
}