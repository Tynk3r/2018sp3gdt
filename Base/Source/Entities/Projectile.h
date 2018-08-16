#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../Particles/ParticleManager.h"
// a projectile class
class CProjectile : public CEntity
{
public:
	enum PROJECTILE_TYPE {
		PTYPE_FIRE,
		PTYPE_ICE,
		PTYPE_BEAM,
		PTYPE_TOTAL
	};

	CProjectile(PROJECTILE_TYPE projectileType = PTYPE_FIRE);
	~CProjectile();

	void Init(Vector3 pos, Vector3 targ);
	void Update(double dt);
	PROJECTILE_TYPE getProjType();
	float getElapsedTime();
private:
	PROJECTILE_TYPE projectileType;
	Vector3 viewVector;
	float elapsedTime;
};

