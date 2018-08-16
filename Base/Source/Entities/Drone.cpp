#include "Drone.h"

CDrone::CDrone() :
	CEntity(),
	state(F_IDLE)
{
}


CDrone::~CDrone()
{
}

void CDrone::Init()
{
	setType(E_DRONE);
	wingRotation = 0;
	eyeRotation = 0;
	rt_scanTime = 0.5;
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
}

float CDrone::getWingRotation() const
{
	if (wingRotation >= 60) return 120 - wingRotation;
	else return wingRotation;
}

void CDrone::Update(double dt)
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	wingRotation += 120 * dt;
	if (wingRotation >= 120) wingRotation = fmod(wingRotation, 120);
	switch (state)
	{
	case F_IDLE:
	{
		setPos(getPos() + Vector3(0, 50 * dt, 0));
		if (getPos().y >= 200) state = F_ROAM;
	}
		break;
	case F_ROAM:
	{
		eyeRotation += 60 * dt;

		if (rt_scanTime <= 0.0)
		{
			if (eyeRotation >= 360) eyeRotation = fmod(eyeRotation, 360);
		}
		setTarget(getPos() + Vector3(sinf(Math::DegreeToRadian(eyeRotation)), 0, cosf(Math::DegreeToRadian(eyeRotation))) );
		rt_scanTime -= dt;
	}
		break;
	case F_ATTACK:
	{
		
		//LAUNCH FIREBALL WHEN CLOSE ENOUGH, THEN GO BACK TO F_ROAM


	}
		break;
	default:
		break;
	}

	//setPos(getPos() + (viewVector * getSpeed() * (float)dt));
}
