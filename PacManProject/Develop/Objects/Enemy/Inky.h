#pragma once

#include "EnemyBase.h"

class Inky : public EnemyBase
{
private:

public:
	//����������
	virtual void Initialize() override;

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="delta_second">1�t���[��������̎���</param>
	virtual void TrackingMovement(float delta_second) override;
};

