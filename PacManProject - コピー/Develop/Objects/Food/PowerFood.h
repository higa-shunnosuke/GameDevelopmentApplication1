#pragma once

#include "../GameObject.h"

// パワー餌クラス
class PowerFood : public GameObject
{
private:
	bool is_disp;
	float disp_time;

public:
	PowerFood();
	virtual ~PowerFood();

	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw(const Vector2D& screen_offset) const override;

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

};