#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Enemy/Enemy.h"
#include"../Utility/InputControl.h"
#include"DxLib.h"

//�R���X�g���N�^
Scene::Scene():objects(),spawn_count(0), image(NULL),type(0)
{
	//x���W
	LocationX[0] = 0.0f;
	LocationX[1] = 640.0f;
	//y���W
	LocationY[0] = 150.0f;
	LocationY[1] = 250.0f;
	LocationY[2] = 350.0f;
	LocationY[3] = 450.0f;

	//�G�̐��̃J�E���g�̏�����

	Enemy_count[0] = 0;
	Enemy_count[1] = 0;
	Enemy_count[2] = 0;
	Enemy_count[3] = 0;
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
	CreateObject<Player>(Vector2D(320.0f, 50.0f),0);
	//�G�l�~�[�𐶐�����
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[0]), 0);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[1]), 1);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[2]), 2);
	CreateObject<Enemy>(Vector2D(LocationX[0], LocationY[3]), 3);

}

//�X�V����
void Scene::Update()
{
	if (InputControl::GetKeyDown(KEY_INPUT_Z))
	{
		if (Enemy_count[type] < 4)
		{
			CreateObject<Enemy>(Vector2D(LocationX[1], LocationY[type]), type);
			Enemy_count[type] += 1;
			type++;
		}
	}
		
	//�V�[���ɑ��݂���I�u�W�F�N�g�̍X�V����
	for (GameObject* obj:objects)
	{
		obj->Update();
	}

	//�I�u�W�F�N�g���m�̓����蔻��`�F�b�N
	for (int i = 0; i < objects.size(); i++)
	{
		//�����蔻��`�F�b�N����
		HitCheckObject(objects[0], objects[i]);
	}
}

//�`�揈��
void Scene::Draw() const
{
	//�w�i�摜�̕`��
	DrawRotaGraphF(320.0f, 240.0f, 0.8, 0, image, TRUE, 0);

	//�V�[���ɑ��݂���I�u�W�F�N�g�̕`�揈��
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(10, 10 + i * 20, 0x00, "�J�E���g%d�F%d",i, Enemy_count[i]);
	}
	DrawFormatString(10, 90, 0x00, "�^�C�v�F%d", type);
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