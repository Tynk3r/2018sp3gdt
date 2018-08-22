#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../Particles/ParticleManager.h"
// a projectile class
class CProjectile : public CEntity
{
public:
	enum PROJECTILE_TYPE {
		PTYPE_FIRE,
		PTYPE_ICE,
		PTYPE_BEAM,
		PTYPE_SPECIAL_KILLERNADO,
		PTYPE_TOTAL
	};
	enum SPELLMOD_TYPE {
		SMTYPE_NORMAL,
		SMTYPE_BURST,
		SMTYPE_SPECIAL,
		SMTYPE_TOTAL
	};

	CProjectile(PROJECTILE_TYPE projectileType = PTYPE_FIRE, SPELLMOD_TYPE spellModType = SMTYPE_NORMAL);
	~CProjectile();

	void Init(Vector3 pos, Vector3 targ, CEntity* source = NULL);
	void Update(double dt);
	PROJECTILE_TYPE getProjType();
	SPELLMOD_TYPE getSpellModType() { return spellModType; }
	float getElapsedTime();
	float getProjRot();
	void SetLifespanTime(double time) { lifespanTime = time; }
	void SetBurstPivRotOff(float _rot) { burstPivotRotOffset = _rot; }
	Vector3 GetBurstPivRot() { return burstPivotRot; }
	void EmitParticles(int amt = 1);
	CEntity* getSource();
private:
	PROJECTILE_TYPE projectileType;
	SPELLMOD_TYPE spellModType;
	Vector3 viewVector;
	Vector3 originDir;
	double lifespanTime;
	float elapsedTime;
	float particleRate;
	float projRot;//just a float for all-axis rotatin i guess???
	CEntity* source;
	float burstPivotRotOffset;
	Vector3 burstPivotRot;
};

