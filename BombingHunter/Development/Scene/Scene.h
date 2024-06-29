#pragma once

#include"vector"
#include"string"
#include"random"
#include"../Objects/GameObject.h"

typedef enum {
	PLAYER,
	BOMB,
	EXPLOSION,
	BULLET,
	HARPY,
	FLY_ENEMY,
	BOX_ENEMY,
	GORLD_ENEMY,
} TYPE;

class Scene
{
private:
	std::vector<GameObject*> objects;
	int frame_count;		//�t���[���J�E���g
	int time;				//�^�C�}�[
	float LocationX[2];		//�Œ�X���W
	float LocationY[4];		//�Œ�Y���W
	int Enemy_count[4];		//�G�̏o����
	int Bomb_count;			//�{���̐�
	int background_image;	//�w�i�摜
	int timer_image;		//�^�C�}�[�摜
	int highscore_image;	//�n�C�X�R�A�摜
	int score_image;		//�X�R�A�摜
	int number[11];			//�����摜
	int score;				//�X�R�A
	int highscore;			//�n�C�X�R�A
	bool Is_pause;			//�|�[�Y�t���O


public:
	Scene();
	~Scene();

	void Initialize();
	void Update();
	void Draw() const;
	void Finalize();

private:
	//�����蔻��`�F�b�N����
	void HitCheckObject(GameObject* a, GameObject* b);

	//�X�R�A�`�揈��
	void UIDraw() const;

	//�I�u�W�F�N�g��������
	template <class T>
	T* CreateObject(const Vector2D& location,int object_type)
	{
		//�C���X�^���X�𐶐�����
		T* new_instance = new T();
		//�w�肵���N���X�𐶐�����
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		//�G���[�`�F�b�N
		if (new_object==nullptr)
		{
			delete new_instance;
			throw std::string("�Q�[���I�u�W�F�N�g�������ł��܂���ł���");
		}

		//�ʒu���̐ݒ�
		new_object->SetLocation(location);

		//����������
		new_object->Initialize(object_type);

		//�I�u�W�F�N�g���X�g�ɒǉ�
		objects.push_back(new_object);

		//�C���X�^���X�̃|�C���^��ԋp
		return new_instance;
	}
};

