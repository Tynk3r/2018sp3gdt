#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../PlayerInfo/PlayerInfo.h"

class CEnemy : public CEntity
{
public:
	enum FSM {
		F_ATTACK,
		F_IDLE,
		F_ROAM,

		F_RETREAT,
		F_TOTAL
	};
	enum ENEMY_AI {
		AI_SHORTRANGE,
		AI_LONGRANGE,
		AI_FLYING,

		AI_TOTAL
	};

	CEnemy(ENEMY_AI ai =  AI_SHORTRANGE);
	~CEnemy();

	virtual void Init();
	virtual void Update(double dt);

	FSM getState(void) { return state; }
	void setState(FSM s) { state = s; }
	void tempMoveBack(float dt);
	void setPlayerRef(CPlayerInfo* playerRef);
	void setAI(ENEMY_AI ai);
protected:
	FSM state;
	CPlayerInfo* playerRef;
	ENEMY_AI enemyAI;
private:

};

