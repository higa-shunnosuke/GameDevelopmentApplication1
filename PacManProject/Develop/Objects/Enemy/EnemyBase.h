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
	enum eDirectionState : unsigned char
	{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};
private:
	std::map<eAdjacentDirection, ePanelID> adjacent_panel;	//隣接するパネルのデータ
	unsigned char		now_panel;							//現在のパネルデータ
	std::vector<int>	move_animation;						//体のアニメーション画像
	std::vector<int>	eyes_animation;						//目のアニメーション画像
	float				animation_time;		//アニメーション時間
	int					animation_count;	//アニメーション添え字
	bool				is_start;			//状態維持時間のカウント開始フラグ
	float				time;				//状態維持時間
	int					time_count;			//状態維持添え字
	int					flash_count;		//点滅カウント用の変数
	bool				is_flash;			//点滅フラグ
	EnemyBase*			enemy;				//エネミー情報
	int					x, y;				//パネル保存用の変数
	int					go_x,go_y;			//目的地のパネル保存用変数
	int					dot_counter;		//ドットカウンター
	int					dot_limit;			//ドット制限
	int					power_counter;		//パワー餌カウンター
	bool				is_speed_down;		//減速フラグ
	bool				is_speed_up;		//加速フラグ
	bool				is_turn;			//ターンフラグ
	float				speed;				//移動量
	
protected:
	Vector2D			velocity;			//移動方向
	eEnemyType			enemy_type;			//エネミーの種類
	eEnemyState			old_state;			//エネミー状態
	eEnemyState			now_state;			//エネミー状態
	eDirectionState		direction_state;	//進行方向状態
	Player*				player;				//プレイヤー情報
	bool				is_death;			//死亡フラグ

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
	/// プレイヤーのポインタ受け取り処理
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player);

	/// <summary>
	/// エネミー種類設定処理
	/// </summary>
	/// <param name="type">エネミーの種類</param>
	void SetType();

	/// <summary>
	/// エネミー状態変更処理
	/// </summary>
	void ChangeState();

	//死亡フラグ設定処理
	void SetDeathFlag(bool is_death);
	//死亡フラグ取得処理
	bool GetDeathFlag();

private:
	/// <summary>
	/// 持ち時間制御
	/// </summary>
	void TimeControl(float delta_second);

	/// <summary>
	/// アニメーション制御
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void AnimationControl(float delta_second);

	/// <summary>
	/// 移動量設定処理
	/// </summary>
	void SetVelocity();

	/// <summary>
	/// 方向設定処理
	/// </summary>
	/// <param name="direction">進行方向</param>
	void SetDirection(eDirectionState direction);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	void Movement(float delta_second);

	/// <summary>
	/// 移動処理
	/// </summary>
	void WaitMovement();

	/// <summary>
	/// 移動処理
	/// </summary>
	void ScatterMovement();

	/// <summary>
	/// 移動処理
	/// </summary>
	void FrightenedMovement();

	/// <summary>
	/// 移動処理
	/// </summary>
	void EscapeMovement();

	/// <summary>
	/// 目標設定処理
	/// </summary>
	void SetDestination();

protected:
	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void TrackingMovement();
};