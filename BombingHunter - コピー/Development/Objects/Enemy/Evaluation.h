#pragma once

#include"../GameObject.h"

class Evaluation :public GameObject
{
private:
	int SE;		//���ʉ�

public:
	Evaluation();
	~Evaluation();

	virtual void Initialize(int object_type) override;	//����������
	virtual void Update() override;		//�X�V����
	virtual void Draw() const override;	//�`�揈��
	virtual void Finalize() override;	//�I��������
};

