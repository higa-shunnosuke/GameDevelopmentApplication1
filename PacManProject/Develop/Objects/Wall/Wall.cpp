#include "Wall.h"
#include "DxLib.h"

Wall::Wall()
{

}

Wall::~Wall()
{

}

void Wall::Initialize()
{
	// 当たり判定の設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::wall;
	collision.hit_object_type.push_back(eObjectType::player);
	collision.hit_object_type.push_back(eObjectType::enemy);
	collision.radius = D_OBJECT_SIZE / 2.0f;
}

void Wall::Draw(const Vector2D& screen_offset) const
{

}

void Wall::SetWallData(int x_size, int y_size)
{
	// 当たり判定のサイズ設定
	collision.point[0] = 0.0f;
	collision.point[1] = 0.0f;

	float a = (D_OBJECT_SIZE / 2) - (D_OBJECT_SIZE / 8.0f);
	if (x_size == 1)
	{
		collision.point[1].y = D_OBJECT_SIZE * (float)(y_size - 1);
	}
	if (y_size == 1)
	{
		collision.point[1].x = D_OBJECT_SIZE * (float)(x_size - 1);
	}
}
