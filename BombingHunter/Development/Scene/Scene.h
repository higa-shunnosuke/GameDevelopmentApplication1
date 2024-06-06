#pragma once

#include"vector"
#include"string"
#include"random"
#include"../Objects/GameObject.h"

class Scene
{
private:
	std::vector<GameObject*> objects;
	int frame_count;		//�t���[���J�E���g
	int time;				//�^�C�}�[
	float LocationX[2];		//�Œ�X���W
	float LocationY[4];		//�Œ�Y���W
	int image;				//�w�i�摜
	int Enemy_count[4];		//�G�̏o����
	int Bomb_count;			//�{���̐�


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

	//�I�u�W�F�N�g��������
	template <class T>
	T* CreateObject(const Vector2D& location,int type)
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
		new_object->Initialize(type);

		//�I�u�W�F�N�g���X�g�ɒǉ�
		objects.push_back(new_object);

		//�C���X�^���X�̃|�C���^��ԋp
		return new_instance;
	}
};

