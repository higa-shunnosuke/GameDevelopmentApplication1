#pragma once

#include "../GameObject.h"
#include "../Player/Player.h"

//エネミー状態
enum eEnemyState
{
	WAIT,		//待機
	SCATTER,	//縄張り
	CHASE,		//追跡
	FRIGHTENED,	//いじけ
	ESCAPE,		//戻る
};


class EnemyBase : public GameObject
{
private:
	//エネミーの種類
	enum eEnemyType
	{
		blinky,	//アカベイ
		pinky,	//ピンキー
		inky,	//アオスケ
		clyde	//グズタ
	};

	//進行方向状態
	enum eDirectionState
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
private:
	std::vector<int>	move_animation;		//体のアニメーション画像
	std::vector<int>	eyes_animation;		//目のアニメーション画像
	float				animation_time;		//アニメーション時間
	int					animation_count;	//アニメーション添え字
	float				time;				//各状態の持ち時間
	int					flash_count;		//点滅カウント用の変数
	eEnemyType			enemy_type;			//エネミーの種類
	int					i;					//
	EnemyBase*			enemy;				//エネミー情報
	int					x, y;				//パネル保存用の変数

protected:
	Vector2D			velocity;			//移動量
	eEnemyState			enemy_state;		//エネミー状態
	eDirectionState		direction_state;	//進行方向状態
	Player*				player;				//プレイヤー情報
	
public:
	EnemyBase();
	virtual ~EnemyBase();

public:
	//初期化処理
	virtual void Initialize() override;
	//更新処理
	virtual void Update(float delta_second) override;
	//描画処理
	virtual void Draw(const Vector2D& screen_offset) const override;
	//終了時処理
	virtual void Finalize() override;

	/// <summary>
	/// 当たり判定通知処理
	/// </summary>
	/// <param name="hit_object">当たったゲームオブジェクトのポインタ</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

	/// <summary>
	/// エネミーの状態を取得する
	/// </summary>
	/// <returns>エネミーの状態</returns>
	eEnemyState GetEnemyState() const;

	/// <summary>
	/// プレイヤーのポインタ受け取り処理
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player);

	/// <summary>
	/// エネミー種類設定処理
	/// </summary>
	/// <param name="type">エネミーの種類</param>
	void SetType();

private:
	/// <summary>
	/// エネミー状態変更処理
	/// </summary>
	void ChangeState();

	
	/// <summary>
	/// 持ち時間制御
	/// </summary>
	void TimeControl();

	/// <summary>
	/// アニメーション制御
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void AnimationControl(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void Movement(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void WaitMovement(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void TerritoryMovement(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void ScaredMovement(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void EscapeMovement(float delta_second);

	/// <summary>
	/// 目標設定処理
	/// </summary>
	void SetDestination();

protected:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void TrackingMovement(float delta_second);
};