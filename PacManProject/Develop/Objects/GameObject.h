#pragma once

#include "GameObjectBase.h"
#include "../Scenes/SceneBase.h"

// ゲームオブジェクトクラス
class GameObject : public GameObjectBase
{
public:
	/// <summary>
	/// ゲームオブジェクト生成処理
	/// </summary>
	/// <typeparam name="OBJECT">生成するゲームオブジェクトクラス</typeparam>
	/// <param name="generate_location">生成位置</param>
	/// <returns>生成したゲームオブジェクトのポインタ</returns>
	template <class OBJECT>
	OBJECT* CreateObject(const Vector2D& generate_location)
	{
		return owner_scene->CreateObject<OBJECT>(generate_location);
	}

	/// <summary>
	/// オブジェクト破棄処理
	/// </summary>
	/// <param name="target">破棄を行うゲームオブジェクトのポインタ</param>
	void DestroyObject(GameObjectBase* target)
	{
		owner_scene->DestroyObject(target);
	}
	
	/// <summary>
	/// スクリーンオフセット情報取得処理
	/// </summary>
	/// <returns>スクリーンオフセット値</returns>
	const Vector2D GetScreenOffset() const
	{
		return owner_scene->GetScreenOffset();
	}
};