#pragma once

#include "Mtx44.h"

enum ParticleObject_TYPE
{
	P_WATER = 0,
	P_SMOKE,
	P_SPARK,
	P_TOTAL,
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE = P_WATER);
	~ParticleObject(void);

	ParticleObject_TYPE type;  // Type of Particle 

	Vector3 pos;	// Position of Particle
	Vector3 vel;	// Velocity of Particle
	Vector3 scale;	// Scale of Particle
	float rotation;		//Rotation of particle
	float rotationSpeed;	//Rotational speed of particle

	bool active;
};