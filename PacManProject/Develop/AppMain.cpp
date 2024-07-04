/****************************************************************
* R6ゲーム開発応用Ⅰ
* パックマンの敵AIを作ろう！
*****************************************************************/
#include "DxLib.h"
#include "Utility/ProjectConfig.h"
#include "Scenes/SceneManager.h"

// メイン関数（プログラムはここから始まります）
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		// シーンマネージャーを生成する
		SceneManager manager;

		// 初期化処理
		manager.WakeUp();
		
		// 実行処理
		manager.Run();

		// 終了時処理
		manager.Shutdown();
	}
	catch (std::string error_log)
	{
		// エラー内容を出力する
		return ErrorThrow(error_log);
	}

	// 終了状態を通知する
	return D_SUCCESS;
}