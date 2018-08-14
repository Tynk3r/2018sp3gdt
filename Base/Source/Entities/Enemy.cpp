#include "Enemy.h"

CEnemy::CEnemy() : 
	CEntity(),
	state(F_IDLE)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init()
{
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
}

void CEnemy::Update(double dt)
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	setPos(getPos() + (viewVector * getSpeed() * (float)dt));
}
