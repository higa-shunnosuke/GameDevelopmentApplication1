#pragma once

#include"../GameObject.h"
#include"../Player/Player.h"
#include "Explosion.h"

class Bomb :public GameObject
{
private:
	int animation[5];			//�A�j���[�V�����摜
	int frame_count;			//�A�j���[�V��������
	int animation_max;			//�A�j���[�V�����摜�̖���
	int count;					//�A�j���[�V�����摜�̃J�E���g
	Vector2D vector;			//�ړ�����
	bool Is_hit;				//�q�b�g������
	Player* player;				//�v���C���[�̃|�C���^
	Explosion* explosion;		//�����̃|�C���^

public:
	Bomb();
	~Bomb();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������

	//�����蔻��ʒm����
	virtual void OnHitCollision(GameObject* hit_object) override;
	//�폜����ʒm����
	virtual bool Delete() override;
	//�v���C���[�̃|�C���^���󂯎��
	virtual void SetPlayer(class Player* player);

private:
	//�ړ�����
	void Movement();

};

