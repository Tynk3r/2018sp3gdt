#include "Projectile.h"




CProjectile::CProjectile(PROJECTILE_TYPE projectileType) :
	projectileType(projectileType),
	elapsedTime(0)
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
		this->setSpeed(1000);
		this->setScale(Vector3(10, 10, 10));
		break;
	case PTYPE_ICE:
		this->setSpeed(750);
		this->setScale(Vector3(2, 2, 2));
		break;
	case PTYPE_BEAM:
		this->setSpeed(0);
		break;
	}
	this->setPos(pos);
	this->setTarget(targ);
	this->originPosition = pos;
	lifespanTime = 10;
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
}

void CProjectile::Update(double dt)
{
	CEntity::Update(dt);
	this->elapsedTime += (float)dt;
	switch (projectileType)
	{
	case PTYPE_BEAM:
		if (!isDone())
		{
			ParticleManager::GetInstance()->AddParticle(this);
			ParticleManager::GetInstance()->AddParticle(this);
			ParticleManager::GetInstance()->AddParticle(this);
			ParticleManager::GetInstance()->AddParticle(this);
			ParticleManager::GetInstance()->AddParticle(this);
		}
		break;
	default:
		setPos(getPos() + (viewVector * getSpeed() * (float)dt));
		ParticleManager::GetInstance()->AddParticle(this);
		break;
	}
	lifespanTime -= dt;
	if (lifespanTime <= 0) setIsDone(true);
}

//bool CProjectile::IsDone()
//{
//	return this->done;
//}
//
//void CProjectile::setDone(bool done)
//{
//	this->done = done;
//}

CProjectile::PROJECTILE_TYPE CProjectile::getProjType()
{
	return this->projectileType;
}

float CProjectile::getElapsedTime()
{
	return this->elapsedTime;
}
