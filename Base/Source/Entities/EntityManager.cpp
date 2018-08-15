#include "EntityManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Enemy.h"
#include <iostream>
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
		if ((*it)->isDone())
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
	}
	
	return false;*/
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
	if (!ThisEntity->hasCollider() || !ThatEntity->hasCollider() || ThisEntity == ThatEntity) { return false; }
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
		if ((*it)->getType() == CEntity::E_PLAYER || ((*it)->getTarget() - (*it)->getPos()).Length() <= 5)
			continue;
		Vector3 viewVector = ((*it)->getTarget() - (*it)->getPos()).Normalized();
		(*it)->setPos((*it)->getPos() + (viewVector * (*it)->getSpeed() * (float)dt));
		for (it2 = entityList.begin(); it2 != end; ++it2)
		{
			if (CheckAABBCollision(*it, *it2) || CheckSphereCollision(*it, *it2)) 
			{ 
				//CPlayerInfo* plr = dynamic_cast<CPlayerInfo*>(*it) ? dynamic_cast<CPlayerInfo*>(*it) : dynamic_cast<CPlayerInfo*>(*it2);
				//CEnemy* enmy = dynamic_cast<CEnemy*>(*it) ? dynamic_cast<CEnemy*>(*it) : dynamic_cast<CEnemy*>(*it2);
				//plr->Move_FrontBack(dt, false, 1.f);
				//enmy->tempMoveBack(dt);
				(*it)->setPos((*it)->getPos() - (viewVector * (*it)->getSpeed() * (float)dt));
				return true; 
			}
		}
	}
	return false;
}
