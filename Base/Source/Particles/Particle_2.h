#pragma once
#include "Vector3.h"
#include "../Entities/Entity.h"

// a projectile class
class CParticle_2 : public CEntity
{
public:
	enum PARTICLE_TYPE {
		PTYPE_FIRE,
		PTYPE_ICE,
		PTYPE_BEAM,
		PTYPE_TOTAL
	};

	CParticle_2(PARTICLE_TYPE particleType, Vector3 pos, Vector3 vel = Vector3(0, 0, 0) , Vector3 scale = Vector3(5, 5, 5));
	CParticle_2(PARTICLE_TYPE particleType, CEntity* parent);
	~CParticle_2();

	void Init();
	void Update(double dt);
	bool IsDone();
	PARTICLE_TYPE getParType();
	float getRot();
	float getTransparency();
private:
	PARTICLE_TYPE particleType;
	Vector3 viewVector;
	Vector3 vel;
	Vector3 acc;
	Vector3 startScale;
	Vector3 endScale;
	float rot;
	float animFrame;
	float lifeSpan;
	float transparency;
	CEntity* parent;
};

