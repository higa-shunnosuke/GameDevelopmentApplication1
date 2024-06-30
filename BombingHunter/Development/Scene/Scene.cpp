#include "Scene.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Player/Bomb.h"
#include "../Objects/Player/Explosion.h"
#include "../Objects/Enemy/Enemy.h"
#include "../Objects/Enemy/Evaluation.h"
#include "../Objects/Enemy/Bullet.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

#define SCORE_DATA ("Resource/dats/highscore.csv")

Player* p;				//�v���C���[�̃|�C���^
int image;

//�R���X�g���N�^
Scene::Scene():objects(), frame_count(0), time(0),
background_image(NULL),
background_sound(NULL),
timer_image(NULL),
score_image(NULL),
minus_image(NULL),
highscore_image(NULL),
throw_SE(NULL),
explosion_SE(NULL),
timeup_SE(NULL),
timeup(false)

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

	//�����摜�̏�����
	for (int i = 0; i < 11; i++)
	{
		number[i] = NULL;
	}

	//�{���̐��̃J�E���g��������
	Bomb_count = 0;

	//�X�R�A�̏�����
	score = 0;
	highscore = 0;

	//�|�[�Y�t���O�̏�����
	Is_pause = false;
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
	//�摜�̓Ǎ���
	background_image = LoadGraph("Resource/Images/background.png");
	timer_image = LoadGraph("Resource/Images/Evaluation/timer.png");
	score_image = LoadGraph("Resource/Images/Score/score.png");
	highscore_image = LoadGraph("Resource/Images/Score/highscore.png");
	minus_image = LoadGraph("Resource/Images/Score/-.png");
	LoadDivGraph("Resource/images/Score/numbers.png", 10, 5, 2, 160, 214, number);

	//�����̓ǂݍ���
	background_sound = LoadSoundMem("Resource/sounds/BGM_arrows.wav");
	throw_SE = LoadSoundMem("Resource/sounds/pan.wav");
	explosion_SE = LoadSoundMem("Resource/sounds/explosion.wav");
	timeup_SE = LoadSoundMem("Resource/sounds/BGM_timeup.wav");

	//�t�@�C���p�X
	FILE* fp = NULL;

	//�Ǎ��݃t�@�C�����J��
	fopen_s(&fp, SCORE_DATA, "r");

	//�G���[�`�F�b�N
	if (fp == NULL)
	{
		throw("�t�@�C�����ǂݍ��߂܂���\n");
	}
	else
	{
		//�n�C�X�R�A��ǂݍ���
		fscanf_s(fp,"%d",&highscore);

		//�t�@�C�������
		fclose(fp);
	}

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

	image = number[0];

	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP);

}

//�X�V����
void Scene::Update()
{
	//�t���[���J�E���g�����Z����
	frame_count++;

	//�U�O�t���[���ڂɓ��B������
	if (frame_count == 60)
	{
		//�J�E���g�̃��Z�b�g
		if (time > 0)
		{
			time--;
			frame_count = 0;
		}
		
	}

	//�������ԂɂȂ�����
	if (time <= 0)
	{		
		if (timeup == false)
		{
			StopSoundMem(background_sound);
			//DeleteSoundMem(background_sound);
			PlaySoundMem(timeup_SE, DX_PLAYTYPE_BACK);
			Finalize();
			frame_count = 0;

			p = CreateObject<Player>(Vector2D(320.0f, 50.0f), TYPE::PLAYER);

			timeup = true;
		}

		if (frame_count == 180)
		{
			if (score >= 3000)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::PERFECT);
			}
			else if (score >= 1500)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::GOOD);
			}
			else if (score >= 1000)
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::OK);
			}
			else
			{
				CreateObject<Evaluation>(Vector2D(320.0f, 240.0f), TYPE::BAD);
			}
		}
		
		//�X�y�[�X�L�[�������ꂽ��A�{���𐶐�����
		if (InputControl::GetKeyDown(KEY_INPUT_SPACE))
		{
			if (Bomb_count < 1)
			{
				CreateObject<Bomb>(objects[0]->GetLocation(), TYPE::BOMB)->SetPlayer(p);
				Bomb_count++;
				PlaySoundMem(throw_SE, DX_PLAYTYPE_BACK);
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
				if (objects[i]->GetType() == 1)
				{
					Bomb_count--;

					//���j�G�t�F�N�g�̐���
					CreateObject<Explosion>(objects[i]->GetLocation(), TYPE::EXPLOSION);
					PlaySoundMem(explosion_SE, DX_PLAYTYPE_BACK);
				}
				//�I�u�W�F�N�g�̍폜
				objects.erase(objects.begin() + i);
			}
		}
	}
	//�������Ԃ��O����Ȃ��̂Ƃ�
	else
	{
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
			//��������Ԋu��������
			if (frame_count == 0)
			{
				//��������m���𒲐�
				if (GetRand(100) <= 20)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::BOX_ENEMY);
					Enemy_count[TYPE::BOX_ENEMY - 4] += 1;
				}
			}
		}
		//���̃e�L�𐶐�����
		if (Enemy_count[3] < 1)
		{
			//��������Ԋu��������
			if (time % 30 == 0)
			{
				//��������m���𒲐�
				if (GetRand(100) <= 1)
				{
					CreateObject<Enemy>(Vector2D(LocationX[GetRand(1)], LocationY[3]), TYPE::GORLD_ENEMY);
					Enemy_count[TYPE::GORLD_ENEMY - 4] += 1;
				}
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
				PlaySoundMem(throw_SE, DX_PLAYTYPE_BACK);
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
					PlaySoundMem(explosion_SE, DX_PLAYTYPE_BACK);
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
	DrawRotaGraphF(320.0f, 240.0f, 0.7, 0, background_image, TRUE, 0);

	//�V�[���ɑ��݂���I�u�W�F�N�g�̕`�揈��
	for (GameObject* obj:objects)
	{
		obj->Draw();
	}

	//�X�R�A�摜�̕`��
	DrawRotaGraphF(150.0f, 465.0f, 1.1, 0, score_image, TRUE, 0);
	//�n�C�X�R�A�摜�̕`��
	DrawRotaGraphF(350.0f, 465.0f, 1.2, 0, highscore_image, TRUE, 0);
	//�^�C�}�[�摜�̕`��
	DrawRotaGraphF(20.0f, 465.0f, 0.5, 0, timer_image, TRUE, 0);

	UIDraw();
}

//�I��������
void Scene::Finalize()
{
	//�t�@�C���p�X
	FILE* fp = NULL;

	if (score > highscore)
	{

		//�Ǎ��݃t�@�C�����J��
		fopen_s(&fp, SCORE_DATA, "w");

		//�G���[�`�F�b�N
		if (fp == NULL)
		{
			throw("�t�@�C�����ǂݍ��߂܂���\n");
		}
		else
		{
			//�n�C�X�R�A��ǂݍ���
			fprintf(fp,"%d",score);

			//�t�@�C�������
			fclose(fp);
		}
	}

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

		//�v���C���[���e�ɓ�����Ɛ������Ԃ����炷
		if (a->GetType() == TYPE::PLAYER && b->GetType() == TYPE::BULLET)
		{
			time -= 2;
		}
		//�n�[�s�[���{���ɓ�����ƃX�R�A�����炷
		if (a->GetType() == TYPE::HARPY && b->GetType() == TYPE::BOMB)
		{
			score -= 100;
		}
		//�n�l�e�L���{���ɓ�����ƃX�R�A�𑝂₷
		if (a->GetType() == TYPE::FLY_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 30;
		}
		//�n�R�e�L���{���ɓ�����ƃX�R�A�𑝂₷
		if (a->GetType() == TYPE::BOX_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 200;
		}
		//���̃e�L���{���ɓ�����ƃX�R�A�𑝂₷
		if (a->GetType() == TYPE::GORLD_ENEMY && b->GetType() == TYPE::BOMB)
		{
			score += 1500;
		}
	}
}

