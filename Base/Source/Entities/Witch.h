#pragma once
#include "Vector3.h"
#include "Enemy.h"
#include "../PlayerInfo/PlayerInfo.h"

class CWitch : public CEnemy
{
public:
	CWitch();
	~CWitch();

	void Update(double dt);
protected:
	float witchCooldown;
private:

};

