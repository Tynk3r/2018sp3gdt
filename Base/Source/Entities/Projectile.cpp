#include "Projectile.h"




CProjectile::CProjectile(PROJECTILE_TYPE projectileType) :
	projectileType(projectileType), done(false)
{
	this->setType(CEntity::E_PROJECTILE);
}

CProjectile::~CProjectile()
{
}

void CProjectile::Init(Vector3 pos, Vector3 targ)
{
	switch (this->projectileType) //set properties depending on projectile's type (fire or ice or whatever)
	{
	case PTYPE_FIRE:
		this->setSpeed(50);
		this->setScale(Vector3(2, 2, 2));
		break;
	case PTYPE_ICE:
		this->setSpeed(25);
		this->setScale(Vector3(2, 2, 2));
		break;
	}
	this->setPos(pos);
	this->setTarget(targ);	
	viewVector = (getTarget() - getPos()).Normalized();
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
}

void CProjectile::Update(double dt)
{
	

	setPos(getPos() + (viewVector * getSpeed() * (float)dt));
	
	
}

bool CProjectile::IsDone()
{
	return this->done;
}

void CProjectile::setDone(bool done)
{
	this->done = done;
}
