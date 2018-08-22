#include "Projectile.h"




CProjectile::CProjectile(PROJECTILE_TYPE projectileType) :
	projectileType(projectileType),
	elapsedTime(0),
	particleRate(1.f / 60.f),
	projRot(0),
	source(NULL)
{
	this->setType(CEntity::E_PROJECTILE);
}

CProjectile::~CProjectile()
{
}

void CProjectile::Init(Vector3 pos, Vector3 targ, CEntity* source)
{
	switch (this->projectileType) //set properties depending on projectile's type (fire or ice or whatever)
	{
	case PTYPE_FIRE:
		this->setSpeed(1000);
		this->setScale(Vector3(10, 10, 10));
		this->particleRate = 1.f / 60.f;
		break;
	case PTYPE_ICE:
		this->setSpeed(750);
		this->setScale(Vector3(2, 2, 2));
		this->particleRate = 1.f / 60.f;
		break;
	case PTYPE_BEAM:
		this->setSpeed(0);
		this->particleRate = 1.f / 60.f;
		break;
	}
	this->setPos(pos);
	this->setTarget(targ);
	this->originPosition = pos;
	this->source = source;
	lifespanTime = 10;
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);

	if (projectileType == PTYPE_BEAM) setGrav(Vector3(0, 0, 0));
}

void CProjectile::Update(double dt)
{
	CEntity::Update(dt);
	this->elapsedTime += (float)dt;
	this->projRot += (float)dt*2;
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
		if (this->elapsedTime > this->particleRate)
		{
			ParticleManager::GetInstance()->AddParticle(this);
			this->elapsedTime = 0;
		}
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

float CProjectile::getProjRot()
{
	return this->projRot;
}

void CProjectile::EmitParticles(int amt)
{
	for (int i = 0; i < amt; ++i)
		ParticleManager::GetInstance()->AddParticle(this);
}

CEntity * CProjectile::getSource()
{
	return this->source;
}
