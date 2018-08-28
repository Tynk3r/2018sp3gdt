#include "Particle_2.h"
#include "EasingStyles\QuadEase.h"
#include "EasingStyles\BackEase.h"
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
	this->originPosition = pos;
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
	if (this->parent->getType() == CEntity::E_PROJECTILE)
		this->setPos(parent->getPos());
	else
		this->setPos(parent->getPos() +
			Vector3(Math::RandFloatMinMax(-parent->getScale().x, parent->getScale().x),
				Math::RandFloatMinMax(-parent->getScale().y, parent->getScale().y),
				Math::RandFloatMinMax(-parent->getScale().z, parent->getScale().z)));
	
	this->originPosition = parent->getOriginPos();
	this->Init();
}

CParticle_2::CParticle_2(std::string text, CEntity * parent, Color color) :
	parent(parent),
	text(text),
	animFrame(0),
	transparency(0),
	endScale(parent->getScale()),
	rot(0)
{
	this->setType(E_PARTICLE);
	this->particleType = PTYPE_TEXT;
	this->startColor = this->endColor = this->color = color;
	this->originPosition = parent->getOriginPos();
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
		this->acc = Vector3(0, Math::RandFloatMinMax(150, 250), 0);
		this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->lifeSpan = Math::RandFloatMinMax(0.25f, 0.55f);
		this->setSpeed(Math::RandFloatMinMax(-120.f, 120.f));//setting speed for ROTATION btw
		if (this->parent != NULL)
		{
			Vector3 tarViewVec = (this->parent->getTarget() - this->parent->getPos()).Normalized();
			if (this->parent->isDone())
			{
				float totalscale = 0.2f + (parent->getScale().x + parent->getScale().y + parent->getScale().z) *0.01f;
				this->vel = Vector3(Math::RandFloatMinMax(-800, 800), Math::RandFloatMinMax(-800, 800), Math::RandFloatMinMax(-800, 800))*totalscale;
				this->setScale(this->parent->getScale()*Math::RandFloatMinMax(1.5f, 3.3f));
				this->lifeSpan = Math::RandFloatMinMax(0.3f, 0.7f);
				this->acc = -this->vel*1.75f;
				this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.25f, 0.5f);
			}
			else
			{
				float totalscale = (parent->getScale().x + parent->getScale().y + parent->getScale().z) *0.035f;
				this->vel = tarViewVec * Math::RandFloatMinMax(18, 150) + Vector3(Math::RandFloatMinMax(-40, 40), Math::RandFloatMinMax(-25, 25), Math::RandFloatMinMax(-40, 40))*totalscale;
				this->setScale(this->parent->getScale()*Math::RandFloatMinMax(1.f, 1.3f));
			}
			this->startScale = this->getScale();
		}
		break;
	case PTYPE_ICE:
		this->acc = Vector3(0, -Math::RandFloatMinMax(150, 400), 0);
		this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->lifeSpan = Math::RandFloatMinMax(0.4f, 0.6f);
		this->setSpeed(Math::RandFloatMinMax(-45, 45));//setting speed for ROTATION btw
		if (this->parent != NULL)
		{
			Vector3 tarViewVec = (this->parent->getTarget() - this->parent->getPos()).Normalized();
			if (this->parent->isDone())
			{
				float totalscale = 0.25f + (parent->getScale().x + parent->getScale().y + parent->getScale().z) *0.02f;
				this->vel = Vector3(Math::RandFloatMinMax(-350, 350), Math::RandFloatMinMax(-350, 350), Math::RandFloatMinMax(-350, 350))*totalscale;
				this->setScale(this->parent->getScale()*Math::RandFloatMinMax(1.5f, 3.5f));
				this->lifeSpan = Math::RandFloatMinMax(0.5f, 1.f);
				this->acc = -this->vel*2.f;
				this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.5f);
			}
			else
			{
				float totalscale = 0.15f + (parent->getScale().x + parent->getScale().y + parent->getScale().z) *0.04f;
				this->vel = tarViewVec * Math::RandFloatMinMax(15, 40) + Vector3(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(10, 35), Math::RandFloatMinMax(-20, 20))*totalscale;
				this->setScale(this->parent->getScale()*Math::RandFloatMinMax(0.9f, 1.15f));
				this->acc = Vector3(0, -Math::RandFloatMinMax(150, 400), 0)*totalscale;
			}
			this->startScale = this->getScale();
		}
		break;
	case PTYPE_BEAM:
		if (this->parent != NULL)
		{
			Vector3 tarViewVec = (this->parent->getTarget() - this->parent->getPos()).Normalized();
			this->vel = tarViewVec * Math::RandFloatMinMax(15, 40) + Vector3(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(10, 35), Math::RandFloatMinMax(-20, 20));
			this->setScale(Vector3(1.f, 1.f, 1.f)*Math::RandFloatMinMax(2.0f, 6.f));
			this->startScale = this->getScale();
			//this->setPos((this->parent->getPos()) + tarViewVec * Math::RandFloatMinMax(this->parent->getScale().z * 0.1, this->parent->getScale().z));
			this->setPos( (this->parent->getPos()) + tarViewVec * this->parent->getScale().z - Vector3(0, this->parent->getScale().y / 2, 0) );
		}
		this->acc = Vector3(Math::RandFloatMinMax(-40, -40), -Math::RandFloatMinMax(200, 400), Math::RandFloatMinMax(-40, 40));
		this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->lifeSpan = Math::RandFloatMinMax(0.6f, 0.9f);
		this->setSpeed(Math::RandFloatMinMax(-45, 45));//setting speed for ROTATION btw
		break;
	case PTYPE_TEXT:
		this->acc = Vector3(0, -Math::RandFloatMinMax(600, 1000), 0);
		//this->endScale = Vector3(1.f, 1.f, 1.f) * Math::RandFloatMinMax(0.05f, 0.2f);
		this->setSpeed(0);
		this->lifeSpan = Math::RandFloatMinMax(1.f, 1.5f);
		if (this->parent != NULL)
		{
			this->setPos(this->parent->getPos());
			this->vel = Vector3(Math::RandFloatMinMax(-150, 150) , 500, Math::RandFloatMinMax(-150, 150));
			this->setScale(Vector3(0.1f, 0.1f, 0.1f));
			this->startScale = this->getScale();
			//this->endScale = this->parent->getScale()*Math::RandFloatMinMax(5.f, 7.15f);
			this->endScale = Vector3(6, 6, 6)*Math::RandFloatMinMax(5.f, 8.15f);
		}
		break;
	case PTYPE_ENEMYAPPEAR:
		this->acc.SetZero();
		this->setSpeed(0);
		this->lifeSpan = Math::RandFloatMinMax(0.1f, 0.15f);
		if (this->parent != NULL)
		{
			this->setPos(this->parent->getPos());
			this->originPosition = this->getPos();
			this->setScale(this->parent->getScale());
			this->startScale = this->getScale();
			this->endScale = this->startScale*Math::RandFloatMinMax(0.f, 0.05f);
		}
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
	case PTYPE_TEXT:
		if (this->animFrame < this->lifeSpan * 0.35f)
		{
			alphaVal = this->animFrame / (this->lifeSpan*0.35f);
			float backAlpha = Back::easeOut(alphaVal, 0, 1, 1);//can b optimised evnetual;ly
			float cosVar = cosf(this->animFrame*12.f);//coul dbe substituted with elapsedTime evebtuyaly
			this->setScale(this->startScale.lerped(this->endScale, backAlpha));
			this->transparency = Math::lerp(1.f, 0.1f, backAlpha);
			this->color.Set(this->startColor.r + 0.2f * cosVar, this->startColor.g + 0.2f * cosVar, this->startColor.b + 0.2f * cosVar);
		}
		else
		{
			alphaVal = (this->animFrame - this->lifeSpan * 0.35f) / (this->lifeSpan*0.65f);
			float quadAlpha = Quad::easeIn(alphaVal, 0, 1, 1);//can b optimised evnetual;ly
			float cosVar = cosf(this->animFrame*12.f);//coul dbe substituted with elapsedTime evebtuyaly
			this->setScale(this->endScale.lerped(this->startScale, quadAlpha));
			this->transparency = Math::lerp(0.1f, 1.f, quadAlpha);
			this->color.Set(this->startColor.r + 0.2f * cosVar, this->startColor.g + 0.2f * cosVar, this->startColor.b + 0.2f * cosVar);
		}
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

Vector3 CParticle_2::getOriginPos()
{
	return this->originPosition;
}

Color CParticle_2::getColor() const
{
	return this->color;
}
