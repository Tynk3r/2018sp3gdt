#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "Entity.h"
//#include "../Particles/ParticleManager.h"

class CEntity;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);

	void AddEntity(CEntity* _newEntity);
	bool RemoveEntity(CEntity* _existingEntity);
	std::list<CEntity*> entityList;

	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(CEntity *ThisEntity, CEntity *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(CEntity *ThisEntity, CEntity *ThatEntity);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(float dt);
	//Check for line intersection across an AABB box, all boxes are assumed to be non-rotated (wall collision is 2 sided so must be done twice)
	Vector3 CheckForLineIntersection(Vector3 pivot, CEntity *ent, Vector3 mousePoint, int times);
	Vector3 CheckForLineIntersectionZFace(Vector3 pivot, CEntity *ent, Vector3 mousePoint);
	int AmountOfEnemies();
	void KillAllEnemies();
};

#endif // ENTITY_MANAGER_H