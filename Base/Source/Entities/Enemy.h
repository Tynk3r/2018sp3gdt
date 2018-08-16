#pragma once
#include "Vector3.h"
#include "Entity.h"

class CEnemy : public CEntity
{
public:
	enum FSM {
		F_ATTACK,
		F_IDLE,
		F_ROAM,
		F_TOTAL
	};

	CEnemy();
	~CEnemy();

	void Init();
	void Update(double dt);

	FSM getState(void) { return state; }
	void setState(FSM s) { state = s; }
	void tempMoveBack(float dt);
private:
	FSM state;
};

