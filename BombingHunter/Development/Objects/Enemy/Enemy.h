#pragma once

#include "../../Scene/Scene.h"
#include"../GameObject.h"

class Enemy :public GameObject
{
private:
	int animation[5];		//アニメーション画像
	int frame_count;		//アニメーション時間
	int animation_max;		//アニメーション画像の枚数
	int count;				//アニメーション画像のカウント
	bool flip_flag;			//画像反転フラグ
	Vector2D vector;		//移動方向
	float speed;			//移動速度
	bool Is_hit;			//ボムに当たったか
	bool Is_death;			//死んだか
	int BlendMode;			//ブレンド値
	int number[11];			//数字画像
	int minus_image;		//マイナス画像
	
public:
	Enemy();
	~Enemy();

	virtual void Initialize(int object_type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理

	//当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object) override;
	//削除判定通知処理
	virtual bool Delete() override;
	//タイプ取得処理
	virtual int GetType() override;

private:
	//移動処理
	void Movement();
	//アニメーション制御
	void AnimeControl();

};


