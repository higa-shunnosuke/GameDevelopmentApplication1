#pragma once

#include"vector"
#include"string"
#include"random"
#include"../Objects/GameObject.h"

class Scene
{
private:
	std::vector<GameObject*> objects;
	int frame_count;		//フレームカウント
	int time;				//タイマー
	float LocationX[2];		//固定X座標
	float LocationY[4];		//固定Y座標
	int image;				//背景画像
	int Enemy_count[4];		//敵の出現数
	int Bomb_count;			//ボムの数


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

	//オブジェクト生成処理
	template <class T>
	T* CreateObject(const Vector2D& location,int type)
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
		new_object->Initialize(type);

		//オブジェクトリストに追加
		objects.push_back(new_object);

		//インスタンスのポインタを返却
		return new_instance;
	}
};

