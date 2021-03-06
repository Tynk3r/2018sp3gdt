#include "EntityManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Enemy.h"
#include "Projectile.h"
#include <iostream>
#include "../SoundEngine.h"
#include "../Particles/ParticleManager.h"
#include "Boss.h"
#include "../CameraEffects/CameraEffectManager.h"
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<CEntity*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}
	//if (CheckForCollision(_dt)) { cout << "Collision Detected" << endl; }
	CheckForCollision(_dt);
	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->isDone() && (*it)->getType() != CEntity::E_TARGET && (*it)->getType() != CEntity::E_MOVING_TARGET && (*it)->getType() != CEntity::E_TARGET_FIRE && (*it)->getType() != CEntity::E_TARGET_ICE)
		{
			// Delete if done
			CEntity* point = *it;
			delete *it;
			*it = NULL;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(CEntity* _newEntity)
{
	entityList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(CEntity* _existingEntity)
{
	// Find the entity's iterator
	std::list<CEntity*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	/*if (   (thisMinAABB.x >= thatMinAABB.x && thisMinAABB.x <= thatMaxAABB.x) || (thisMaxAABB.x >= thatMinAABB.x && thisMaxAABB.x <= thatMaxAABB.x)
		&& (thisMinAABB.y >= thatMinAABB.y && thisMinAABB.y <= thatMaxAABB.y) || (thisMaxAABB.y >= thatMinAABB.y && thisMaxAABB.y <= thatMaxAABB.y)
		&& (thisMinAABB.z >= thatMinAABB.z && thisMinAABB.z <= thatMaxAABB.z) || (thisMaxAABB.z >= thatMinAABB.z && thisMaxAABB.z <= thatMaxAABB.z))
	{
		return true;
	}
	if (   (thatMinAABB.x >= thisMinAABB.x && thatMinAABB.x <= thisMaxAABB.x) || (thatMaxAABB.x >= thisMinAABB.x && thatMaxAABB.x <= thisMaxAABB.x)
		&& (thatMinAABB.y >= thisMinAABB.y && thatMinAABB.y <= thisMaxAABB.y) || (thatMaxAABB.y >= thisMinAABB.y && thatMaxAABB.y <= thisMaxAABB.y)
		&& (thatMinAABB.z >= thisMinAABB.z && thatMinAABB.z <= thisMaxAABB.z) || (thatMaxAABB.z >= thisMinAABB.z && thatMaxAABB.z <= thisMaxAABB.z))
	{
		return true;
	}
	if (   (thisMinAABB.x >= thatMinAABB.x && thisMinAABB.x <= thatMaxAABB.x) && (thisMaxAABB.x >= thatMinAABB.x && thisMaxAABB.x <= thatMaxAABB.x)
		&& (thisMinAABB.y >= thatMinAABB.y && thisMinAABB.y <= thatMaxAABB.y) && (thisMaxAABB.y >= thatMinAABB.y && thisMaxAABB.y <= thatMaxAABB.y)
		&& (thisMinAABB.z >= thatMinAABB.z && thisMinAABB.z <= thatMaxAABB.z) && (thisMaxAABB.z >= thatMinAABB.z && thisMaxAABB.z <= thatMaxAABB.z))
	{
		return true;
	}
	if (   (thatMinAABB.x >= thisMinAABB.x && thatMinAABB.x <= thisMaxAABB.x) && (thatMaxAABB.x >= thisMinAABB.x && thatMaxAABB.x <= thisMaxAABB.x)
		&& (thatMinAABB.y >= thisMinAABB.y && thatMinAABB.y <= thisMaxAABB.y) && (thatMaxAABB.y >= thisMinAABB.y && thatMaxAABB.y <= thisMaxAABB.y)
		&& (thatMinAABB.z >= thisMinAABB.z && thatMinAABB.z <= thisMaxAABB.z) && (thatMaxAABB.z >= thisMinAABB.z && thatMaxAABB.z <= thisMaxAABB.z))
	{
		return true;
	}*/

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(CEntity *ThisEntity, CEntity *ThatEntity)
{
	Vector3 thisMinAABB = ThisEntity->getMinAABB();
	Vector3 thisMaxAABB = ThisEntity->getMaxAABB();
	Vector3 thatMinAABB = ThatEntity->getMinAABB();
	Vector3 thatMaxAABB = ThatEntity->getMaxAABB();

	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) > DistanceSquaredBetween(ThisEntity->getPos(), ThatEntity->getPos()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(CEntity *ThisEntity, CEntity *ThatEntity)
{
	if (!ThisEntity->hasCollider() || !ThatEntity->hasCollider() || ThisEntity == ThatEntity || ThisEntity->isDone() || ThatEntity->isDone()) { return false; }

	Vector3 thisMinAABB = /*ThisEntity->getPos() +*/ ThisEntity->getMinAABB();
	Vector3 thisMaxAABB = /*ThisEntity->getPos() +*/ ThisEntity->getMaxAABB();
	Vector3 thatMinAABB = /*ThatEntity->getPos() +*/ ThatEntity->getMinAABB();
	Vector3 thatMaxAABB = /*ThatEntity->getPos() +*/ ThatEntity->getMaxAABB();

	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB) || CheckOverlap(altThatMinAABB, altThatMaxAABB, thisMinAABB, thisMaxAABB))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(float dt)
{
	// Update all entities
	std::list<CEntity*>::iterator it, it2, end;
	end = entityList.end();
	bool hasDoneCollide = false;
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->getType() == CEntity::E_PLAYER || !(*it)->hasCollider() || (*it)->isDone())
			continue;
		Vector3 viewVector = ((*it)->getTarget() - (*it)->getPos()).Normalized();
		for (it2 = entityList.begin(); it2 != end; ++it2)
		{
			if ((*it) == (*it2) || !(*it2)->hasCollider() || (*it2)->isDone())
				continue;
			switch ((*it)->getType()) //pre-collision check updates
			{
			case CEntity::E_ENEMY:
			{
				if ((*it2)->getType() == CEntity::E_PLAYER)
				{
					CEnemy* enemy = static_cast<CEnemy*>((*it));
					CPlayerInfo* player= static_cast<CPlayerInfo*>((*it2));
					enemy->setPlayerRef(player);
				}
				break;
			}
			case CEntity::E_PLAYER:
			{
				if ((*it2)->getType() == CEntity::E_ENEMY)
				{
					CEnemy* enemy = static_cast<CEnemy*>((*it2));
					CPlayerInfo* player = static_cast<CPlayerInfo*>((*it));
					enemy->setPlayerRef(player);
				}
				break;
			}
			default:
				break;
			}						//pre-collision check updates <<<<<<<<<<<<END>>>>>>>>>>>>

			bool notBeam = true;
			if ((*it2)->getType() == CEntity::E_PROJECTILE)
			{
				CProjectile* beamCheck = static_cast<CProjectile*>((*it2));
				if (beamCheck->getProjType() == CProjectile::PTYPE_BEAM)
				{
					notBeam = false;
				}
			}

			if ((CheckAABBCollision(*it, *it2) || CheckSphereCollision(*it, *it2)) && notBeam) 
			{ 
				switch ((*it)->getType())
				{
				case CEntity::E_WALL:
				case CEntity::E_PILLAR:
				case CEntity::E_ROCKS:
					break;
				case CEntity::E_BOSS:
					if ((*it2)->getType() == CEntity::E_PROJECTILE) //&& CheckAABBCollision(*it, *it2))
					{
						CProjectile* proj = static_cast<CProjectile*>(*it2);
						CBoss* bos = static_cast<CBoss*>(*it);
						if (proj->getSource() != (*it))
						{
							if (proj->getProjType() != CProjectile::PTYPE_BEAM)
								proj->setIsDone(true);
							proj->EmitParticles(Math::RandIntMinMax(16, 32));
							CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + proj->getScale().x*0.5f);
							CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + proj->getScale().x*0.015f);
							//CSoundEngine::GetInstance()->AddSound("barrelbreak", "Sound//barrelbreak.mp3");
							//CSoundEngine::GetInstance()->PlayASound("barrelbreak");
							if (proj->getSource() == CPlayerInfo::GetInstance() || proj->getSource() == NULL)
							{
								int damagetaken = bos->TakeDamage(proj);
								if (proj->getProjType() == CProjectile::PTYPE_FIRE)
								{
									ParticleManager::GetInstance()->AddParticle(std::to_string(damagetaken) + " Damage!", (*it2), Color(1, 0.1f, 0.1f));
									bos->SetBurnTime();
									CSoundEngine::GetInstance()->PlayASound("floorImpact");
								}
								else if (proj->getProjType() == CProjectile::PTYPE_ICE)
								{
									ParticleManager::GetInstance()->AddParticle(std::to_string(damagetaken) + " Damage!", (*it2), Color(0.1f, 0.1f, 1.f));
									bos->SetFreezeTime();
									CSoundEngine::GetInstance()->PlayASound("IceImpact");
								}
							}
						}
					}
					break;
				case CEntity::E_ENEMY:
					if ((*it2)->getType() == CEntity::E_PROJECTILE)
					{
						CProjectile* proj = static_cast<CProjectile*>((*it2));
						if (proj->getSourceType() != (*it)->getType() || (proj->getSource() == NULL) || proj->getSource() == CPlayerInfo::GetInstance())
						{
							CEnemy* tempEnemy = (CEnemy*)*it;
							if (tempEnemy->getAI() != CEnemy::AI_LONGRANGEFOG) { (*it)->setIsDone(true); }
							if (proj->getProjType() != CProjectile::PTYPE_BEAM && proj->getProjType() != CProjectile::PTYPE_SPECIAL_KILLERNADO)
							{
								(*it2)->setIsDone(true);
								CSoundEngine::GetInstance()->PlayASound("floorImpact");
							}
							CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + proj->getScale().x*0.5f);
							CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + proj->getScale().x*0.015f);
							proj->EmitParticles(Math::RandIntMinMax(16, 32));
							if (proj->getProjType() == CProjectile::PTYPE_FIRE)
							{
								CSoundEngine::GetInstance()->PlayASound("floorImpact");
							}
							else if (proj->getProjType() == CProjectile::PTYPE_ICE)
							{
								CSoundEngine::GetInstance()->PlayASound("IceImpact");
							}
							switch ((*it)->getType())
							{
							case CEntity::E_ENEMY:
								if (tempEnemy->getAI() != CEnemy::AI_LONGRANGEFOG) {
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 5);
									ParticleManager::GetInstance()->AddParticle("+5 Points", (*it2), Color(0.1f, 1, 0.1f));
								}
								break;
							case CEntity::E_MOVING_TARGET:
								CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
								ParticleManager::GetInstance()->AddParticle("+3 Points", (*it2), Color(0.1f, 1, 0.1f));
								CSoundEngine::GetInstance()->AddSound("barrelbreak", "Sound//barrelbreak.mp3");
								CSoundEngine::GetInstance()->PlayASound("barrelbreak");
								break;
							case CEntity::E_TARGET:
								CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 1);
								ParticleManager::GetInstance()->AddParticle("+1 Points", (*it2), Color(0.1f, 1, 0.1f));
								CSoundEngine::GetInstance()->AddSound("barrelbreak", "Sound//barrelbreak.mp3");
								CSoundEngine::GetInstance()->PlayASound("barrelbreak");
								break;
							}
							cout << "Score: " << CPlayerInfo::GetInstance()->GetScore() << endl;
						}
					}
					break;
				case CEntity::E_PROJECTILE:
				{
					CProjectile* proj1 = dynamic_cast<CProjectile*>(*(it));
					if ((*it2)->getType() == CEntity::E_PLAYER) 
					{ 
						if (proj1 && proj1->getType() == CEntity::E_PROJECTILE && proj1->getSource() != NULL && proj1->getSource() != nullptr && (proj1->getSource()->getType() == CEntity::E_BOSS || proj1->getSource()->getType() == CEntity::E_ENEMY))
						{
							proj1->setIsDone(true);
							proj1->EmitParticles(Math::RandIntMinMax(16, 32));
							CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + proj1->getScale().x*0.5f);
							CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + proj1->getScale().x*0.015f);
							
							if (proj1->getSource()->getType() == CEntity::E_BOSS) CPlayerInfo::GetInstance()->setHealth(CPlayerInfo::GetInstance()->GetHealth() - 20);
							else if (proj1->getSource()->getType() == CEntity::E_ENEMY) CPlayerInfo::GetInstance()->setHealth(CPlayerInfo::GetInstance()->GetHealth() - 10);
							CameraEffectManager::GetInstance()->AddCamEffect(CameraEffect::CE_TYPE_BLOODSCREEN);
							if (proj1->getProjType()==CProjectile::PTYPE_FIRE)
								CSoundEngine::GetInstance()->PlayASound("floorImpact");
							else if (proj1->getProjType() == CProjectile::PTYPE_ICE)
								CSoundEngine::GetInstance()->PlayASound("IceImpact");
						}
						break;
					}
					CProjectile* proj2 = dynamic_cast<CProjectile*>(*(it2));
					if (proj2 && proj1->getSource() != proj2->getSource() && proj1->getProjType() != proj2->getProjType())
					{
						if ((proj1->getSource() != NULL || proj1->getSource() != nullptr) && proj1->getSource()->getType() == CEntity::E_BOSS)//if proj1 beglons to boss
						{
							//proj1->bossDone = true;
							proj2->setIsDone(true); proj1->setIsDone(true);
						}
						else if ((proj2->getSource() || proj2->getSource() != nullptr) != NULL && proj2->getSource()->getType() == CEntity::E_BOSS)
						{
							//proj2->bossDone = true;
							proj1->setIsDone(true); proj2->setIsDone(true);
						}
						else if ((proj1->getSource() != proj2->getSource()))
						{
							proj1->setIsDone(true);
							proj2->setIsDone(true);
						}
						if (proj1->getProjType() == CProjectile::PTYPE_SPECIAL_KILLERNADO || proj1->getProjType() == CProjectile::PTYPE_BEAM)
							proj1->setIsDone(false);
						if (proj2->getProjType() == CProjectile::PTYPE_SPECIAL_KILLERNADO || proj2->getProjType() == CProjectile::PTYPE_BEAM)
							proj2->setIsDone(false);
						proj1->EmitParticles(Math::RandIntMinMax(16, 32));
						proj2->EmitParticles(Math::RandIntMinMax(16, 32));
						CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + (proj1->getScale().x + proj2->getScale().x)*0.5f*0.5f);
						CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + (proj1->getScale().x + proj2->getScale().x)*0.015f*0.5f);
						CSoundEngine::GetInstance()->PlayASound("IceImpact");
						CSoundEngine::GetInstance()->PlayASound("floorImpact");
						//CSoundEngine::GetInstance()->AddSound("barrelbreak", "Sound//barrelbreak.mp3");
						//CSoundEngine::GetInstance()->PlayASound("barrelbreak");

					}
				}
				break;
				case CEntity::E_TARGET:
				case CEntity::E_MOVING_TARGET:
				case CEntity::E_TARGET_FIRE:
				case CEntity::E_TARGET_ICE:
					if ((*it2)->getType() == CEntity::E_PROJECTILE)
					{
						CProjectile* proj = static_cast<CProjectile*>((*it2));
						if ((*it)->getType() == CEntity::E_TARGET_ICE && (proj->getProjType() != CProjectile::PTYPE_FIRE && proj->getProjType() != CProjectile::PTYPE_SPECIAL_KILLERNADO))
						{
							if (proj->getProjType() != CProjectile::PTYPE_BEAM) 
								(*it2)->setIsDone(true);
							break;
						}
						if ((*it)->getType() == CEntity::E_TARGET_FIRE && proj->getProjType() != CProjectile::PTYPE_ICE)
						{
							if (proj->getProjType() != CProjectile::PTYPE_BEAM && proj->getProjType() != CProjectile::PTYPE_SPECIAL_KILLERNADO) 
								(*it2)->setIsDone(true);
						
							if (proj->getSpellModType() == CProjectile::SMTYPE_SPECIAL)
							{
								CProjectile* aa = new CProjectile(CProjectile::PTYPE_SPECIAL_KILLERNADO);
								Vector3 viewvec = (proj->getTarget() - proj->getPos()).Normalized();
								viewvec.y = 0;
								viewvec.Normalize();
								aa->Init(proj->getPos() + viewvec, proj->getPos() + viewvec * 1.5);
								aa->setPos(aa->getPos() + Vector3(0, aa->getScale().y, 0));
								aa->setTarget(aa->getTarget() + Vector3(0, aa->getScale().y, 0));
							}
							break;
						}
						(*it)->setIsDone(true);
						if (proj->getProjType() != CProjectile::PTYPE_BEAM && proj->getProjType() != CProjectile::PTYPE_SPECIAL_KILLERNADO)
						{
							(*it2)->setIsDone(true);
						}

						if (proj->getProjType() == CProjectile::PTYPE_FIRE && proj->getSpellModType() == CProjectile::SMTYPE_SPECIAL)
						{
							CProjectile* aa = new CProjectile(CProjectile::PTYPE_SPECIAL_KILLERNADO);
							Vector3 viewvec = (proj->getTarget() - proj->getPos()).Normalized();
							viewvec.y = 0;
							viewvec.Normalize();
							aa->Init(proj->getPos() + viewvec, proj->getPos() + viewvec * 1.5);
							aa->setPos(aa->getPos() + Vector3(0, aa->getScale().y, 0));
							aa->setTarget(aa->getTarget() + Vector3(0, aa->getScale().y, 0));
						}
						proj->EmitParticles(Math::RandIntMinMax(16, 32));
						(*it)->EmitParticles(Math::RandIntMinMax(16, 32));
						CSoundEngine::GetInstance()->AddSound("barrelbreak","Sound//barrelbreak.mp3");
						CSoundEngine::GetInstance()->PlayASound("barrelbreak");
						CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + proj->getScale().x*0.5f);
						CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + proj->getScale().x*0.015f);
						switch ((*it)->getType())
						{
						case CEntity::E_ENEMY:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 5);
							ParticleManager::GetInstance()->AddParticle("+5 Points", (*it2), Color(0.1f,1,0.1f));
							break;
						case CEntity::E_MOVING_TARGET:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
							ParticleManager::GetInstance()->AddParticle("+3 Points", (*it2), Color(0.1f, 1, 0.1f));
							break;
						case CEntity::E_TARGET_ICE:
						case CEntity::E_TARGET_FIRE:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
							if ((*it)->getType() == CEntity::E_TARGET_ICE)
								ParticleManager::GetInstance()->AddParticle("+3 Points", (*it), Color(0.1f, 0.1f, 1));
							else
								ParticleManager::GetInstance()->AddParticle("+3 Points", (*it), Color(1, 0.1f, 0.1f));
							break;
						case CEntity::E_TARGET:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 1);
							ParticleManager::GetInstance()->AddParticle("+1 Points", (*it2), Color(0.1f, 1, 0.1f));
							break;
						}
						cout << "Score: " << CPlayerInfo::GetInstance()->GetScore() << endl;
					}
						break;
				default:
					if ((*it)->getType() != CEntity::E_PROJECTILE && (*it2)->getType() != CEntity::E_PLAYER)
					(*it)->setPos((*it)->getPos() - (viewVector * (*it)->getSpeed() * (float)dt)); // collision response
					break;
				}
				hasDoneCollide = true;
			}
			else
			{
				if ((*it)->getType() == CEntity::E_ENEMY || (*it)->getType() == CEntity::E_TARGET || (*it)->getType() == CEntity::E_MOVING_TARGET || (*it)->getType() == CEntity::E_TARGET_FIRE || (*it)->getType() == CEntity::E_TARGET_ICE || (*it)->getType() == CEntity::E_BOSS || (*it)->getType() == CEntity::E_PROJECTILE)
				{
					bool check = true;
					if ((*it)->getType() == CEntity::E_PROJECTILE)
					{
						CProjectile* tempProjectile = (CProjectile*)*it;
						if (tempProjectile->getSource())
						{
							if (tempProjectile->getSource()->getType() != CEntity::E_BOSS)
							{
								check = false;
							}
						}
					}

					if ((*it2)->getType() == CEntity::E_PROJECTILE && check)
					{
						CProjectile* tempProjectile = (CProjectile*)*it2;
						if (tempProjectile->getProjType() == CProjectile::PTYPE_BEAM)
						{
							if (!(CheckForLineIntersection(tempProjectile->getPos(), (*it), (tempProjectile->getTarget() - tempProjectile->getPos()).Normalized() * tempProjectile->getScale().z * 1.1, 0) - Vector3(9999, 9999, 9999)).IsZero()
								|| !(CheckForLineIntersection(tempProjectile->getPos(), (*it), (tempProjectile->getTarget() - tempProjectile->getPos()).Normalized() * tempProjectile->getScale().z * 1.1, 1) - Vector3(9999, 9999, 9999)).IsZero()
								|| !(CheckForLineIntersection(tempProjectile->getPos(), (*it), (tempProjectile->getTarget() - tempProjectile->getPos()).Normalized() * tempProjectile->getScale().z * 1.1, 2) - Vector3(9999, 9999, 9999)).IsZero())
							{
								//(*it)->setIsDone(true);
								//(*it2)->setIsDone(true);
								switch ((*it)->getType())
								{
								case CEntity::E_ENEMY:
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 5);
									(*it)->setIsDone(true);
									CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + tempProjectile->getScale().x*0.5f);
									CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + tempProjectile->getScale().x*0.015f);
									break;
								case CEntity::E_MOVING_TARGET:
								case CEntity::E_TARGET_ICE:
								case CEntity::E_TARGET_FIRE:
									(*it)->setIsDone(true);
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
									CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + tempProjectile->getScale().x*0.5f);
									CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + tempProjectile->getScale().x*0.015f);
									break;
								case CEntity::E_TARGET:
									(*it)->setIsDone(true);
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 1);
									CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + tempProjectile->getScale().x*0.5f);
									CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + tempProjectile->getScale().x*0.015f);
									break;
								case CEntity::E_BOSS:
								{
									CBoss* bos = static_cast<CBoss*>(*it);
									bos->TakeDamage(tempProjectile);
									tempProjectile->setIsDone(true);
									CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + tempProjectile->getScale().x*0.5f);
									CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + tempProjectile->getScale().x*0.015f);
								}
									break;
								case CEntity::E_PROJECTILE:
								{
									CProjectile* bossProj = (CProjectile*)*it;
									bossProj->bossDone = true;
									(*it)->setIsDone(true);
									CPlayerInfo::GetInstance()->setScreenShakeIntensity(2.f + tempProjectile->getScale().x*0.5f);
									CPlayerInfo::GetInstance()->setScreenShakeTime(0.075f + tempProjectile->getScale().x*0.015f);
								}
									break;
								default:
									break;
								}
								//cout << "Score: " << CPlayerInfo::GetInstance()->GetScore() << endl;
								break;
							}
						}
					}
				}
			}
		}
	}
	return hasDoneCollide;
}

