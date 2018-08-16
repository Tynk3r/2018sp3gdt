#include "Projectile.h"




CProjectile::CProjectile(PROJECTILE_TYPE projectileType) :
	projectileType(projectileType)
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
		this->setSpeed(350);
		this->setScale(Vector3(2, 2, 2));
		break;
	case PTYPE_ICE:
		this->setSpeed(225);
		this->setScale(Vector3(2, 2, 2));
		break;
	}
	this->setPos(pos);
	this->setTarget(targ);	
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
}

void CProjectile::Update(double dt)
{
	CEntity::Update(dt);
	ParticleManager::GetInstance()->AddParticle(this);
}

CProjectile::PROJECTILE_TYPE CProjectile::getProjType()
{
	return this->projectileType;
}
