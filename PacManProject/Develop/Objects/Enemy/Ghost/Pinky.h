#pragma once

#include "../EnemyBase.h"

class Pinky : public EnemyBase
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

