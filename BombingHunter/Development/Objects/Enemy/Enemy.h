#pragma once

#include"../GameObject.h"
#include"../Player/Player.h"

class Enemy :public GameObject
{
private:
	int animation[5];			//�A�j���[�V�����摜
	int frame_count;			//�A�j���[�V��������
	int animation_max;			//�A�j���[�V�����摜�̖���
	int count;					//�A�j���[�V�����摜�̃J�E���g
	Vector2D vector;			//�ړ�����
	float speed;				//�ړ����x
	int type;					//�G�̃^�C�v
	Player* player;

public:
	Enemy();
	~Enemy();

	virtual void Initialize(int type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������

	//�����蔻��ʒm����
	virtual void OnHitCollision(GameObject* hit_object) override;
	//�폜����ʒm����
	virtual bool Delete() override;
	//�v���C���[�̃|�C���^���󂯎��
	virtual void SetPlayer(class Player* player);
	//�^�C�v�擾����
	int GetType();

private:
	//�ړ�����
	void Movement();
	//�A�j���[�V��������
	void AnimeControl();

};


