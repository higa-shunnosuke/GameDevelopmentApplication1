#pragma once

#include "Vector2D.h"
#include <vector>

// オブジェクトタイプ
enum class eObjectType : unsigned char
{
	none,
	player,
	enemy,
	wall,
	food ,
	power_food,
	special,
};

// 当たり判定基底クラス
class CapsuleCollision
{
public:
	bool						is_blocking;		// すり抜けフラグ
	eObjectType					object_type;		// 自身のオブジェクトタイプ
	std::vector<eObjectType>	hit_object_type;	// 適用するオブジェクトタイプ
	float						radius;				// 円の半径
	Vector2D					point[2];			// 始点と終点（相対座標）

public:
	CapsuleCollision() :
		is_blocking(false),
		object_type(eObjectType::none),
		hit_object_type(),
		radius(0.0f)
	{
		point[0] = 0.0f;
		point[1] = 0.0f;
	}
	~CapsuleCollision()
	{
		hit_object_type.clear();
	}

	// 当たり判定有効確認処理
	bool IsCheckHitTarget(eObjectType hit_object) const;
};

// 円形状クラス
class CircleCollision
{
public:
	Vector2D point;
	float radius;

public:
	CircleCollision() :
		point(0.0f), radius(0.0f)
	{

	}
	~CircleCollision() = default;
};


/// <summary>
/// 当たり判定確認処理
/// </summary>
/// <param name="c1">形状の情報1</param>
/// <param name="c2">形状の情報2</param>
/// <returns>当たっているなら、true</returns>
bool IsCheckCollision(const CircleCollision& c1, const CircleCollision& c2);
bool IsCheckCollision(const CapsuleCollision& c1, const CircleCollision& c2);
bool IsCheckCollision(const CapsuleCollision& c1, const CapsuleCollision& c2);

/// <summary>
/// 点とカプセルの線分の最近傍点を求める処理
/// </summary>
/// <param name="cap">カプセル情報</param>
/// <param name="point">円の中心点</param>
/// <returns>最近傍点</returns>
Vector2D NearPointCheck(const CapsuleCollision& cap, const Vector2D& point);
