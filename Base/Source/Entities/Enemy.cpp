#include "Enemy.h"

CEnemy::CEnemy() : 
	CEntity(),
	target(Vector3(10.f, 10.f, 0.f)),
	speed(10.f),
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
	Vector3 viewVector = (target - getPos()).Normalized();
	setPos(getPos() + (viewVector * speed * (float)dt));
}
