#pragma once

#include"../GameObject.h"

class Explosion :public GameObject
{
private:
	int animation[3];		//アニメーション画像
	int frame_count;		//アニメーション時間
	int animation_max;		//アニメーション画像の枚数
	int count;				//アニメーション画像のカウント
	int image;				//描画する画像
	double radian;			//向き

public:
	Explosion(Vector2D location);
	~Explosion();

	virtual void Initialize(int type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理

	//削除判定通知処理
	virtual bool Delete() override;

private:
	//アニメーション制御
	void AnimeControl();

};

