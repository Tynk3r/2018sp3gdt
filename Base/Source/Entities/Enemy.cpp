#include "Enemy.h"

CEnemy::CEnemy(ENEMY_AI ai) : 
	CEntity(),
	state(F_IDLE),
	playerRef(NULL),
	enemyAI(ai)
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
		Vector3 enemytoplayer = (Vector3(plr->getPos().x, 0, plr->getPos().z) - Vector3(this->getPos().x, 0, this->getPos().z));
		float lengthSQ;
		try 
		{ 
			lengthSQ = enemytoplayer.LengthSquared();
			
		}
		catch (exception) { lengthSQ = 1; }
		switch (this->enemyAI)
		{
		case AI_SHORTRANGE:// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Short Range AI START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			if (lengthSQ < 80 * 80)//curernt hardcoded sphere "area" detection
			{
				this->state = F_IDLE; //close enough to attack so no need to move i guess
				this->setTarget(this->getPos() + enemytoplayer.Normalized()*0.05f);
				//std::cout << "enemy IDSLE" << std::endl;
			}
			else if (lengthSQ < 300 * 300)
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
			break;// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Short Range AI END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		case AI_LONGRANGE:// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Long Range AI START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			if (lengthSQ < 300 * 300)//player is getting too close!!!
			{
				this->state = F_RETREAT;
				this->setTarget(this->getPos() + enemytoplayer.Normalized()*-10.05f);//move AWAY from player
			}
			else if (lengthSQ < 600 * 600)//player is not too close and not too far
			{
				this->state = F_IDLE; //close enough to attack so no need to move i guess
				this->setTarget(this->getPos() + enemytoplayer.Normalized()*0.05f);
			}
			else if (lengthSQ < 900 * 900)//player is too far, getting close
			{
				this->state = F_ATTACK; //chase player until it gets close enuf
				Vector3 targ = Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z);
				this->setTarget(targ);
			}
			else//cant see in general, too far
			{
				this->state = F_ROAM; //since its too far to see te player it just roams around
			}
			break;// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Long Range AI END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		}
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

void CEnemy::setAI(ENEMY_AI ai)
{
	this->enemyAI = ai;
}
