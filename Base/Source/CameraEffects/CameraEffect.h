#pragma once
#include "Vector3.h"
//camera effect class, able to store stuff like duration of cam effect and transparency n etc
class CameraEffect
{
public:
	enum CAMEFFECT_TYPE {
		CE_TYPE_ACTIONLINE_WHITE,

		CE_TYPE_TOTAL
	};

	CameraEffect(CAMEFFECT_TYPE camEffectType = CE_TYPE_ACTIONLINE_WHITE);
	~CameraEffect();

	void Init();
	void Update(double dt);
	CAMEFFECT_TYPE getCamEffectType() const;
	Vector3 getOffset() const;
	float getTransparency() const;
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

