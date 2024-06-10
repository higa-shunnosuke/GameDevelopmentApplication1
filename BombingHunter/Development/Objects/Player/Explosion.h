#pragma once

#include"../GameObject.h"

class Explosion :public GameObject
{
private:
	int animation[3];		//アニメーション画像
	int frame_count;		//アニメーション時間
	int animation_max;		//アニメーション画像の枚数
	int count;				//アニメーション画像のカウント
	double radian;			//向き
	bool Is_anim;			//アニメーション終了フラグ

public:
	Explosion();
	~Explosion();

	virtual void Initialize(int object_type) override;	//初期化処理
	virtual void Update() override;		//更新処理
	virtual void Draw() const override;	//描画処理
	virtual void Finalize() override;	//終了時処理

	//削除判定通知処理
	virtual bool Delete() override;
	//タイプ取得処理
	virtual int GetType() override;

private:
	//アニメーション制御
	void AnimeControl();

};

