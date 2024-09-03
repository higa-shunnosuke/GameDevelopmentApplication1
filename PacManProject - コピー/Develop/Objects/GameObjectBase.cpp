#include "GameObjectBase.h"
#include "../Scenes/SceneBase.h"
#include "DxLib.h"

GameObjectBase::GameObjectBase() :
	owner_scene(nullptr),
	location(0.0f),
	image(NULL),
	z_layer(0),
	mobility(eMobilityType::Stationary)
{

}

GameObjectBase::~GameObjectBase()
{

}

/// <summary>
/// 初期化処理
/// </summary>
void GameObjectBase::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレームあたりの時間</param>
void GameObjectBase::Update(float delta_second)
{

}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset">オフセット値</param>
void GameObjectBase::Draw(const Vector2D& screen_offset) const
{
	// オフセット値を基に画像の描画を行う
	Vector2D graph_location = this->location + screen_offset;
	DrawRotaGraphF(graph_location.x, graph_location.y, 1.0, 0.0, image, TRUE);
}

/// <summary>
/// 終了時処理
/// </summary>
void GameObjectBase::Finalize()
{

}

/// <summary>
/// 当たり判定通知処理
/// </summary>
/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
void GameObjectBase::OnHitCollision(GameObjectBase* hit_object)
{

}

/// <summary>
/// 所有シーン情報の設定
/// </summary>
/// <param name="scene">所有シーン情報</param>
void GameObjectBase::SetOwnerScene(SceneBase* scene)
{
	this->owner_scene = scene;
}

/// <summary>
/// 位置座標取得処理
/// </summary>
/// <returns>位置座標情報</returns>
const Vector2D& GameObjectBase::GetLocation() const
{
	return location;
}

/// <summary>
/// 位置情報変更処理
/// </summary>
/// <param name="location">変更したい位置情報</param>
void GameObjectBase::SetLocation(const Vector2D& location)
{
	this->location = location;
}

/// <summary>
/// 当たり判定取得処理
/// </summary>
/// <returns>当たり判定情報</returns>
CapsuleCollision GameObjectBase::GetCollision() const
{
	return collision;
}

/// <summary>
/// Zレイヤー情報取得処理
/// </summary>
/// <returns>Zレイヤー情報</returns>
const int GameObjectBase::GetZLayer() const
{
	return z_layer;
}

/// <summary>
/// 可動性情報の取得処理
/// </summary>
/// <returns>可動性情報</returns>
const eMobilityType GameObjectBase::GetMobility() const
{
	return mobility;
}
