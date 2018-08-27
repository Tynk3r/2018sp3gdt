#include "FlyingWitch.h"

CFlyingWitch::CFlyingWitch() : 
	CEnemy(AI_FLYING),
	witchCooldown(3),
	witchProjectile(NULL),
	witchState(WS_IDLE)
{
}


CFlyingWitch::~CFlyingWitch()
{
}

void CFlyingWitch::Update(double dt)
{
	CEnemy::Update(dt);
	switch (this->witchState)
	{
	case WS_IDLE://<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< handle Witch IDLE START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		switch (this->state)
		{
		case F_RETREAT:
			witchCooldown = Math::Max(1.f, witchCooldown);
			break;
		case F_IDLE:
			witchCooldown -= (float)dt;
			if (witchCooldown < 0)
			{
				witchState = WS_CHARGING;
				this->setSpeed(0);
				witchCooldown = 0;//used as anim frame now muhahahaha
				this->witchProjectile = Math::RandIntMinMax(0, 1000) < 500 ? new CProjectile(CProjectile::PTYPE_FIRE) : new CProjectile(CProjectile::PTYPE_ICE);
				this->witchProjectile->Init(this->getPos() + Vector3(0, this->getScale().y * 3.5f, 0), this->playerRef->getPos(), this);
				this->witchProjectile->setScale(Vector3(1, 1, 1));
			}
			break;
		}
		break;//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< handle Witch IDLE END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	case WS_CHARGING:
		witchCooldown += (float)dt;
		this->witchProjectile->setPos(this->getPos() + Vector3(0, (this->getScale().y + this->witchProjectile->getScale().y)*3.5f, 0));
		this->witchProjectile->setScale(Vector3(1, 1, 1).lerped(Vector3(25, 25, 25), this->witchCooldown*0.2f));
		this->setTarget(this->playerRef->getPos());
		if (witchProjectile->bossDone)
		{
			this->witchProjectile = NULL;
			this->witchCooldown = 0;
			this->witchState = WS_SURPRISED;
		}
		else if (witchCooldown > 5.f)
		{
			float dist = 1;
			if (!(this->playerRef->getPos() - this->witchProjectile->getPos()).IsZero())
			{
				dist = (this->playerRef->getPos() - this->witchProjectile->getPos()).Length();
			}
			this->witchProjectile->setTarget(this->playerRef->getPos() - this->witchProjectile->getGrav()*dist*0.7f);
			this->witchProjectile = NULL;
			this->witchCooldown = 0;
			this->witchState = WS_FIRED;
		}
		break;
	case WS_FIRED:
		witchCooldown += (float)dt;
		this->setTarget(this->playerRef->getPos());
		if (witchCooldown > 1.5f)
		{
			this->witchCooldown = Math::RandIntMinMax(3, 5);
			this->witchState = WS_IDLE;
			this->setSpeed(100);
		}
		break;
	case WS_SURPRISED:
		witchCooldown += (float)dt;
		if (witchCooldown > 1.5f)
		{
			this->witchCooldown = Math::RandIntMinMax(3, 5);
			this->witchState = WS_IDLE;
			this->setSpeed(100);
		}
		break;
	}
}