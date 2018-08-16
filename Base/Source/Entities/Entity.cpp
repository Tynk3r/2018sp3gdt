#include "Entity.h"

CEntity::CEntity():
	position(Vector3(0.f,0.f,0.f)),
	scale(Vector3(1.f, 1.f, 1.f)),
	target(Vector3(0.f, 0.f, 0.f)),
	speed(100.f),
	type(E_TOTAL),
	done(false),
	maxAABB(Vector3(0, 0, 0)), 
	minAABB(Vector3(0, 0, 0)),
	m_bCollider(false)
{
}


CEntity::~CEntity()
{
}

void CEntity::Init()
{
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
}

void CEntity::Update(double dt) 
{
	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	switch (type) 
	{
	case E_ENEMY:
		if ((getTarget() - getPos()).LengthSquared() < 1.f) 
		{ 
			viewVector.SetZero(); 
		}
		setPos(getPos() + (viewVector * getSpeed() * (float)dt));
		break;
	case E_PROJECTILE:
	case E_DRONE:
	case E_MOVING_TARGET:
		setPos(getPos() + (viewVector * getSpeed() * (float)dt));
		setTarget(getPos() + viewVector);
		break;
	case E_PLAYER:
	case E_TARGET:
		break;
	default:
		break;
	}
	setAABB(Vector3(position.x + scale.x, position.y + scale.y, position.z + scale.z), Vector3(position.x - scale.x, position.y - scale.y, position.z - scale.z));
}

// Set the maxAABB and minAABB
void CEntity::setAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}
// Set the maxAABB
void CEntity::setMaxAABB(Vector3 maxAABB)
{
	this->maxAABB = maxAABB;
}
// Get the maxAABB
Vector3 CEntity::getMaxAABB(void)
{
	return maxAABB;
}
// Set the minAABB
void CEntity::setMinAABB(Vector3 minAABB)
{
	this->minAABB = minAABB;
}
// Get the minAABB
Vector3 CEntity::getMinAABB(void)
{
	return minAABB;
}

void CEntity::setIsDone(bool _value)
{
	done = _value;
}

// Check if this entity has a collider class parent
bool CEntity::hasCollider(void) const
{
	return m_bCollider;
}

// Set the flag to indicate if this entity has a collider class parent
void CEntity::setCollider(const bool _value)
{
	m_bCollider = _value;
}