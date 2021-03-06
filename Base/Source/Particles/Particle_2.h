#ifndef PARTICLE_2_H
#define PARTICLE_2_H
#include "Vector3.h"
#include "../Entities/Entity.h"
#include "../Vertex.h"
#include <string>

/*
Contributions :
	Coen/175448P -> {
		Created Particle_2 Class;
		Coded the basis of the class;
		Created the following Particle types : [ FIRE, ICE, TEXT, ENEMYAPPEAR ];
	}

Updated as of 1:23 PM @ 29/8
*/

// a projectile class
class CParticle_2 : public CEntity
{
public:
	enum PARTICLE_TYPE {
		PTYPE_FIRE,
		PTYPE_ICE,
		PTYPE_BEAM,
		PTYPE_SPECIAL_KILLERNADO,
		PTYPE_TEXT,
		PTYPE_ENEMYAPPEAR,
		PTYPE_TOTAL
	};

	CParticle_2(PARTICLE_TYPE particleType, Vector3 pos, Vector3 vel = Vector3(0, 0, 0) , Vector3 scale = Vector3(5, 5, 5));
	CParticle_2(PARTICLE_TYPE particleType, CEntity* parent);
	CParticle_2(std::string text, CEntity* parent, Color color = Color());
	~CParticle_2();

	void Init();
	void Update(double dt);
	bool IsDone();
	PARTICLE_TYPE getParType();
	float getRot();
	float getTransparency();
	Vector3 getOriginPos();
	Color getColor() const;
	std::string text;
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
	Vector3 originPosition;
	Color startColor;
	Color endColor;
	Color color;
};

#endif