#pragma once
#include "Vector3.h"

/*
Contributions :
	Coen/175448P -> {
		Created CameraEffect Class;
		Coded the basis of the class;
		Created the following CamEffect Types = [ ACTIONLINE_WHITE, TIME_SLOW ];
	}

Updated as of 1:35 PM @ 29/8
*/

//camera effect class, able to store stuff like duration of cam effect and transparency n etc
class CameraEffect
{
public:
	enum CAMEFFECT_TYPE {
		CE_TYPE_ACTIONLINE_WHITE,
		CE_TYPE_TIME_SLOW,
		CE_TYPE_TOTAL
	};

	CameraEffect(CAMEFFECT_TYPE camEffectType = CE_TYPE_ACTIONLINE_WHITE);
	~CameraEffect();

	void Init();
	void Update(double dt);
	CAMEFFECT_TYPE getCamEffectType() const;
	Vector3 getOffset() const;
	float getTransparency() const;
	float getAnimFrame() const;
	float getDuration() const;
	bool IsDone();
	void SetFade(bool fading);
private:
	CAMEFFECT_TYPE camEffectType;
	Vector3 offset;
	bool canFade;
	float fadeDuration;
	float animFrame;
	float transparency;
};