//UI�`�揈��
void Scene::UIDraw() const
{
	//�X�R�A����v�Z
	int s1;			//�X�R�A
	int i;			//�J�E���g�p�̕ϐ�

	//�X�R�A���}�C�i�X�������Ƃ�
	if (score < 0)
	{
		s1 = -score;
		i = 1;
	}
	else
	{
		s1 = score;
		i = 0;
	}

	int s2 = s1;		//��P�����Ȃ����l
	int s3 = 0;			//��ꌅ�̈ʂ̒l
	int digit = 0;		//���̐�
	int num;			//�ԍ�
	

	//�X�R�A�̌������m�F
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//�������ʂ��珇�ɏȂ��Ă���
	}

	//�X�R�A��`��
	while (digit > -1)
	{
		//��P�����Ȃ����l
		s2 = s1 % (int)pow(10, digit);

		//��ꌅ�̈ʂ̒l
		s3 = s1 - s2;

		//��ꌅ�̐�
		num = s3 / (int)pow(10, digit);

		//�}�C�i�X��`��
		if (score < 0)
		{
			DrawRotaGraphF(185.0f, 465.0f, 1.0, 0, minus_image, TRUE, 0);
		}

		//��ꌅ�̐���`��
		DrawRotaGraphF(185.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);
		
		//�ʂ����炷
		digit--;
		s1 = s2;
		i++;
	}
	
	//�n�C�X�R�A���v�Z
	s1 = highscore;
	s2 = s1;
	digit = 0;
	i = 0;

	//�n�C�X�R�A�̌������m�F
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//�������ʂ��珇�ɏȂ��Ă���
	}

	//�n�C�X�R�A��`��
	while (digit > -1)
	{
		//��P�����Ȃ����l
		s2 = s1 % (int)pow(10, digit);

		//��ꌅ�̈ʂ̒l
		s3 = s1 - s2;

		//��ꌅ�̐�
		num = s3 / (int)pow(10, digit);

		//��ꌅ�̐���`��
		DrawRotaGraphF(405.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);
		
		//�ʂ����炷
		digit--;
		s1 = s2;
		i++;
	}

	//�^�C�����v�Z
	s1 = time;
	s2 = s1;
	digit = 0;
	i = 0;

	//�^�C���̌������m�F
	while (s2 / 10 > 0)
	{
		digit++;
		s2 = s2 / 10;		//�������ʂ��珇�ɏȂ��Ă���
	}

	//�^�C����`��
	while (digit > -1)
	{
		//��P�����Ȃ����l
		s2 = s1 % (int)pow(10, digit);

		//��ꌅ�̈ʂ̒l
		s3 = s1 - s2;

		//��ꌅ�̐�
		num = s3 / (int)pow(10, digit);

		//��ꌅ�̐���`��
		DrawRotaGraphF(45.0f + (i * 16.0f), 465.0f, 0.1, 0, number[num], TRUE, 0);

		//�ʂ����炷
		digit--;
		s1 = s2;
		i++;
	}
}