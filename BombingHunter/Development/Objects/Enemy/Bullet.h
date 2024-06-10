#pragma once

#include"../GameObject.h"
#include"../Player/Player.h"

class Bullet :public GameObject
{
private:
	int animation[5];			//�A�j���[�V�����摜
	int frame_count;			//�A�j���[�V��������
	int animation_max;			//�A�j���[�V�����摜�̖���
	int count;					//�A�j���[�V�����摜�̃J�E���g
	Vector2D vector;			//�ړ�����
	Player* player;				//�v���C���[�̃|�C���^�[

public:
	Bullet();
	~Bullet();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;						//�X�V����
	virtual void Draw() const override;					//�`�揈��
	virtual void Finalize() override;					//�I��������

	//�����蔻��ʒm����
	virtual void OnHitCollision(GameObject* hit_object) override;
	//�폜����ʒm����
	virtual bool Delete() override;
	//�v���C���[�̃|�C���^���󂯎��
	virtual void SetPlayer(class Player* player);
	//�^�C�v�擾����
	virtual int GetType() override;

private:
	//�ړ�����
	void Movement();
	//�A�j���[�V��������
	void AnimeControl();


};

