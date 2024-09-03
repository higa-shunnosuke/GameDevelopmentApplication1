#include "Food.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"

Food::Food()
{

}

Food::~Food()
{

}

void Food::Initialize()
{
	// 画像の読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	image = rm->GetImages("Resource/Images/dot.png")[0];

	// 当たり判定の設定
	collision.is_blocking = false;
	collision.object_type = eObjectType::food;
	collision.hit_object_type.push_back(eObjectType::player);
	collision.radius = 1.0f;

	z_layer = 1;
}

void Food::Draw(const Vector2D& screen_offset) const
{
	// オフセット値を基に画像の描画を行う
	Vector2D graph_location = this->location + screen_offset;
	DrawRotaGraphF(graph_location.x, graph_location.y, 0.2, 0.0, image, TRUE);
}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void Food::OnHitCollision(GameObjectBase* hit_object)
{
	// 当たったオブジェクトがプレイヤーだったら、削除する
	if (hit_object->GetCollision().object_type == eObjectType::player)
	{
		DestroyObject(this);
	}
}
