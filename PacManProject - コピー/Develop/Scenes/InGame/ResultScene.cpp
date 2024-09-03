#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{

}

eSceneType ResultScene::Update(const float& delta_second)
{
	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);

	// シーン情報
	return GetNowSceneType();
}

void ResultScene::Draw() const
{
	// 親クラスの描画処理を呼び出す
	__super::Draw();
}

void ResultScene::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

const eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::result;
}
