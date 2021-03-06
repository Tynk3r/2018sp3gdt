#ifndef ENTITY_H
#define ENTITY_H
#include "Vector3.h"
#include "EntityManager.h"

class CEntity
{
public:
	enum TYPE {
		E_ENEMY = 0,
		E_TARGET,
		E_MOVING_TARGET,
		E_TARGET_FIRE,
		E_TARGET_ICE,
		E_PROJECTILE,
		E_PLAYER,
		E_DRONE,
		E_PARTICLE,
		E_WALL,
		E_NPC,
		E_BOSS,
		E_PILLAR,
		E_ROCKS,
		E_TOTAL
	};

	CEntity();
	virtual ~CEntity();

	void Init();
	virtual void Update(double dt);

	Vector3 getPos(void) { return position; }
	void setPos(Vector3 p) { position = p; }
	Vector3 getOriginPos(void) { return originPosition; }
	void setOriginPos(Vector3 p) { originPosition = p; }
	Vector3 getOriginTarget(void) { return originTarget; }
	void setOriginTarget(Vector3 t) { originTarget = t; }
	Vector3 getScale(void) { return scale; }
	void setScale(Vector3 s) { scale = s; }
	TYPE getType(void) { return type; }
	void setType(TYPE t) { type = t; }
	Vector3 getTarget(void) { return target; }
	void setTarget(Vector3 t) { target = t; }
	float getSpeed(void) { return speed; }
	void setSpeed(float s) { speed = s; }

	// Set the maxAABB and minAABB
	virtual void setAABB(Vector3 maxAABB, Vector3 minAABB);
	// Set the maxAABB
	virtual void setMaxAABB(Vector3 maxAABB);
	// Get the maxAABB
	virtual Vector3 getMaxAABB(void);
	// Set the minAABB
	virtual void setMinAABB(Vector3 minAABB);
	// Get the minAABB
	virtual Vector3 getMinAABB(void);

	virtual bool isDone(void) { return done; }
	void setIsDone(const bool _value);

	virtual void setGrav(Vector3 _grav) { gravVel = _grav; }
	virtual Vector3 getGrav() const { return gravVel; }

	virtual void setCanBeWalled(bool _canbewalled) { canbeWalled = _canbewalled; }

	// Check if this entity has a collider class parent
	virtual bool hasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void setCollider(const bool _value);
	virtual void EmitParticles(int amt);
private:
	Vector3 position;
	Vector3 scale;
	Vector3 target;
	float speed;
	TYPE type;
	bool done = false;
	bool canbeWalled = false;

	// Vector3 variables to store coordinates of the AABB bounding box of an object
	Vector3 maxAABB;
	Vector3 minAABB;
	Vector3 gravVel;
	bool m_bCollider;
protected:
	Vector3 originPosition;
	Vector3 originTarget;
};

#endif