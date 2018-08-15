#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "Particle_2.h"
#include "../Entities/Projectile.h"


class ParticleManager : public Singleton<ParticleManager>
{
	friend Singleton<ParticleManager>;
public:
	void Update(double _dt);

	void AddParticle(CParticle_2* _newParticle);
	void AddParticle(CEntity* entity);//automatically create a CParticle_2 based on the entity
	bool RemoveParticle(CParticle_2* _existingEntity);
/*
	std::list<CEntity*> getList() { return entityList; }*/
	std::list<CParticle_2*> particleList;
private:
	ParticleManager();
	virtual ~ParticleManager();
};

#endif // PARTICLE_MANAGER_H