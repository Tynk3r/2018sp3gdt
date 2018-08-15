#include "Particle_2.h"

CParticle_2::CParticle_2(PARTICLE_TYPE particleType, Vector3 pos, Vector3 vel, Vector3 scale) :
	parent(NULL),
	animFrame(0),
	particleType(particleType),
	transparency(0),
	vel(vel),
	startScale(scale),
	rot(0)
{
	this->setType(E_PARTICLE);
	this->setPos(pos);
	this->setScale(scale);
	this->Init();
}

CParticle_2::CParticle_2(PARTICLE_TYPE particleType, CEntity * parent) :
	parent(parent),
	animFrame(0),
	particleType(particleType),
	transparency(0),
	startScale(parent->getScale()),
	rot(0)
{
	this->setType(E_PARTICLE);
	this->setPos(parent->getPos());
	this->Init();
}

CParticle_2::~CParticle_2()
{
}

void CParticle_2::Init()
{
	switch (this->particleType)
	{
	case PTYPE_FIRE:
		if (this->parent != NULL)
		{
			Vector3 tarViewVec = (this->parent->getTarget() - this->parent->getPos()).Normalized();
			this->vel = tarViewVec * Math::RandFloatMinMax(18, 150) + Vector3(Math::RandFloatMinMax(-40, 40), Math::RandFloatMinMax(-25, 25), Math::RandFloatMinMax(-40, 40));
			this->setScale(this->parent->getScale()*Math::RandFloatMinMax(1.f, 1.3f));
			this->startScale = this->getScale();
		}
		this->acc = Vector3(0, Math::RandFloatMinMax(150,250), 0);
		this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->lifeSpan = Math::RandFloatMinMax(0.25f, 0.45f);
		this->setSpeed(Math::RandFloatMinMax(-120.f, 120.f));//setting speed for ROTATION btw
		break;
	case PTYPE_ICE:
		if (this->parent != NULL)
		{
			Vector3 tarViewVec = (this->parent->getTarget() - this->parent->getPos()).Normalized();
			this->vel = tarViewVec * Math::RandFloatMinMax(15, 40) + Vector3(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(10, 35), Math::RandFloatMinMax(-20, 20));
			this->setScale(this->parent->getScale()*Math::RandFloatMinMax(0.9f, 1.15f));
			this->startScale = this->getScale();
		}
		this->acc = Vector3(0, -Math::RandFloatMinMax(150, 400), 0);
		this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->lifeSpan = Math::RandFloatMinMax(0.4f, 0.6f);
		this->setSpeed(Math::RandFloatMinMax(-45, 45));//setting speed for ROTATION btw
		break;
	}
}

void CParticle_2::Update(double dt)
{
	this->animFrame += (float)dt;
	float alphaVal = this->animFrame / this->lifeSpan;
	this->setScale(this->startScale.lerped(this->endScale, alphaVal));
	this->vel += this->acc*dt;
	this->setPos(this->getPos() + this->vel * dt);
	this->rot += this->getSpeed()*dt;
	switch (this->particleType)
	{
	case PTYPE_FIRE:
		this->transparency = Math::lerp(0.2f, 1.f, alphaVal);
		break;
	case PTYPE_ICE:
		this->transparency = Math::lerp(0.1f, 1.f, alphaVal);
		break;
	}
}

bool CParticle_2::IsDone()
{
	return this->animFrame > this->lifeSpan;
}

CParticle_2::PARTICLE_TYPE CParticle_2::getParType()
{
	return this->particleType;
}

float CParticle_2::getRot()
{
	return this->rot;
}

float CParticle_2::getTransparency()
{
	return this->transparency;
}
