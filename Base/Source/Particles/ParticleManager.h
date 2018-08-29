#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "Particle_2.h"
#include "../Entities/Projectile.h"
/*
Contributions :
	Coen/175448P -> {
		Created ParticleManager Class;
		pretty much did everything in ParticleManager Class;
		Rendered the particles in <particleList> in the scenes (RenderWorld() functions in the scenes);
	}

Updated as of 1:23 PM @ 29/8
*/

class ParticleManager : public Singleton<ParticleManager>
{
	friend Singleton<ParticleManager>;
public:
	void Update(double _dt);

	void AddParticle(CParticle_2* _newParticle);
	void AddParticle(CEntity* entity);//automatically create a CParticle_2 based on the entity
	void AddParticle(std::string text, CEntity* entity, Color color = Color());
	bool RemoveParticle(CParticle_2* _existingEntity);
/*
	std::list<CEntity*> getList() { return entityList; }*/
	std::list<CParticle_2*> particleList;
private:
	ParticleManager();
	virtual ~ParticleManager();
};

#endif // PARTICLE_MANAGER_H