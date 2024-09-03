#include "Collision.h"
#include <math.h>

/// <summary>
/// 適用オブジェクトか確認する処理
/// </summary>
/// <param name="hit_object">相手のオブジェクトタイプ</param>
/// <returns>適用するオブジェクトなら、true</returns>
bool CapsuleCollision::IsCheckHitTarget(eObjectType hit_object) const
{
	// 適用するオブジェクトタイプなら、true
	for (eObjectType type : hit_object_type)
	{
		if (type == hit_object)
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// 円と円の当たり判定確認処理
/// </summary>
/// <param name="c1">円の情報1</param>
/// <param name="c2">円の情報2</param>
/// <returns>当たっているなら、true</returns>
bool IsCheckCollision(const CircleCollision& c1, const CircleCollision& c2)
{
	// 2点間の距離と長さ
	Vector2D diff = (c1.point - c2.point);
	float len = diff.SqrLength();

	// 半径の長さ
	float r = (c1.radius + c2.radius) * (c1.radius + c2.radius);

	// 三平方の定理により、当たり判定を確認する
	if (len < r)
	{
		return true;
	}

	return false;
}
bool IsCheckCollision(const CapsuleCollision& c1, const CircleCollision& c2)
{
	CircleCollision circle;
	// 最近傍点を求める
	circle.point = NearPointCheck(c1, c2.point);
	// 半径情報の設定
	circle.radius = c1.radius;
	 
	// 円形同士の当たり判定
	return IsCheckCollision(circle, c2);
}
bool IsCheckCollision(const CapsuleCollision& c1, const CapsuleCollision& c2)
{
	CircleCollision circle;

	// c2の始点を円形として扱って、
	// c1カプセルと当たっているか確認する
	circle.radius = c2.radius;
	circle.point = c2.point[0];
	if (IsCheckCollision(c1, circle))
	{
		return true;
	}

	// c2の終点を円形として扱って、
	// c1カプセルと当たっているか確認する
	circle.radius = c2.radius;
	circle.point = c2.point[1];
	if (IsCheckCollision(c1, circle))
	{
		return true;
	}

	// c1の始点を円形として扱って、
	// c2カプセルと当たっているか確認する
	circle.radius = c1.radius;
	circle.point = c1.point[0];
	if (IsCheckCollision(c2, circle))
	{
		return true;
	}
	
	// c1の終点を円形として扱って、
	// c2カプセルと当たっているか確認する
	circle.radius = c1.radius;
	circle.point = c1.point[1];
	if (IsCheckCollision(c2, circle))
	{
		return true;
	}

	// 全ての条件が成立していない場合、当たっていない
	return false;
}

/// <summary>
/// 点とカプセルの線分の最近傍点を求める処理
/// </summary>
/// <param name="cap">カプセル情報</param>
/// <param name="point">円の中心点</param>
/// <returns>最近傍点</returns>
Vector2D NearPointCheck(const CapsuleCollision& cap, const Vector2D& point)
{
	// 線分の向きを表すベクトル
	Vector2D line = cap.point[1] - cap.point[0];
	// 線分の始点から点にベクトルを計算する
	Vector2D a1 = point - cap.point[0];
	// 線分の終点から点にベクトルを計算する
	Vector2D a2 = point - cap.point[1];

	// 点とカプセルの線分の最近傍点を求める
	if (Vector2D::Dot(line, a1) < 0.0f)
	{
		return cap.point[0];
	}
	if (Vector2D::Dot(line, a2) > 0.0f)
	{
		return cap.point[1];
	}
	
	// 線分との最近傍点を求める
	Vector2D n = line.Normalize();
	float dot = Vector2D::Dot(n, a1);
	return (cap.point[0] + (n * dot));
}
