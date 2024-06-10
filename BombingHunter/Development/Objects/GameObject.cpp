#include"GameObject.h"
#include"DxLib.h"

//コンストラクタ
GameObject::GameObject() : 
	location(0.0f),
	box_size(0.0f),
	radian(0.0),
	image(NULL),
	sound(NULL)
{

}

//デストラクタ
GameObject::~GameObject()
{

}

//初期化処理
void GameObject::Initialize(int object_type)
{
	type = object_type;
}

//更新所理
void GameObject::Update()
{

}

//描画処理
void GameObject::Draw() const
{
#if _DEBUG
	//当たり判定の可視化

	Vector2D tl = location - (box_size / 2.0f);
	Vector2D br = location + (box_size / 2.0f);

	DrawBoxAA(tl.x, tl.y, br.x, br.y, GetColor(255, 0, 0), FALSE);
#endif
}

//終了時処理
void GameObject::Finalize()
{

}

//当たり判定通知処理
void GameObject::OnHitCollision(GameObject* hit_object)
{
	//当たった時に行う処理
}

//削除判定通知処理
bool GameObject::Delete()
{
	//削除時に行う処理
	bool ret = false;
	return ret;
}

//タイプ取得処理
int GameObject::GetType()
{
	return this->type;
}

//位置情報取得処理
Vector2D GameObject::GetLocation() const
{
	return this->location;
}

//位置情報設定処理
void GameObject::SetLocation(const Vector2D& location)
{
	this->location = location;
}

//当たり判定の大きさを取得する
Vector2D GameObject::GetBoxSize() const
{
	return box_size;
}