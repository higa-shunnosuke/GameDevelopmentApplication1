#pragma once

#include"../GameObject.h"
#include"vector"


class Player:public GameObject
{
private:
	int frame_count;			//�A�j���[�V��������
	int flip_flag;				//���]�t���O
	bool Is_hit;				//�e�ɓ���������
	int direction;				//����
	std::vector<int> animation;	//�A�j���[�V�����f�[�^

public:
	Player();
	~Player();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������

	//�����蔻��ʒm����
	virtual void OnHitCollision(GameObject* hit_object) override;

	//�����擾����
	int GetDirection() const;

private:
	//�ړ�����
	void Movement();
	//�A�j���[�V��������
	void AnimeControl();

};

