#pragma once

#include"vector"
#include"string"
#include"random"
#include"../Objects/GameObject.h"

typedef enum {
	PLAYER,
	BOMB,
	EXPLOSION,
	BULLET,
	HARPY,
	FLY_ENEMY,
	BOX_ENEMY,
	GORLD_ENEMY,
	PERFECT,
	GOOD,
	OK,
	BAD,
} TYPE;

class Scene
{
private:
	std::vector<GameObject*> objects;
	int frame_count;		//フレームカウント
	int time;				//タイマー
	float LocationX[2];		//固定X座標
	float LocationY[4];		//固定Y座標
	int Enemy_count[4];		//敵の出現数
	int Bomb_count;			//ボムの数
	int background_image;	//背景画像
	int background_sound;	//背景BGM
	int timer_image;		//タイマー画像
	int highscore_image;	//ハイスコア画像
	int score_image;		//スコア画像
	int minus_image;		//マイナス画像
	int number[11];			//数字画像
	int score;				//スコア
	int highscore;			//ハイスコア
	bool Is_pause;			//ポーズフラグ
	int throw_SE;			//ボムを投げるSE
	int explosion_SE;		//爆発SE
	int timeup_SE;			//タイムアップSE
	bool timeup;			//タイムアップフラグ

public:
	Scene();
	~Scene();

	void Initialize();
	void Update();
	void Draw() const;
	void Finalize();

private:
	//当たり判定チェック処理
	void HitCheckObject(GameObject* a, GameObject* b);

	//スコア描画処理
	void UIDraw() const;

	//オブジェクト生成処理
	template <class T>
	T* CreateObject(const Vector2D& location,int object_type)
	{
		//インスタンスを生成する
		T* new_instance = new T();
		//指定したクラスを生成する
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		//エラーチェック
		if (new_object==nullptr)
		{
			delete new_instance;
			throw std::string("ゲームオブジェクトが生成できませんでした");
		}

		//位置情報の設定
		new_object->SetLocation(location);

		//初期化処理
		new_object->Initialize(object_type);

		//オブジェクトリストに追加
		objects.push_back(new_object);

		//インスタンスのポインタを返却
		return new_instance;
	}
};

