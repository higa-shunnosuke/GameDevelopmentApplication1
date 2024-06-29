#pragma once

#include "../../Scene/Scene.h"
#include"../GameObject.h"

class Enemy :public GameObject
{
private:
	int animation[5];		//�A�j���[�V�����摜
	int frame_count;		//�A�j���[�V��������
	int animation_max;		//�A�j���[�V�����摜�̖���
	int count;				//�A�j���[�V�����摜�̃J�E���g
	bool flip_flag;			//�摜���]�t���O
	Vector2D vector;		//�ړ�����
	float speed;			//�ړ����x
	bool Is_hit;			//�{���ɓ���������
	bool Is_death;			//���񂾂�
	int BlendMode;			//�u�����h�l
	int number[11];			//�����摜
	int minus_image;		//�}�C�i�X�摜
	
public:
	Enemy();
	~Enemy();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������

	//�����蔻��ʒm����
	virtual void OnHitCollision(GameObject* hit_object) override;
	//�폜����ʒm����
	virtual bool Delete() override;
	//�^�C�v�擾����
	virtual int GetType() override;

private:
	//�ړ�����
	void Movement();
	//�A�j���[�V��������
	void AnimeControl();

};


