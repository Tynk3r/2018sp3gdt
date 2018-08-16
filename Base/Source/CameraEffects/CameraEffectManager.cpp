#include "CameraEffectManager.h"

#include <iostream>
using namespace std;

// Update all particles
void CameraEffectManager::Update(double _dt)
{
	// Update all particles
	std::list<CameraEffect*>::iterator it, end;
	end = camEfflist.end();
	for (it = camEfflist.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Clean up particles that are done
	it = camEfflist.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = camEfflist.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Add a particle to this CameraEffectManager
void CameraEffectManager::AddCamEffect(CameraEffect* newCamEff)
{
	camEfflist.push_back(newCamEff);
}

void CameraEffectManager::AddCamEffect(CameraEffect::CAMEFFECT_TYPE camEffType)
{
	//CameraEffect* newPar = NULL;
	//switch (entity->getType())
	//{
	//	case CEntity::E_PROJECTILE:
	//	{
	//		CProjectile* proj = static_cast<CProjectile*>(entity);
	//		switch (proj->getProjType())
	//		{
	//			case CProjectile::PTYPE_FIRE:
	//				newPar = new CameraEffect(CameraEffect::PTYPE_FIRE, proj);
	//				break;
	//			case CProjectile::PTYPE_ICE:
	//				newPar = new CameraEffect(CameraEffect::PTYPE_ICE, proj);
	//				break;
	//		}
	//		break;
	//	}
	//}
	//if (newPar != NULL)
	//	this->AddParticle(newPar);
	CameraEffect* newCamEff = NULL;
	std::list<CameraEffect*>::iterator it, end;
	end = camEfflist.end();
	for (it = camEfflist.begin(); it != end; ++it)
	{
		CameraEffect* currCamEff = *it;
		if (currCamEff->getCamEffectType() == camEffType)
		{
			currCamEff->Init();
			return;
		}
	}
	newCamEff = new CameraEffect(camEffType);
	if (newCamEff != NULL)
		this->AddCamEffect(newCamEff);
}

// Remove a particle from this CameraEffectManager
bool CameraEffectManager::RemoveCamEff(CameraEffect* _existingParticle)
{
	// Find the entity's iterator
	std::list<CameraEffect*>::iterator findIter = std::find(camEfflist.begin(), camEfflist.end(), _existingParticle);

	// Delete the entity if found
	if (findIter != camEfflist.end())
	{
		delete *findIter;
		findIter = camEfflist.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

// Constructor
CameraEffectManager::CameraEffectManager()
{
}

// Destructor
CameraEffectManager::~CameraEffectManager()
{
	std::list<CameraEffect*>::iterator it, end;
	it = camEfflist.begin();
	end = camEfflist.end();
	while (it != end)
	{
		delete *it;
		it = camEfflist.erase(it);
	}
}
