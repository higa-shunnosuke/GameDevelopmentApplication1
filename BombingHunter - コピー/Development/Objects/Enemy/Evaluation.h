#pragma once

#include"../GameObject.h"

class Evaluation :public GameObject
{
private:
	int SE;		//Œø‰Ê‰¹

public:
	Evaluation();
	~Evaluation();

	virtual void Initialize(int object_type) override;	//‰Šú‰»ˆ—
	virtual void Update() override;		//XVˆ—
	virtual void Draw() const override;	//•`‰æˆ—
	virtual void Finalize() override;	//I—¹ˆ—
};

