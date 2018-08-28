#pragma once
#include "Vector3.h"
#include "Enemy.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Projectile.h"
class CWitchFog : public CEnemy
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
	CWitchFog();
	~CWitchFog();

	void Update(double dt);
protected:
	float witchCooldown;
	WITCH_STATE witchState;
private:
	CProjectile* witchProjectile;
};

