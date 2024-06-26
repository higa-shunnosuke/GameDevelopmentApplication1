#pragma once

#include"../GameObject.h"

class Evaluation :public GameObject
{
private:
	int SE;		//効果音

public:
	Evaluation();
	~Evaluation();

	virtual void Initialize(int object_type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理
};

