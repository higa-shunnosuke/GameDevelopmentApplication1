#pragma once

#include "EnemyBase.h"

class Inky : public EnemyBase
{
private:

public:
	//初期化処理
	virtual void Initialize() override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void TrackingMovement() override;
};

