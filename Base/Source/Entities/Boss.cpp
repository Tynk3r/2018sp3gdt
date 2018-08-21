#include "Boss.h"

CBoss::CBoss(Vector3 pos, Vector3 scale, Vector3 target) :
	CEntity(),
	state(F_IDLE),
	playerRef(NULL)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setTarget(target);
}


CBoss::~CBoss()
{
}

void CBoss::Init()
{
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
	setType(E_BOSS);
}

void CBoss::Update(double dt)
{
	CEntity::Update(dt);
	if (this->playerRef != NULL)
	{
		/*CPlayerInfo* plr = this->playerRef;
		Vector3 enemytoplayer = (plr->getPos() - this->getPos());
		try 
		{ 
			float lengthSQ = enemytoplayer.LengthSquared();
			if (lengthSQ < 800 * 800)//curernt hardcoded sphere "area" detection
			{
				this->state = F_IDLE; //close enough to attack so no need to move i guess
				this->setTarget(this->getPos() + enemytoplayer.Normalized()*0.05f);
				//std::cout << "enemy IDSLE" << std::endl;
			}
			else if (lengthSQ < 3000 * 3000)
			{
				this->state = F_ATTACK; //chase player until it gets close enuf
				Vector3 targ = Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z);
				this->setTarget(targ);
				//std::cout << "enemy ATACK" << std::endl;
			}
			else
			{
				this->state = F_ROAM; //since its too far to see te player it just roams around
				//std::cout << "enemy rOAM" << std::endl;
			}
		}
		catch (exception) {}*/
	}
}

void CBoss::tempMoveBack(float dt)
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	this->setPos(this->getPos() - viewVector*this->getSpeed()*dt);
}

void CBoss::setPlayerRef(CPlayerInfo * playerRef)
{
	this->playerRef = playerRef;
}
