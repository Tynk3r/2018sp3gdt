#include "Boss.h"
#include "../TimeTrackerManager.h"
#include "EasingStyles\QuadEase.h"
#include "EasingStyles\BackEase.h"
#include "../SoundEngine.h"
#include "Mtx44.h"
CBoss::CBoss(Vector3 pos, Vector3 scale, Vector3 target) :
	CEntity(),
	state(F_NORMAL),
	playerRef(NULL),
	elapsedTime(0),
	animFrame(0),
	rig(CRigInfo::RIG_BOSS),
	fireball(NULL)
{
	target.y = pos.y;
	this->setPos(pos);
	this->setScale(scale);
	this->originalScale = scale;
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
	elapsedTime += (float)dt;
	float tElapsedTime = TimeTrackerManager::GetInstance()->getElapsedTime();
	this->setScale(Vector3(20 + 2 * cosf(tElapsedTime * 2), 60 + 3 * cosf(tElapsedTime * 4), 20 + 2 * cosf(tElapsedTime * 2)));
	this->setTarget(this->getPos() + Vector3(cosf(tElapsedTime * 0.0f) * 50, 0, sinf(tElapsedTime * 0.0f) * 50));
	if (this->playerRef != NULL)
	{
		CPlayerInfo* plr = this->playerRef;
		float alpha;
		switch (this->state)
		{
		case F_ATTACK_FIREBALL:
			//std::cout << this->elapsedTime << std::endl;
			if (this->elapsedTime < 0.7f)
			{
				alpha = this->elapsedTime / 0.7f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_FIREBALL_1);
				this->animFrame = Quad::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->fireball == NULL)
				{
					CProjectile* fBall = new CProjectile(CProjectile::PTYPE_FIRE);
					this->fireball = fBall;
					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					Vector3 toSetfBallPos = this->getPos() + offset;
					fBall->Init(toSetfBallPos, plr->getPos(), this);
				}
				else
				{
					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					Vector3 toSetfBallPos = this->getPos() + offset;
					this->fireball->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.f)
			{
				if (this->fireball != NULL)
				{
					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					Vector3 toSetfBallPos = this->getPos() + offset;
					this->fireball->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.5f)
			{
				alpha = (this->elapsedTime - 3.f) * 2.f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_FIREBALL_2);
				this->animFrame = Back::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->fireball != NULL)
				{
					this->fireball = NULL;
					CSoundEngine::GetInstance()->PlayASound("Fireball");
				}
			}
			else if (this->elapsedTime > 4.5f)
			{
				this->state = F_NORMAL;
				this->elapsedTime = 0;
				this->rig.SetStartKeyframe(CJointInfo::KEYFRAME_NONE);
				this->rig.SetGoalKeyframe(CJointInfo::KEYFRAME_NONE);
				this->rig.Animate(1);
			}
			break;
		case F_NORMAL:
			if (this->elapsedTime > 8.f)
			{
				this->state = F_ATTACK_FIREBALL;
				this->elapsedTime = this->animFrame = 0;
			}
			break;
		}
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

Vector3 CBoss::getOrigScale() const
{
	return this->originalScale;
}

float CBoss::getElapsedTime() const
{
	return this->elapsedTime;
}
