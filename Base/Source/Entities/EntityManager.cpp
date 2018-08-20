#include "EntityManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Enemy.h"
#include "Projectile.h"
#include <iostream>
#include "../SoundEngine.h"
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
	if (CheckForCollision(_dt)) { cout << "Collision Detected" << endl; }
	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->isDone() && (*it)->getType() != CEntity::E_TARGET && (*it)->getType() != CEntity::E_MOVING_TARGET)
		{
			// Delete if done
			delete *it;
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

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(CEntity *ThisEntity, CEntity *ThatEntity)
{
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(CEntity *ThisEntity, CEntity *ThatEntity)
{
	if (!ThisEntity->hasCollider() || !ThatEntity->hasCollider() || ThisEntity == ThatEntity || ThisEntity->isDone() || ThatEntity->isDone()) { return false; }
	return CheckOverlap(ThisEntity->getMinAABB(), ThisEntity->getMaxAABB(), ThatEntity->getMinAABB(), ThatEntity->getMaxAABB());
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(float dt)
{
	// Update all entities
	std::list<CEntity*>::iterator it, it2, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->getType() == CEntity::E_PLAYER)
			continue;
		Vector3 viewVector = ((*it)->getTarget() - (*it)->getPos()).Normalized();
		for (it2 = entityList.begin(); it2 != end; ++it2)
		{
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
			default:
				break;
			}
			}						//pre-collision check updates <<<<<<<<<<<<END>>>>>>>>>>>>
			if (CheckAABBCollision(*it, *it2) || CheckSphereCollision(*it, *it2)) 
			{ 
				switch ((*it)->getType())
				{
				case CEntity::E_ENEMY:
				case CEntity::E_TARGET:
				case CEntity::E_MOVING_TARGET:
					if ((*it2)->getType() == CEntity::E_PROJECTILE)
					{
						(*it)->setIsDone(true);
						(*it2)->setIsDone(true);
						CProjectile* proj = static_cast<CProjectile*>((*it2));
						proj->EmitParticles(Math::RandIntMinMax(16, 32));
						CSoundEngine::GetInstance()->AddSound("Death", "Sound//roblox.mp3");
						CSoundEngine::GetInstance()->PlayASound("Death");
						CSoundEngine::GetInstance()->PlayASound("floorImpact");

						switch ((*it)->getType())
						{
						case CEntity::E_ENEMY:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 5);
							break;
						case CEntity::E_MOVING_TARGET:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
							break;
						case CEntity::E_TARGET:
							CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 1);
							break;
						}
						cout << "Score: " << CPlayerInfo::GetInstance()->GetScore() << endl;
						break;
					}
				default:
					(*it)->setPos((*it)->getPos() - (viewVector * (*it)->getSpeed() * (float)dt)); // collision response
					break;
				}
				return true; 
			}
			else
			{
				if ((*it)->getType() == CEntity::E_ENEMY || (*it)->getType() == CEntity::E_TARGET || (*it)->getType() == CEntity::E_MOVING_TARGET)
					if ((*it2)->getType() == CEntity::E_PROJECTILE)
					{
						CProjectile* tempProjectile = (CProjectile*)*it2;
						if (tempProjectile->getProjType() == CProjectile::PTYPE_BEAM)
						{
							if(!(CheckForLineIntersection(tempProjectile->getPos(), (*it), (tempProjectile->getTarget() - tempProjectile->getPos()).Normalized() * tempProjectile->getScale().z * 1.03, 0) - Vector3(9999, 9999, 9999)).IsZero() 
								|| !(CheckForLineIntersection(tempProjectile->getPos(), (*it), (tempProjectile->getTarget() - tempProjectile->getPos()).Normalized() * tempProjectile->getScale().z * 1.03, 1) - Vector3(9999, 9999, 9999)).IsZero())
							{
								(*it)->setIsDone(true);
								//(*it2)->setIsDone(true);
								switch ((*it)->getType())
								{
								case CEntity::E_ENEMY:
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 5);
									break;
								case CEntity::E_MOVING_TARGET:
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 3);
									break;
								case CEntity::E_TARGET:
									CPlayerInfo::GetInstance()->SetScore(CPlayerInfo::GetInstance()->GetScore() + 1);
									break;
								}
								cout << "Score: " << CPlayerInfo::GetInstance()->GetScore() << endl;
								break;
							}
						}
					}
			}
		}
	}
	return false;
}

Vector3 EntityManager::CheckForLineIntersection(Vector3 pivot, CEntity *ent, Vector3 mousePoint, bool times)
{
	Vector3 tempScale = ent->getMaxAABB() - ent->getMinAABB();
	tempScale *= 0.5;

	Vector3 relativePos = ent->getPos() - pivot;
	float tempAng = Math::DegreeToRadian(90);
	float grad0, grad1, con0, con1;
	Vector3 NP(cosf(tempAng) /** go2->normal.x*/ - sinf(tempAng) /** go2->normal.y*/, sinf(tempAng) /** go2->normal.x*/ + cosf(tempAng) /** go2->normal.y*/);

	if (abs(NP.x) <= Math::EPSILON) NP.x = 0;
	if (abs(NP.y) <= Math::EPSILON) NP.y = 0;
	Vector3 p0, p1, p2, p3;
	if (times == false)
	{
		p0 = relativePos + Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p1 = relativePos + Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;

		p2 = relativePos - Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(-NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p3 = relativePos - Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(-NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
	}
	else
	{
		p0 = relativePos + Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;
		p1 = relativePos - Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(-NP.x * tempScale.x, NP.y * tempScale.y) * 0.5;

		p2 = relativePos + Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
		p3 = relativePos - Vector3(0, 0, 0) * tempScale.x * 0.5 + Vector3(-NP.x * tempScale.x, -NP.y * tempScale.y) * 0.5;
	}

	//std::cout << p0 << p1 << p2 << p3 << relativePos << mousePoint << std::endl << std::endl;

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
	//time to check!!

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