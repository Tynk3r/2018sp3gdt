#include "Projectile.h"
#include "Mtx44.h"



CProjectile::CProjectile(PROJECTILE_TYPE projectileType, SPELLMOD_TYPE spellModType) :
	projectileType(projectileType),
	spellModType(spellModType),
	elapsedTime(0),
	particleRate(1.f / 60.f),
	projRot(0),
	source(NULL),
	burstPivotRotOffset(0),
	burstPivotRot(Vector3(0, 0, 0)),
	originDir(Vector3(0, 0, 0))
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
	{
		if (spellModType == SMTYPE_NORMAL)
		{
			this->setSpeed(1000);
			this->setScale(Vector3(10, 10, 10));
			this->particleRate = 1.f / 60.f;
			lifespanTime = 10;
		}
		else if (spellModType == SMTYPE_BURST)
		{
			this->setSpeed(500);
			this->setScale(Vector3(8, 8, 8));
			this->particleRate = 1.f / 30.f;
			lifespanTime = 4;
			setGrav(Vector3(0, 0, 0));
			originDir = (targ - pos).Normalized();
		}
		else if (spellModType == SMTYPE_SPECIAL)
		{
			this->setSpeed(600);
			this->setScale(Vector3(15, 15, 15));
			this->particleRate = 1.f / 60.f;
			lifespanTime = 10;
		}
		break;
	}
	case PTYPE_ICE:
	{
		if (spellModType == SMTYPE_NORMAL)
		{
			this->setSpeed(750);
			this->setScale(Vector3(2, 2, 2));
			this->particleRate = 1.f / 60.f;
			lifespanTime = 10;
		}
		else if (spellModType == SMTYPE_BURST)
		{
			this->setSpeed(600);
			this->setScale(Vector3(2, 2, 2));
			this->particleRate = 1.f / 30.f;
			lifespanTime = 5;
			setGrav(Vector3(0, 0, 0));
			originDir = (targ - pos).Normalized();
		}
		else if (spellModType == SMTYPE_SPECIAL)
		{
			this->setSpeed(600);
			this->setScale(Vector3(4, 4, 4));
			this->particleRate = 1.f / 60.f;
			lifespanTime = 10;
		}
		break;
	}
	case PTYPE_BEAM:
	{
		this->setSpeed(0);
		this->particleRate = 1.f / 60.f;
		setGrav(Vector3(0, 0, 0));
		lifespanTime = 0.3;
		break;
	}
	case PTYPE_SPECIAL_KILLERNADO:
	{
		this->setSpeed(500);
		this->particleRate = 1.f / 60.f;
		this->setScale(Vector3(60, 60, 60));
		setGrav(Vector3(0, 0, 0));
		lifespanTime = 5;
		break;
	}

	default:
		break;
	}
	this->setPos(pos);
	this->setTarget(targ);
	this->originPosition = pos;
	this->source = source;
	
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);

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
	
	if (projectileType == PTYPE_FIRE && spellModType == SMTYPE_BURST)
	{
		setTarget(getPos() - burstPivotRot);

		burstPivotRotOffset += 90 * dt;
		if (burstPivotRotOffset >= 360) burstPivotRotOffset = fmodf(burstPivotRotOffset, 360);
		Vector3 tempRight = Vector3(1, 0, 0);
		Mtx44 rotation;
		rotation.SetToRotation(burstPivotRotOffset, 0, 1, 0);
		burstPivotRot = rotation * tempRight;
		burstPivotRot *= 4;

		setTarget(getTarget() + originDir + burstPivotRot);
	}
	else if (projectileType == PTYPE_ICE && spellModType == SMTYPE_BURST)
	{
		setTarget(getTarget() - burstPivotRot);

			burstPivotRotOffset += 2 * 360 * dt;
			if (burstPivotRotOffset >= 360) burstPivotRotOffset = fmodf(burstPivotRotOffset, 360);
			Vector3 tempRight;
			if (!(originDir - Vector3(0, 1, 0)).IsZero()) tempRight = originDir.Cross(Vector3(0, 1, 0));
			else tempRight = Vector3(1, 0, 0);
			Mtx44 rotation;
			rotation.SetToRotation(burstPivotRotOffset, originDir.x, originDir.y, originDir.z);
			burstPivotRot = rotation * tempRight;
			burstPivotRot *= 0.5;

			setTarget(getTarget() + burstPivotRot);
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
