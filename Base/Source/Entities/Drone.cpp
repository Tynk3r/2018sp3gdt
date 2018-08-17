#include "Drone.h"
#include "EntityManager.h"

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
	setCollider(true);
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
	//Vector3 viewVector = (getTarget() - getPos()).Normalized();
	wingRotation += 120 * dt;
	if (wingRotation >= 120) wingRotation = fmod(wingRotation, 120);
	switch (state)
	{
	case F_IDLE:
	{
		setTarget(getPos() + Vector3(0, 1 * dt, 0));
		if (getPos().y >= 200)
		{
			state = F_ROAM;
			setSpeed(0);
		}
	}
		break;
	case F_ROAM:
	{
		eyeRotation += 60 * dt;

		if (rt_scanTime <= 0.0)
		{
			if (eyeRotation >= 360) eyeRotation = fmod(eyeRotation, 360);
		
			////raycasting
			//std::list<CEntity*>::iterator it, it2, end;
			//end = EntityManager::GetInstance()->entityList.end();
			//for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
			//{
			//	if ((*it)->getType() == CEntity::E_ENEMY)
			//	{
			//		Vector3 tempView = (getTarget() - getPos()).Normalized();
			//		Vector3 tempProj = Vector3((*it)->getPos().x - getPos().x, 0, (*it)->getPos().z - getPos().z).Dot(Vector3(tempView.x, 0, tempView.z) * 50) / 50;
			//		if ( tempProj <= 50 && tempProj >= 0)
			//		{
			//			state = F_ATTACK;
			//			setSpeed(200);
			//			setTarget((*it)->getPos());
			//		}
			//	}
			//}

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
	CEntity::Update(dt);
	//setPos(getPos() + (viewVector * getSpeed() * (float)dt));
}
