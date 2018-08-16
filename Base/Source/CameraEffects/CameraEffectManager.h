#ifndef CAMERA_EFFECT_MANAGER_H
#define CAMERA_EFFECT_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "CameraEffect.h"

class CameraEffectManager : public Singleton<CameraEffectManager>
{
	friend Singleton<CameraEffectManager>;
public:
	void Update(double _dt);

	void AddCamEffect(CameraEffect* newCamEff);
	void AddCamEffect(CameraEffect::CAMEFFECT_TYPE camEffType);//automatically create a CParticle_2 based on the entity
	bool RemoveCamEff(CameraEffect* existingCamEff);
/*
	std::list<CEntity*> getList() { return entityList; }*/
	std::list<CameraEffect*> camEfflist;
private:
	CameraEffectManager();
	virtual ~CameraEffectManager();
};

#endif // CAMERA_EFFECT_MANAGER_H