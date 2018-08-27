#include "Witch.h"

CWitch::CWitch() : 
	CEnemy(AI_LONGRANGE),
	witchCooldown(0)
{
}


CWitch::~CWitch()
{
}

void CWitch::Update(double dt)
{
	CEnemy::Update(dt);
	switch (this->state)
	{
	case F_RETREAT:
		break;
	}
}