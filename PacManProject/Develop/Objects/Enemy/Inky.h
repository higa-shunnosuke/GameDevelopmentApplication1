#pragma once

#include "EnemyBase.h"

class Inky : public EnemyBase
{
private:

public:
	//‰Šú‰»ˆ—
	virtual void Initialize() override;

private:
	/// <summary>
	/// ˆÚ“®ˆ—
	/// </summary>
	/// <param name="delta_second">1ƒtƒŒ[ƒ€‚ ‚½‚è‚ÌŠÔ</param>
	virtual void TrackingMovement(float delta_second) override;
};

