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
	setCollider(true);
	setType(E_ENEMY);
}

void CEnemy::Update(double dt)
{
	CEntity::Update(dt);
}

void CEnemy::tempMoveBack(float dt)
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	this->setPos(this->getPos() - viewVector*this->getSpeed()*dt);
}
