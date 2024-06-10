#pragma once

#include"../GameObject.h"

class Explosion :public GameObject
{
private:
	int animation[3];		//�A�j���[�V�����摜
	int frame_count;		//�A�j���[�V��������
	int animation_max;		//�A�j���[�V�����摜�̖���
	int count;				//�A�j���[�V�����摜�̃J�E���g
	double radian;			//����
	bool Is_anim;			//�A�j���[�V�����I���t���O

public:
	Explosion();
	~Explosion();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������

	//�폜����ʒm����
	virtual bool Delete() override;
	//�^�C�v�擾����
	virtual int GetType() override;

private:
	//�A�j���[�V��������
	void AnimeControl();

};

