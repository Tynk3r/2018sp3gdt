#include "CameraEffect.h"




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

bool CameraEffect::IsDone()
{
	return this->animFrame > this->fadeDuration;
}

void CameraEffect::SetFade(bool fading)
{
	this->canFade = fading;
}