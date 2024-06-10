#pragma once

#include"../GameObject.h"
#include"../Player/Player.h"

class Bullet :public GameObject
{
private:
	int animation[5];			//アニメーション画像
	int frame_count;			//アニメーション時間
	int animation_max;			//アニメーション画像の枚数
	int count;					//アニメーション画像のカウント
	Vector2D vector;			//移動方向
	Player* player;				//プレイヤーのポインター

public:
	Bullet();
	~Bullet();

	virtual void Initialize(int object_type) override;	//初期化処理
	virtual void Update() override;						//更新処理
	virtual void Draw() const override;					//描画処理
	virtual void Finalize() override;					//終了時処理

	//当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object) override;
	//削除判定通知処理
	virtual bool Delete() override;
	//プレイヤーのポインタを受け取る
	virtual void SetPlayer(class Player* player);
	//タイプ取得処理
	virtual int GetType() override;

private:
	//移動処理
	void Movement();
	//アニメーション制御
	void AnimeControl();


};

