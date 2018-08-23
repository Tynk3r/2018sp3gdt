#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Rigs/RigInfo.h"
#include "../Entities/Projectile.h"

class CBoss : public CEntity
{
public:
	enum FSM {
		F_NORMAL,
		F_ATTACK_FIREBALL,
		F_ATTACK_ICEBALL,
	};

	CBoss(Vector3 pos = Vector3(0, 150, 500), Vector3 scale = Vector3(20, 60, 20), Vector3 target = Vector3(30, 0, 79));
	~CBoss();

	void Init();
	void Update(double dt);

	FSM getState(void) { return state; }
	void setState(FSM s) { state = s; }
	void tempMoveBack(float dt);
	void setPlayerRef(CPlayerInfo* playerRef);
	Vector3 getOrigScale() const;
	float getElapsedTime() const;
	CRigInfo rig;
private:
	FSM state;
	CPlayerInfo* playerRef;
	Vector3 originalScale;
	float elapsedTime;
	float animFrame;
	CProjectile* holdingProjectile;
};

