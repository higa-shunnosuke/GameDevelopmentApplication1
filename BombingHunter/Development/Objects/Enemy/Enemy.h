#pragma once

#include"../GameObject.h"

class Enemy :public GameObject
{
private:
	int animation[5];			//アニメーション画像
	int frame_count;			//アニメーション時間
	int animation_max;			//アニメーション画像の枚数
	Vector2D velocity;			//移動速度
	int count;					//アニメーション画像のカウント
public:
	Enemy();
	~Enemy();

	virtual void Initialize(int type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理

	//当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object) override;

private:
	//移動処理
	void Movement();
	//アニメーション制御
	void AnimeControl();

};


