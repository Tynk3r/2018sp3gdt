#include "CameraEffect.h"
#include "EasingStyles\QuadEase.h"
#include "EasingStyles\BackEase.h"
#include "../TimeTrackerManager.h"


CameraEffect::CameraEffect(CAMEFFECT_TYPE camEffectType) :
	camEffectType(camEffectType),
	transparency(0),
	canFade(false),
	fadeDuration(1),
	animFrame(0)
{
	this->Init();
}

CameraEffect::~CameraEffect()
{
}

void CameraEffect::Init()
{
	//switch (this->projectileType) //set properties depending on projectile's type (fire or ice or whatever)
	//{
	//case PTYPE_FIRE:
	//	this->setSpeed(350);
	//	this->setScale(Vector3(2, 2, 2));
	//	break;
	//case PTYPE_ICE:
	//	this->setSpeed(225);
	//	this->setScale(Vector3(2, 2, 2));
	//	break;
	//}
	//this->setPos(pos);
	//this->setTarget(targ);	
	//// Add to EntityManager
	//EntityManager::GetInstance()->AddEntity(this);
	//setCollider(true);
	switch (this->camEffectType)
	{
	case CE_TYPE_ACTIONLINE_WHITE:
		this->fadeDuration = 0.65f;
		this->transparency = 0;
		this->canFade = true;
		this->animFrame = 0;
		break;
	case CE_TYPE_TIME_SLOW:
		this->fadeDuration = 2.f;
		this->transparency = 1.f;
		this->canFade = true;
		this->animFrame = 0;
		break;
	case CE_TYPE_CONFETTI:
		this->fadeDuration = 8.f;
		this->transparency = 0.f;
		this->canFade = true;
		this->animFrame = 0;
	}
}

void CameraEffect::Update(double dt)
{
	//CEntity::Update(dt);
	//switch (projectileType)
	//{
	//case PTYPE_BEAM:
	//	ParticleManager::GetInstance()->AddParticle(this);
	//	break;
	//default:
	//	setPos(getPos() + (viewVector * getSpeed() * (float)dt));
	//	ParticleManager::GetInstance()->AddParticle(this);
	//	break;
	//}
	switch (this->camEffectType)
	{
	case CE_TYPE_ACTIONLINE_WHITE:
		if (this->canFade)
		{
			this->animFrame += (float)dt;
			float alpha = this->animFrame / this->fadeDuration;
			this->transparency = Math::lerp(0.f, 1.f, alpha);
			
		}
		break;
	case CE_TYPE_CONFETTI:
		if (this->canFade)
		{
			this->animFrame += (float)dt;
			if (this->animFrame / this->fadeDuration < 0.05f)
			{
				float alpha = (this->animFrame / this->fadeDuration) / 0.05f;
				this->offset = Vector3(0, 0, 0).lerped(Vector3(10, 10, 10), alpha);
			}
			else if (this->animFrame / this->fadeDuration < 0.9f)
			{
				float alpha = ((this->animFrame / this->fadeDuration) - 0.05f) / 0.85f;
				this->offset = Vector3(10, 10, 10).lerped(Vector3(9, 9, 9), alpha);
			}
			else
			{
				float alpha = ((this->animFrame / this->fadeDuration) - 0.9f) / 0.1f;
				this->transparency = Math::lerp(0.f, 1.f, alpha);
			}
		}
		break;
	case CE_TYPE_TIME_SLOW:
		if (this->canFade)
		{
			this->animFrame += ((float)dt) / TimeTrackerManager::GetInstance()->getSpeed();
			if (this->animFrame/this->fadeDuration < 0.25f)
			{
				float alpha = (this->animFrame/this->fadeDuration) / 0.25f;
				this->transparency = Math::lerp(1.f, 0.1f, Back::easeOut(alpha, 0, 1, 1));
			}
			else if (this->animFrame / this->fadeDuration < 0.75f)
			{
				float alpha = ((this->animFrame / this->fadeDuration) - 0.25f) / 0.5f;
				this->transparency = Math::lerp(0.1f, 0.3f, alpha);
			}
			else
			{
				float alpha = ((this->animFrame / this->fadeDuration) - 0.75f) / 0.25f;
				this->transparency = Math::lerp(0.3f, 1.f, Quad::easeIn(alpha, 0, 1, 1));
			}
		}
		break;
	}
}

CameraEffect::CAMEFFECT_TYPE CameraEffect::getCamEffectType() const
{
	return this->camEffectType;
}

Vector3 CameraEffect::getOffset() const
{
	return this->offset;
}

float CameraEffect::getTransparency() const
{
	return this->transparency;
}

float CameraEffect::getAnimFrame() const
{
	return this->animFrame;
}

float CameraEffect::getDuration() const
{
	return this->fadeDuration;
}

bool CameraEffect::IsDone()
{
	return this->animFrame > this->fadeDuration;
}

void CameraEffect::SetFade(bool fading)
{
	this->canFade = fading;
}
