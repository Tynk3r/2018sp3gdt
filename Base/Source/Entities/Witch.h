#pragma once
#include "Vector3.h"
#include "Enemy.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Projectile.h"

/*
Contributions :
	Coen/175448P -> {
		Created Witch Class;
		everything lul;
	}

Updated as of 1:29 PM @ 29/8
*/

class CWitch : public CEnemy
{
public:
	enum WITCH_STATE
	{
		WS_IDLE,
		WS_CHARGING,
		WS_SURPRISED,
		WS_FIRED,

		WS_TOTAL
	};
	CWitch();
	~CWitch();

	void Update(double dt);
protected:
	float witchCooldown;
	WITCH_STATE witchState;
private:
	CProjectile* witchProjectile;
};

