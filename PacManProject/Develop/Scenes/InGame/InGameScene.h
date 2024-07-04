#pragma once

#include "../SceneBase.h"

class InGameScene : public SceneBase
{
private:
	class Player* player;		// プレイヤー情報
	int back_ground_image;		// 背景画像
	int back_ground_sound;		// BGM
	bool pause_flag;			// 一時停止フラグ

public:
	InGameScene();
	virtual ~InGameScene();

	virtual void Initialize() override;
	virtual eSceneType Update(const float& delta_second) override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	// 現在のシーンタイプ情報を取得する
	virtual const eSceneType GetNowSceneType() const override;

	/// <summary>
	/// 当たり判定確認処理
	/// </summary>
	/// <param name="target">1つ目のゲームオブジェクト</param>
	/// <param name="partner">2つ目のゲームオブジェクト</param>
	virtual void CheckCollision(GameObjectBase* target, GameObjectBase* partner) override;

private:
	/// <summary>
	/// ステージマップ（壁）読み込み処理
	/// </summary>
	void LoadStageMapCSV();
	/// <summary>
	/// ステージマップ餌読み込み処理
	/// </summary>
	void LoadStageMapFoodsCSV();
};

