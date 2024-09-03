#include "TitleScene.h"
#include "../../Utility/InputManager.h"
#include "DxLib.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{

}

eSceneType TitleScene::Update(const float& delta_second)
{
	// 入力情報を取得
	InputManager* input = InputManager::GetInstance();

	// インゲームシーンに遷移する
	if (input->GetKeyDown(KEY_INPUT_SPACE))
	{
		return eSceneType::in_game;
	}
	if (input->GetButtonDown(XINPUT_BUTTON_START))
	{
		return eSceneType::in_game;
	}

	// 親クラスの更新処理を呼び出す
	return __super::Update(delta_second);
}

void TitleScene::Draw() const
{
	// 親クラスの描画処理を呼び出す
	__super::Draw();

	// UIの描画
	SetFontSize(60);
	DrawFormatString(120, 140, GetColor(255, 255, 0), "P A C - M A N");
	SetFontSize(40);
	DrawFormatString(10, 640, GetColor(255, 0, 0), "Space key pressed game start");
	SetFontSize(16);
}

void TitleScene::Finalize()
{
	// 親クラスの終了時処理を呼び出す
	__super::Finalize();
}

const eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::title;
}
