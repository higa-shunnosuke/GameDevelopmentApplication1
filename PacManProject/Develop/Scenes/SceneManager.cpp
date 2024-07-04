#include "SceneManager.h"
#include "../Utility/InputManager.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/ProjectConfig.h"
#include "DxLib.h"

#include "Title/TitleScene.h"
#include "InGame/InGameScene.h"
#include "InGame/ResultScene.h"

SceneManager::SceneManager() :
	current_scene(nullptr)
{

}

SceneManager::~SceneManager()
{
	// 解放忘れ防止
	Shutdown();
}

/// <summary>
/// 初期化処理
/// </summary>
void SceneManager::WakeUp()
{
	// ウィンドウモードで起動する
	ChangeWindowMode(TRUE);

	// ウィンドウサイズの設定
	SetGraphMode(D_WIN_MAX_X, D_WIN_MAX_Y, D_COLOR_BIT);

	// ウィンドウタイトルの設定
	SetWindowText("Game Development Pac-Man 2024");

	// 垂直同期を行わない
	SetWaitVSyncFlag(FALSE);

	// Log.txtファイルの生成制御（Debugモードのみ生成する）
#if _DEBUG
	SetOutApplicationLogValidFlag(TRUE);
#else
	SetOutApplicationLogValidFlag(FALSE);
#endif // _DEBUG

	// Dxライブラリの初期化
	if (DxLib_Init() == D_FAILURE)
	{
		throw std::string("Dxライブラリの初期化に失敗しました！\n");
	}

	// 描画先を表画面に反映する
	SetDrawScreen(DX_SCREEN_BACK);

	// 最初のシーンをタイトル画面にする
	ChangeScene(eSceneType::title);

	// 非アクティブ状態でも動作させる
	SetAlwaysRunFlag(TRUE);
}

/// <summary>
///  実行処理
/// </summary>
void SceneManager::Run()
{
	// 入力情報を取得する
	InputManager* input = InputManager::GetInstance();

	// メインループ
	while (ProcessMessage() == D_SUCCESS)
	{
		// 入力情報の更新
		input->Update();
		

		// フレームレートの制御
		FreamControl();

		// シーンの更新
		eSceneType next_scene_type = current_scene->Update(GetDeltaSecond());

		// 描画処理
		Graph();

		// ゲームを終了するか確認する
		if ((next_scene_type == eSceneType::exit) ||
			(input->GetButtonUp(XINPUT_BUTTON_BACK)) ||
			(input->GetKeyUp(KEY_INPUT_ESCAPE)))
		{
			break;
		}

		// 現在のシーンタイプが次のシーンタイプと違っているか？
		if (current_scene->GetNowSceneType() != next_scene_type)
		{
			// シーン切り替え処理
			ChangeScene(next_scene_type);
		}
	}
}

/// <summary>
/// 終了時処理
/// </summary>
void SceneManager::Shutdown()
{
	// シーン情報が生成されていれば、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}

	// Singletonのインスタンスを解放する
	InputManager::DeleteInstance();
	ResourceManager::DeleteInstance();

	// Dxライブラリの使用を終了する
	DxLib_End();
}

/// <summary>
/// 描画処理
/// </summary>
void SceneManager::Graph() const
{
	// 画面の初期化
	ClearDrawScreen();

	// シーンの描画処理
	current_scene->Draw();

	// 裏画面の内容を表画面に反映する
	ScreenFlip();
}

/// <summary>
/// シーン切り替え処理
/// </summary>
/// <param name="next_type">次のシーンタイプ</param>
void SceneManager::ChangeScene(eSceneType next_type)
{
	// 次のシーンを生成する
	SceneBase* next_scene = CreateScene(next_type);

	// エラーチェック
	if (next_scene == nullptr)
	{
		throw ("シーンが生成できませんでした\n");
	}

	// シーン情報が格納されていたら、削除する
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// 次のシーンの初期化
	next_scene->Initialize();

	// 現在シーンの上書き
	current_scene = next_scene;
}

/// <summary>
/// シーン生成処理
/// </summary>
/// <param name="next_type">次のシーンタイプ</param>
/// <returns>生成したシーン情報のポインタ</returns>
SceneBase* SceneManager::CreateScene(eSceneType next_type)
{
	// シーンタイプによって、生成するシーンを切り替える
	switch (next_type)
	{
		case eSceneType::title:
			return dynamic_cast<SceneBase*>(new TitleScene());
		case eSceneType::in_game:
		case eSceneType::re_start:
			return dynamic_cast<SceneBase*>(new InGameScene());
		case eSceneType::result:
			return dynamic_cast<SceneBase*>(new ResultScene());
		default:
			return nullptr;
	}
}
