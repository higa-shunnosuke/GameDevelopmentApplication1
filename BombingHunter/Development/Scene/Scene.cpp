#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Player/Bomb.h"
#include "../Objects/Player/Explosion.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Enemy/Bullet.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

bool Is_pause;		//�|�[�Y�t���O
Player* p;			//�v���C���[�̃|�C���^


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

	//�G�̐��̃J�E���g��������
	for (int i = 0; i < 4; i++)
	{
		Enemy_count[i] = 0;
	}

	//�{���̐��̃J�E���g��������
	Bomb_count = 0;
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
	p = CreateObject<Player>(Vector2D(320.0f, 50.0f),TYPE::PLAYER);

	//�G�̏o�����̐ݒ�
	Enemy_count[0] = 0;
	Enemy_count[1] = 0;
	Enemy_count[2] = 0;
	Enemy_count[3] = 0;

	//�|�[�Y�t���O�̏�����
	Is_pause = false;

	//�t���[���J�E���g�̏�����
	frame_count = 0;

	//�������Ԃ̏�����
	time = 60;
}

//�X�V����
void Scene::Update()
{
	//�|�[�Y��Ԃ̂Ƃ�
	if (Is_pause == true)
	{
		//Z�L�[�������ƃ|�[�Y����������
		if (InputControl::GetKeyDown(KEY_INPUT_Z))
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
		//Z�L�[�������ƃ|�[�Y�ɂ���
		if (InputControl::GetKeyDown(KEY_INPUT_Z))
		{
			Is_pause = true;
		}

		//�t���[���J�E���g�����Z����
		frame_count++;

		//�U�O�t���[���ڂɓ��B������
		if (frame_count == 60)
		{
			//�J�E���g�̃��Z�b�g
			time--;
			frame_count = 0;
		}

		//���ꂼ��̃G�l�~�[���o�����̏���܂Ő�������
		//�n�[�s�[�𐶐�����
		//�������P�b�҂��ăn�[�s�[���d�Ȃ�Ȃ��悤�ɂ���
		if (Enemy_count[0] < 2)
		{
			//��������Ԋu��������
			if (frame_count == 0)
			{
				//��������m���𒲐�
				if (GetRand(100) <= 40)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(1) + 1]), TYPE::HARPY);
					Enemy_count[TYPE::HARPY - 4] += 1;
				}
			}
		}
		//�n�l�e�L�𐶐�����
		if (Enemy_count[1] < 5)
		{
			//��������Ԋu��������
			if (frame_count == 0)
			{
				//��������m���𒲐�
				if (GetRand(100) <= 50)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[GetRand(2)]), TYPE::FLY_ENEMY);
					Enemy_count[TYPE::FLY_ENEMY - 4] += 1;
				}
			}
		}
		//�n�R�e�L�𐶐�����
		if (Enemy_count[2] < 1)
		{
			//��������m���𒲐�
			if (GetRand(100) <= 20)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::BOX_ENEMY);
				Enemy_count[TYPE::BOX_ENEMY - 4] += 1;
			}
		}
		//���̃e�L�𐶐�����
		if (Enemy_count[3] < 1)
		{
			//��������m���𒲐�
			if (GetRand(100) <= 1)
			{
				CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::GORLD_ENEMY);
				Enemy_count[TYPE::GORLD_ENEMY - 4] += 1;
			}
		}
		//�e�𐶐�����
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetType() == TYPE::BOX_ENEMY)
			{
				//��������Ԋu��������
				if (frame_count == 0)
				{
					//��������m���𒲐�
					if (GetRand(10) <= 3)
					{
						CreateObject<Bullet>(Vector2D(objects[i]->GetLocation()), TYPE::BULLET)->SetPlayer(p);
					}
				}
			}
		}

		//�X�y�[�X�L�[�������ꂽ��A�{���𐶐�����
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			if (Bomb_count < 1)
			{
				CreateObject<Bomb>(objects[0]->GetLocation(), TYPE::BOMB)->SetPlayer(p);
				Bomb_count++;
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
			for (int j = i + 1; j < objects.size(); j++)
			{
				//�����蔻��`�F�b�N����
				HitCheckObject(objects[i], objects[j]);
			}
		}
		
		//�I�u�W�F�N�g�̍폜����`�F�b�N
		for (int i = 0; i < objects.size(); i++)
		{
			//�폜����`�F�b�N����
			if (objects[i]->Delete() == true)
			{
				if (objects[i]->GetType() > 3)
				{
					Enemy_count[objects[i]->GetType() - 4 ]--;
				}
				else if (objects[i]->GetType() == 1)
				{
					Bomb_count--;

					//���j�G�t�F�N�g�̐���
					CreateObject<Explosion>(objects[i]->GetLocation(), TYPE::EXPLOSION);
				}
				//�I�u�W�F�N�g�̍폜
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