#include "Entity.h"

CEntity::CEntity():
	position(Vector3(0.f,0.f,0.f)),
	scale(Vector3(1.f, 1.f, 1.f)),
	type(E_ENEMY)
{
}


CEntity::~CEntity()
{
}
