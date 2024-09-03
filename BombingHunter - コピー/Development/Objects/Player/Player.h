#pragma once

#include"../GameObject.h"
#include"vector"


class Player:public GameObject
{
private:
	int frame_count;			//アニメーション時間
	int flip_flag;				//反転フラグ
	bool Is_hit;				//弾に当たったか
	int direction;				//向き
	std::vector<int> animation;	//アニメーションデータ

public:
	Player();
	~Player();

	virtual void Initialize(int object_type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理

	//当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object) override;

	//向き取得処理
	int GetDirection() const;

private:
	//移動処理
	void Movement();
	//アニメーション制御
	void AnimeControl();

};

