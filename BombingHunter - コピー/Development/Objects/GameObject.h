#pragma once

#include"../Utility/Vector2D.h"
#include"../Utility/ResourceManager.h"
#include"vector"

#define	D_PIVOT_CENTER		//座標の原点を画像の中心にする

//ゲームオブジェクト基底クラス
class GameObject
{
protected:
	std::vector<int> tmp;		//データ保存用の変数

	Vector2D location;	//位置情報
	Vector2D box_size;	//大きさ
	double radian;		//向き
	int image;			//描画する画像
	int sound;			//再生する音源
	int type;			//オブジェクトのタイプ

public:
	GameObject();
	virtual ~GameObject();

	virtual void Initialize(int object_type);//初期化処理
	virtual void Update();		//更新処理
	virtual void Draw() const;	//描画処理
	virtual void Finalize();	//終了時処理

	//当たり判定通知処理
	virtual void OnHitCollision(GameObject* hit_object);
	//削除判定通知処理
	virtual bool Delete();

	//タイプ取得処理
	virtual int GetType();

	//位置情報取得処理
	Vector2D GetLocation() const;

	//位置情報変更処理
	void SetLocation(const Vector2D& location);

	//当たり判定の大きさを取得する
	Vector2D GetBoxSize() const;
};

