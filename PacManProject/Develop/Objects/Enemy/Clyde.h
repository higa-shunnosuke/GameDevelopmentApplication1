#pragma once

#include "EnemyBase.h"

class Clyde : public EnemyBase
{
private:

public:
	//‰Šú‰»ˆ—
	virtual void Initialize() override;

private:
	/// <summary>
	/// ˆÚ“®ˆ—
	/// </summary>
	virtual void TrackingMovement() override;
};

