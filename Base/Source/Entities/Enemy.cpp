#include "Enemy.h"

CEnemy::CEnemy() : 
	CEntity(),
	state(F_IDLE),
	playerRef(NULL)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init()
{
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
	setType(E_ENEMY);
}

void CEnemy::Update(double dt)
{
	CEntity::Update(dt);
	if (this->playerRef != NULL)
	{
		CPlayerInfo* plr = this->playerRef;
		Vector3 enemytoplayer = (plr->getPos() - this->getPos());
		try 
		{ 
			float lengthSQ = enemytoplayer.LengthSquared();
			if (lengthSQ < 80 * 80)//curernt hardcoded sphere "area" detection
			{
				this->state = F_IDLE; //close enough to attack so no need to move i guess
				//std::cout << "enemy IDSLE" << std::endl;
			}
			else if (lengthSQ < 300 * 300)
			{
				this->state = F_ATTACK; //chase player until it gets close enuf
				this->setTarget(plr->getPos());
				//std::cout << "enemy ATACK" << std::endl;
			}
			else
			{
				this->state = F_ROAM; //since its too far to see te player it just roams around
				//std::cout << "enemy rOAM" << std::endl;
			}
		}
		catch (exception) {}
	}
}

void CEnemy::tempMoveBack(float dt)
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	this->setPos(this->getPos() - viewVector*this->getSpeed()*dt);
}

void CEnemy::setPlayerRef(CPlayerInfo * playerRef)
{
	this->playerRef = playerRef;
}
