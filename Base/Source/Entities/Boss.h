#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../PlayerInfo/PlayerInfo.h"

class CBoss : public CEntity
{
public:
	enum FSM {
		F_ATTACK,
		F_IDLE,
		F_ROAM,
		F_TOTAL
	};

	CBoss(Vector3 pos = Vector3(0, 150, 500), Vector3 scale = Vector3(20, 60, 20), Vector3 target = Vector3(0, 150, 79));
	~CBoss();

	void Init();
	void Update(double dt);

	FSM getState(void) { return state; }
	void setState(FSM s) { state = s; }
	void tempMoveBack(float dt);
	void setPlayerRef(CPlayerInfo* playerRef);
private:
	FSM state;
	CPlayerInfo* playerRef;
};

