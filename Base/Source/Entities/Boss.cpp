#include "Boss.h"
#include "../TimeTrackerManager.h"
#include "EasingStyles\QuadEase.h"
#include "EasingStyles\BackEase.h"
#include "../SoundEngine.h"
#include "Mtx44.h"
#include "../Particles/ParticleManager.h"
CBoss::CBoss(Vector3 pos, Vector3 scale, Vector3 target) :
	CEntity(),
	state(F_NORMAL),
	playerRef(NULL),
	elapsedTime(0),
	animFrame(0),
	rig(CRigInfo::RIG_BOSS),
	holdingProjectile(NULL),
	maxHealth(400),
	freezeTime(0),
	burnTime(0)
{
	target.y = pos.y;
	this->setPos(pos);
	this->setScale(scale);
	this->originalScale = scale;
	this->setTarget(target);
	this->health = this->maxHealth;
	this->setSpeed(70);
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
	float speedMultiplier = 1.f;
	if (this->IsOnFire() || this->IsFrozen())
		this->EmitParticles(Math::RandIntMinMax(1, 2));
	if (this->IsOnFire())
	{
		this->burnTime -= (float)dt;
		this->TakeDamage((float)dt*10.f);
	}
	else if (this->IsFrozen())
	{
		this->freezeTime -= (float)dt;
		speedMultiplier = 0.375f;
		dt *= speedMultiplier;
	}
	CEntity::Update(dt);
	elapsedTime += (float)dt;
	float tElapsedTime = TimeTrackerManager::GetInstance()->getElapsedTime()*speedMultiplier;
	this->setScale(Vector3(20 + 2 * cosf(tElapsedTime * 2), 60 + 3 * cosf(tElapsedTime * 4), 20 + 2 * cosf(tElapsedTime * 2)));
	//this->setTarget(this->getPos() + Vector3(cosf(tElapsedTime * 0.0f) * 50, 0, sinf(tElapsedTime * 0.0f) * 50));
	if (this->health == 0 && this->state != F_DEAD)
	{
		this->state = F_DEAD;
		this->elapsedTime = 0;
	}
	if (this->playerRef != NULL)
	{
		CPlayerInfo* plr = this->playerRef;
		float alpha;
		switch (this->state)
		{
		case F_ATTACK_FIREBALL:
			this->setSpeed(0);
			//std::cout << this->elapsedTime << std::endl;
			if (this->elapsedTime < 0.7f)
			{
				this->setTarget(Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z));
				alpha = this->elapsedTime / 0.7f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_FIREBALL_1);
				this->animFrame = Quad::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->holdingProjectile == NULL)
				{
					CProjectile* fBall = new CProjectile(CProjectile::PTYPE_FIRE);
					this->holdingProjectile = fBall;
					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					fBall->setScale(Vector3(1, 1, 1));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					fBall->Init(toSetfBallPos, plr->getPos(), this);
				}
				else
				{
					if (this->holdingProjectile->bossDone)
					{
						this->state = F_SURPRISED;
						this->holdingProjectile->setIsDone(true);
						this->animFrame = this->elapsedTime = 0;
						this->holdingProjectile = NULL;
						//break;
					}
					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					this->holdingProjectile->setScale(Vector3(1, 1, 1).lerped(Vector3(8, 8, 8), alpha));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					this->holdingProjectile->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.f)
			{
				this->setTarget(Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z));
				alpha = (this->elapsedTime - 0.7f) / 2.3f;
				if (this->holdingProjectile->bossDone)
				{
					this->state = F_SURPRISED;
					this->holdingProjectile->setIsDone(true);
					this->animFrame = this->elapsedTime = 0;
					this->holdingProjectile = NULL;
					//break;
				}
				if (this->holdingProjectile != NULL)
				{

					Vector3 rTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_RIGHT")->getPosOffset();
					Vector3 rTentactleFBALLPos = this->rig.GetJoint("OCTO_FIREBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((rTentaclePos + rTentactleFBALLPos)*this->originalScale.x);
					this->holdingProjectile->setScale(Vector3(8, 8, 8).lerped(Vector3(40, 40, 40), alpha));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					this->holdingProjectile->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.5f)
			{
				alpha = (this->elapsedTime - 3.f) * 2.f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_FIREBALL_2);
				this->animFrame = Back::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->holdingProjectile != NULL)
				{
					float dist = 1;
					if (!(plr->getPos() - this->holdingProjectile->getPos()).IsZero())
					{
						dist = (plr->getPos() - this->holdingProjectile->getPos()).Length();
					}
					this->holdingProjectile->setTarget(plr->getPos() - this->holdingProjectile->getGrav()*dist);
					//this->holdingProjectile->setSource(nullptr);//a different source other than null
					this->holdingProjectile = NULL;
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
		case F_ATTACK_ICEBALL:
			//std::cout << this->elapsedTime << std::endl;
			this->setSpeed(70);
			if (this->elapsedTime < 0.7f)
			{
				this->setTarget(Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z));
				alpha = this->elapsedTime / 0.7f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_ICEBALL_1);
				this->animFrame = Quad::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->holdingProjectile == NULL)
				{
					CProjectile* fBall = new CProjectile(CProjectile::PTYPE_ICE);
					this->holdingProjectile = fBall;
					Vector3 lTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_LEFT")->getPosOffset();
					Vector3 lTentactleIBALLPos = this->rig.GetJoint("OCTO_ICEBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((lTentaclePos + lTentactleIBALLPos)*this->originalScale.x);
					fBall->setScale(Vector3(1, 1, 1));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					fBall->Init(toSetfBallPos, plr->getPos(), this);
				}
				else
				{
					if (this->holdingProjectile->bossDone)
					{
						this->state = F_SURPRISED;
						this->holdingProjectile->setIsDone(true);
						this->animFrame = this->elapsedTime = 0;
						this->holdingProjectile = NULL;
						//break;
					}
					Vector3 lTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_LEFT")->getPosOffset();
					Vector3 lTentactleIBALLPos = this->rig.GetJoint("OCTO_ICEBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((lTentaclePos + lTentactleIBALLPos)*this->originalScale.x);
					this->holdingProjectile->setScale(Vector3(1, 1, 1).lerped(Vector3(8, 8, 8), alpha));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					this->holdingProjectile->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.f)
			{
				this->setTarget(Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z));
				alpha = (this->elapsedTime - 0.7f) / 2.3f;
				if (this->holdingProjectile->bossDone)
				{
					this->state = F_SURPRISED;
					this->holdingProjectile->setIsDone(true);
					this->animFrame = this->elapsedTime = 0;
					this->holdingProjectile = NULL;
					//break;
				}
				if (this->holdingProjectile != NULL)
				{

					Vector3 lTentaclePos = this->rig.GetJoint("OCTO_TENTACLE_LOWER_LEFT")->getPosOffset();
					Vector3 lTentactleIBALLPos = this->rig.GetJoint("OCTO_ICEBALL")->getPosOffset();
					Mtx44 mat;
					mat.SetToIdentity();
					mat.SetToRotation(Math::RadianToDegree(atan2(this->getTarget().x - this->getPos().x, this->getTarget().z - this->getPos().z)), 0, 1, 0);
					Vector3 offset = mat * ((lTentaclePos + lTentactleIBALLPos)*this->originalScale.x);
					this->holdingProjectile->setScale(Vector3(8, 8, 8).lerped(Vector3(40, 40, 40), alpha));
					Vector3 toSetfBallPos = this->getPos() + offset + Vector3(0, this->holdingProjectile->getScale().x, 0);
					this->holdingProjectile->setPos(toSetfBallPos);
				}
			}
			else if (this->elapsedTime < 3.5f)
			{
				alpha = (this->elapsedTime - 3.f) * 2.f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_ICEBALL_2);
				this->animFrame = Back::easeOut(alpha, 0, 1, 1);
				this->rig.Animate(animFrame);
				if (this->holdingProjectile != NULL)
				{
					float dist = 1; 
					if (!(plr->getPos() - this->holdingProjectile->getPos()).IsZero())
					{
						dist = (plr->getPos() - this->holdingProjectile->getPos()).Length();
					}
					this->holdingProjectile->setTarget(plr->getPos() - this->holdingProjectile->getGrav()*dist);
					//this->holdingProjectile->setSource(nullptr);//a different source other than null
					this->holdingProjectile = NULL;
					CSoundEngine::GetInstance()->PlayASound("Iceattack");
				}
			}
			else if (this->elapsedTime > 4.5f)
			{
				this->state = F_NORMAL;
				this->elapsedTime = 0;
				this->rig.SetStartKeyframe(CJointInfo::KEYFRAME_NONE);
				this->rig.SetGoalKeyframe(CJointInfo::KEYFRAME_NONE);
				//this->rig.Animate(1);
			}
			break;
		case F_SURPRISED:
			this->setSpeed(0);
			if (this->elapsedTime < 0.4f)
			{
				alpha = this->elapsedTime / 0.4f;
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_SURPRISED);
				this->rig.Animate(Back::easeOut(alpha, 0, 1, 1));
			}
			else if (this->elapsedTime > 1)
			{
				this->state = F_VULNERABLE;
				this->elapsedTime = this->animFrame = 0;
			}
			break;
		case F_VULNERABLE:
			if (this->elapsedTime < 3.f)
			{
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_VULNERABLE);
				this->rig.Animate(1);
				this->setScale(Vector3(20 + 2 * cosf(this->elapsedTime * 20), 60 + 6 * cosf(this->elapsedTime * 40), 20 + 2 * cosf(this->elapsedTime * 20)));
				this->setTarget(this->getPos() + Vector3(cosf(this->elapsedTime * 10.f) * 50, 0, sinf(this->elapsedTime * 10.f) * 50));
			}
			else
			{
				this->state = F_NORMAL;
				this->elapsedTime = this->animFrame = 0;
			}
			break;
		case F_DEAD:
			this->setSpeed(0);
			alpha = Math::Min(1.f, this->elapsedTime*0.3f);
			this->setScale(Vector3(20, 60, 20).lerped(Vector3(40, 5, 40), alpha));
			if (alpha < 1)
			{
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_OCTO_DEAD);
				this->rig.Animate(Quad::easeOut(alpha, 0, 1, 1));
			}
			break;
		case F_NORMAL:
			this->setSpeed(70);
			this->setTarget(Vector3(plr->getPos().x, this->getPos().y, plr->getPos().z));
			if (this->elapsedTime < 0.75f)
			{
				this->rig.MoveToKeyframe(CJointInfo::KEYFRAME_NONE);
				this->rig.Animate(Math::Min(1.f, Quad::easeOut(this->elapsedTime/0.75f, 0, 1, 1)));
			}
			if (this->elapsedTime > 3.f)
			{
				this->state = Math::RandFloatMinMax(0, 10000) > 5000 ? F_ATTACK_FIREBALL : F_ATTACK_ICEBALL;
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

float CBoss::getCurrHealth() const
{
	return this->health;
}

float CBoss::getMaxHealth() const
{
	return this->maxHealth;
}

float CBoss::TakeDamage(CEntity * ent)
{
	float damageMultiplier = 0.1f;
	if (this->state == F_VULNERABLE || this->state == F_SURPRISED)
		damageMultiplier = 1;
	float damagetodo = 0;
	CProjectile* proj = dynamic_cast<CProjectile*>(ent);
	if (proj)
	{
		switch (proj->getProjType())
		{
		case CProjectile::PTYPE_SPECIAL_KILLERNADO:
			damagetodo = 30 * damageMultiplier;
			break;
		case CProjectile::PTYPE_BEAM:
			damagetodo = 40 * damageMultiplier;
			break;
		case CProjectile::PTYPE_ICE:
			damagetodo = 20 * damageMultiplier;
			break;
		case CProjectile::PTYPE_FIRE:
			damagetodo = 30 * damageMultiplier;
			break;
		}
	}
	this->health -= damagetodo;
	if (this->health < 0)
		this->health = 0;
	return damagetodo;
}

void CBoss::SetBurnTime(float burntime)
{
	if (this->IsFrozen())
		this->burnTime = this->freezeTime = 0;
	else if (this->getState() == CBoss::F_VULNERABLE || this->getState() == CBoss::F_SURPRISED || this->getState() == CBoss::F_DEAD)
		this->burnTime = burntime;
}

void CBoss::SetFreezeTime(float freezetime)
{
	if (this->IsOnFire())
		this->burnTime = this->freezeTime = 0;
	else if (this->getState() == CBoss::F_VULNERABLE || this->getState() == CBoss::F_SURPRISED || this->getState() == CBoss::F_DEAD)
		this->freezeTime = freezetime;
}
;
bool CBoss::IsOnFire()
{
	return this->burnTime > 0;
}

bool CBoss::IsFrozen()
{
	return this->freezeTime > 0;
}

void CBoss::TakeDamage(float damage)
{
	this->health -= damage;
	if (this->health < 0)
		this->health = 0;
}
