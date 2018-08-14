#pragma once
#include "Vector3.h"
#include "EntityManager.h"

class CEntity
{
public:
	enum TYPE {
		E_ENEMY = 0,
		E_MOVING_TARGET,
		E_PROJECTILE,
		E_TOTAL
	};

	CEntity();
	virtual ~CEntity() = 0;

	virtual void Init() {};
	virtual void Update(double dt) {};

	Vector3 getPos(void) { return position; }
	void setPos(Vector3 p) { position = p; }
	Vector3 getScale(void) { return scale; }
	void setScale(Vector3 s) { scale = s; }

	bool IsDone(void) { return false; }

private:
	Vector3 position;
	Vector3 scale;
	
	TYPE type;
};

