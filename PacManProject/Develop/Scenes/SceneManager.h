#pragma once

#include "SceneBase.h"

class SceneManager
{
private:
	SceneBase* current_scene;	// 現在のシーン情報

public:
	SceneManager();
	~SceneManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void WakeUp();

	/// <summary>
	///  実行処理
	/// </summary>
	void Run();

	/// <summary>
	/// 終了時処理
	/// </summary>
	void Shutdown();

private:
	/// <summary>
	/// 描画処理
	/// </summary>
	void Graph() const;

	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	void ChangeScene(eSceneType next_type);

	/// <summary>
	/// シーン生成処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	/// <returns>生成したシーン情報のポインタ</returns>
	SceneBase* CreateScene(eSceneType next_type);
};