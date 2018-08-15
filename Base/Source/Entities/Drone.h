#pragma once
#include "Vector3.h"
#include "Entity.h"

class CDrone : public CEntity //uses light[1] spotlight
{
public:
	enum FSM {
		F_ATTACK, //DRONE - HOMING MODE
		F_IDLE, //DRONE - START MODE
		F_ROAM, //DRONE - SCAN MODE
		F_TOTAL
	};

	CDrone();
	~CDrone();

	void Init();
	void Update(double dt);

	FSM getState(void) { return state; }
	void setState(FSM s) { state = s; }

	float getWingRotation() const;

private:
	FSM state;
	float wingRotation, eyeRotation;
	double rt_scanTime; //rt = realtime check
};

