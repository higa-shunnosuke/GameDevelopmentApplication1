#pragma once

#include "EnemyBase.h"

class Blinky : public EnemyBase
{
private:

public:
	//����������
	virtual void Initialize() override;

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual void TrackingMovement() override;
};