//DOCUMENTATION
//This collision helps to check for an origin line intersecting a cuboid, where in this case the cuboid is the AABB bounding box
//This function is carried out three times with each axis respectively, which is what the parameter 'times' represents
//When (times == 2), it will be handled by a different but similar function as it does not lie in the 2D X-Y plane
//Each time, it will check 2 faces with the origin line.
//The 2 faces are represented by 2 lines separately in a cartesian plane graph, along with the origin line.
//The pivot is the starting point of the origin line, and will be the center of the graph. (0, 0, 0)
//Thus, this function depends on relative position.
//The function checks if the origin line intersects any of those 2 line graphs (y = ax + c), returning the relative point of intersection
//If it intersects both line graphs, the nearest point will be obtained
//Points that are not within the origin line are ignored.
//If there is no final valid intersection point, Vector3(9999, 9999, 9999) is returned as an invalid point
Vector3 EntityManager::CheckForLineIntersection(Vector3 pivot, CEntity *ent, Vector3 mousePoint, int times)
{
	if (times == 2) return CheckForLineIntersectionZFace(pivot, ent, mousePoint);

	Vector3 tempScale = ent->getMaxAABB() - ent->getMinAABB();

	Vector3 relativePos = ent->getPos() - pivot;
	float tempAng = Math::DegreeToRadian(90);
	float grad0, grad1, con0, con1;
	Vector3 NP(cosf(tempAng) - sinf(tempAng), sinf(tempAng) + cosf(tempAng));

	if (abs(NP.x) <= Math::EPSILON) NP.x = 0;
	if (abs(NP.y) <= Math::EPSILON) NP.y = 0;
	Vector3 p0, p1, p2, p3;
	//first line graph is p0 to p1, second linegraph is p2 to p3
	if (times == 0)
	{
		p0 = relativePos + Vector3(NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p1 = relativePos + Vector3(NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;

		p2 = relativePos + Vector3(-NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p3 = relativePos + Vector3(-NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
	}
	else if (times == 1)
	{
		p0 = relativePos + Vector3(NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p1 = relativePos + Vector3(-NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;

		p2 = relativePos + Vector3(NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
		p3 = relativePos + Vector3(-NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
	}

	//Gradient cannot be obtained because you cannot divide by zero where (p1.x - p0.x) = 0, where (p3.x - p2.x) would also be 0
	if (abs(p1.x - p0.x) < Math::EPSILON) /////////////////
	{
		float excepY0, excepY1;
		excepY0 = p0.x * (mousePoint.y / mousePoint.x);
		excepY1 = p2.x * (mousePoint.y / mousePoint.x);

		Vector3 tempNormalizedMV = mousePoint.Normalized();
		float z0 = tempNormalizedMV.z * (p0.x / tempNormalizedMV.x);
		float z1 = tempNormalizedMV.z * (p2.x / tempNormalizedMV.x);

		if ((((excepY0 < p0.y || excepY0 > p1.y) && p1.y > p0.y) || ((excepY0 < p1.y || excepY0 > p0.y) && p0.y >= p1.y)) && (((excepY1 < p0.y || excepY1 > p1.y) && p1.y > p0.y) || ((excepY1 < p1.y || excepY1 > p0.y) && p0.y >= p1.y)))
		{
			return Vector3(9999, 9999, 9999);
		}
		else if (((excepY0 < p0.y || excepY0 > p1.y) && p1.y > p0.y) || ((excepY0 < p1.y || excepY0 > p0.y) && p0.y >= p1.y))
		{
			if (Vector3(p2.x, excepY1, z1).Length() <= mousePoint.Length() && (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z))
			{
				return Vector3(p2.x, excepY1, z1);
			}
			else return Vector3(9999, 9999, 9999);
		}
		else if (((excepY1 < p0.y || excepY1 > p1.y) && p1.y > p0.y) || ((excepY1 < p1.y || excepY1 > p0.y) && p0.y >= p1.y))
		{
			if (Vector3(p0.x, excepY0, z0).Length() <= mousePoint.Length() && (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z))
			{
				return Vector3(p0.x, excepY0, z0);
			}
			else return Vector3(9999, 9999, 9999);
		}
		else
		{
			if (Vector3(p0.x, excepY0, z0).Length() > mousePoint.Length() && Vector3(p2.x, excepY1, z1).Length() > mousePoint.Length())
			{
				return Vector3(9999, 9999, 9999);
			}
			if (Vector3(p0.x, excepY0, z0).Length() > mousePoint.Length())
			{
				if (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z) return Vector3(p2.x, excepY1, z1);
				else return Vector3(9999, 9999, 9999);
			}
			if (Vector3(p2.x, excepY1, z1).Length() > mousePoint.Length() && (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z))
			{
				if (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z) return Vector3(p0.x, excepY0, z0);
				else return Vector3(9999, 9999, 9999);
			}
			if (Vector3(p0.x, excepY0, z0).Length() > Vector3(p2.x, excepY1, z1).Length() && (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z))
			{
				if (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z) return Vector3(p2.x, excepY1, z1);
				else return Vector3(9999, 9999, 9999);
			}
			else if (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z) return Vector3(p0.x, excepY0, z0);
			return Vector3(9999, 9999, 9999);
		}

	}/////////////////////
	else
	{
		if (p1.x > p0.x) grad0 = (p1.y - p0.y) / (p1.x - p0.x);
		else grad0 = (p0.y - p1.y) / (p0.x - p1.x);
		con0 = p0.y - grad0 * p0.x;

		if (p3.x > p2.x) grad1 = (p3.y - p2.y) / (p3.x - p2.x);
		else grad1 = (p2.y - p3.y) / (p2.x - p3.x);
		con1 = p2.y - grad1 * p2.x;
	}
	//With the equations of the 2 line graphs obtained (grad = a, con = c, in 'y = ax + c'), now we can check for intersection

	float x0, y0, x1, y1, mGrad, z0, z1;
	mGrad = mousePoint.y / mousePoint.x;
	x0 = con0 / (mGrad - grad0);
	x1 = con1 / (mGrad - grad1);
	y0 = x0 * mGrad;
	y1 = x1 * mGrad;

	Vector3 tempNormalizedMV = mousePoint.Normalized();
	z0 = tempNormalizedMV.z * (x0 / tempNormalizedMV.x);
	z1 = tempNormalizedMV.z * (x1 / tempNormalizedMV.x);

	if ((((x0 < p0.x || x0 > p1.x) && p1.x > p0.x) || ((x0 < p1.x || x0 > p0.x) && p0.x >= p1.x)) && (((x1 < p2.x || x1 > p3.x) && p3.x > p2.x) || ((x1 < p3.x || x1 > p2.x) && p2.x >= p3.x)))
	{
		return Vector3(9999, 9999, 9999);
	}
	else if (((x0 < p0.x || x0 > p1.x) && p1.x > p0.x) || ((x0 < p1.x || x0 > p0.x) && p0.x >= p1.x))
	{
		if (Vector3(x1, y1, z1).Length() <= mousePoint.Length() && (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z))
		{
			return Vector3(x1, y1, z1);
		}
		else return Vector3(9999, 9999, 9999);
	}
	else if (((x1 < p2.x || x1 > p3.x) && p3.x > p2.x) || ((x1 < p3.x || x1 > p2.x) && p2.x >= p3.x))
	{
		if (Vector3(x0, y0, z0).Length() <= mousePoint.Length() && (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z))
		{
			return Vector3(x0, y0, z0);
		}
		else return Vector3(9999, 9999, 9999);
	}
	else //needs changes for z
	{
		if (Vector3(x0, y0, z0).Length() > mousePoint.Length() && Vector3(x1, y1, z1).Length() > mousePoint.Length())
		{
			return Vector3(9999, 9999, 9999);
		}
		if (Vector3(x0, y0, z0).Length() > mousePoint.Length())
		{
			if (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z) return Vector3(x1, y1, z1);
			else return Vector3(9999, 9999, 9999);
		}
		if (Vector3(x1, y1, z1).Length() > mousePoint.Length())
		{
			if (z0 >= ent->getMinAABB().z - pivot.z  && z0 <= ent->getMaxAABB().z - pivot.z) return Vector3(x0, y0, z0);
			else return Vector3(9999, 9999, 9999);
		}
		if (Vector3(x0, y0, z0).Length() > Vector3(x1, y1, z1).Length())
		{
			if (z1 >= ent->getMinAABB().z - pivot.z && z1 <= ent->getMaxAABB().z - pivot.z) return Vector3(x1, y1, z1);
			else return Vector3(9999, 9999, 9999);
		}
		else if (z0 >= ent->getMinAABB().z - pivot.z && z0 <= ent->getMaxAABB().z - pivot.z) return Vector3(x0, y0, z0);

		return Vector3(9999, 9999, 9999);
	}

	return Vector3(9999, 9999, 9999);
}

Vector3 EntityManager::CheckForLineIntersectionZFace(Vector3 pivot, CEntity *ent, Vector3 mousePoint)
{
	Vector3 tempScale = ent->getMaxAABB() - ent->getMinAABB();
	//tempScale *= 0.5;

	Vector3 relativePos = ent->getPos() - pivot;

	Vector3 tempDir = mousePoint.Normalized();

	Vector3 point0, point1, pointOutput(9999, 9999, 9999);
	point0 = tempDir * ((ent->getMinAABB().z - pivot.z) / tempDir.z);
	point1 = tempDir * ((ent->getMaxAABB().z - pivot.z) / tempDir.z);

	if (point0.x < ent->getMaxAABB().x - pivot.x && point0.x > ent->getMinAABB().x - pivot.x && point0.y < ent->getMaxAABB().y - pivot.y && point0.y > ent->getMinAABB().y - pivot.y)
	{
		if (point0.Length() <= mousePoint.Length()) pointOutput = point0;
	}
	if (point1.x < ent->getMaxAABB().x - pivot.x && point1.x > ent->getMinAABB().x - pivot.x && point1.y < ent->getMaxAABB().y - pivot.y && point1.y > ent->getMinAABB().y - pivot.y)
	{
		if (point1.Length() <= mousePoint.Length() && point1.Length() <= pointOutput.Length()) pointOutput = point1;
	}

	if (pointOutput.Dot(mousePoint) < 0) return Vector3(9999, 9999, 9999);

	return pointOutput;
}

int EntityManager::AmountOfEnemies()
{
	int toret = 0;
	std::list<CEntity*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if (!(*it)->isDone() && (*it)->getType() == CEntity::E_ENEMY)
			toret++;
	}
	return toret;
}

void EntityManager::KillAllEnemies()
{
	std::list<CEntity*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->getType() == CEntity::E_ENEMY)
			(*it)->setIsDone(true);
	}
}
