#include "ParticleManager.h"

#include <iostream>
using namespace std;

// Update all particles
void ParticleManager::Update(double _dt)
{
	// Update all particles
	std::list<CParticle_2*>::iterator it, end;
	end = particleList.end();
	for (it = particleList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Clean up particles that are done
	it = particleList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = particleList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Add a particle to this ParticleManager
void ParticleManager::AddParticle(CParticle_2* _newParticle)
{
	particleList.push_back(_newParticle);
}

void ParticleManager::AddParticle(CEntity * entity)
{
	CParticle_2* newPar = NULL;
	switch (entity->getType())
	{
		case CEntity::E_PROJECTILE:
		{
			CProjectile* proj = static_cast<CProjectile*>(entity);
			switch (proj->getProjType())
			{
				case CProjectile::PTYPE_FIRE:
					newPar = new CParticle_2(CParticle_2::PTYPE_FIRE, proj);
					break;
				case CProjectile::PTYPE_ICE:
					newPar = new CParticle_2(CParticle_2::PTYPE_ICE, proj);
					break;
			}
			break;
		}
	}
	if (newPar != NULL)
		this->AddParticle(newPar);
}

// Remove a particle from this ParticleManager
bool ParticleManager::RemoveParticle(CParticle_2* _existingParticle)
{
	// Find the entity's iterator
	std::list<CParticle_2*>::iterator findIter = std::find(particleList.begin(), particleList.end(), _existingParticle);

	// Delete the entity if found
	if (findIter != particleList.end())
	{
		delete *findIter;
		findIter = particleList.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

// Constructor
ParticleManager::ParticleManager()
{
}

// Destructor
ParticleManager::~ParticleManager()
{
}
