#pragma once
#include "../Entities/Entity.h"
#include "Vector3.h"
#include "../Camera3.h"
#include "MouseController.h"
#include "../LoadHmap.h"

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CPlayerInfo : public CEntity
{
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

	// The postures of the FPS player/camera
	enum CURRENT_POSTURE
	{
		STAND = 0,
		CROUCH,
		PRONE,
		NUM_POSTURE,
	};
	CURRENT_POSTURE theCurrentPosture;

public:
	enum PLR_ANIM_STATE
	{
		PLR_ANIM_IDLE,
		PLR_ANIM_LEFTARM_CASTHOLDING,
		PLR_ANIM_LEFTARM_CASTING,
		PLR_ANIM_LEFTARM_CASTED,
		PLR_ANIM_RIGHTARM_CASTHOLDING,
		PLR_ANIM_RIGHTARM_CASTING,
		PLR_ANIM_RIGHTARM_CASTED,

		PLR_ANIM_TOTAL,
	};
	enum SPELL_TYPE
	{
		SPELL_NONE,
		SPELL_FIREBALL,
		SPELL_ICEBALL,
	};
	enum SPELLMOD_TYPE {
		SMTYPE_NORMAL,
		SMTYPE_BURST,
		SMTYPE_SPECIAL,
		SMTYPE_TOTAL
	};
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance && s_instance != NULL)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

	// Initialise this class instance
	void Init(void);
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's vertical movement
	void StopVerticalMovement(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set m_dJumpSpeed of the player
	void SetJumpSpeed(const double m_dJumpSpeed);
	// Set m_dJumpAcceleration of the player
	void SetJumpAcceleration(const double m_dJumpAcceleration);
	// Set Fall Speed of the player
	void SetFallSpeed(const double m_dFallSpeed);
	// Set Fall Acceleration of the player
	void SetFallAcceleration(const double m_dFallAcceleration);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Get Up
	Vector3 GetUp(void) const;
	// Get Jump Speed of the player
	double GetJumpSpeed(void) const;
	// Get Jump Acceleration of the player
	double GetJumpAcceleration(void) const;
	// Get Fall Speed of the player
	double GetFallSpeed(void) const;
	// Get Fall Acceleration of the player
	double GetFallAcceleration(void) const;

	void SetAnimState(PLR_ANIM_STATE state);
	void SetSpellType(SPELL_TYPE spelltype);
	void SetCurrentNPC(CEntity* npc = NULL);

	Vector3 GetScreenshake(void) const;
	Vector3 GetCameraSway(void) const;
	Vector3 GetLeftArmOffset() const;
	Vector3 GetRightArmOffset() const;
	Vector3 GetLeftArmRotation() const;
	Vector3 GetRightArmRotation() const;
	PLR_ANIM_STATE GetAnimState() const;
	SPELL_TYPE GetSpellType() const;
	CEntity* GetCurrentNPC() const;

	// Update Jump Upwards
	void UpdateJumpUpwards(double dt = 0.0333f);
	// Update FreeFall
	void UpdateFreeFall(double dt = 0.0333f);
	// Update
	void Update(double dt = 0.0333f);

	void SetNotMoving()
	{
		hasMoved = false;
		hasRan = false;
	}

	// Hardware Abstraction methods
	// Detect and process front / back movement on the controller
	bool Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process left / right movement on the controller
	bool Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process look up / down on the controller
	bool Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process look left / right on the controller
	bool Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);

	//Rocket movement
	bool Rocket_Yaw(const float deltaTime, const bool direction, const float speedMultiplier); //left and right
	bool Rocket_Pitch(const float deltaTime, const bool direction, const float speedMultiplier); //up and down
	bool Rocket_Roll(const float deltaTime, const bool direction, const float speedMultiplier); //roll
	float rocketYawAccel = 0, rocketPitchAccel = 0, rocketRollAccel = 0;

	// Stop sway
	bool StopSway(const float deltaTime);

	// Constrain the position within the borders
	void Constrain(double dt);

	// Handling Camera
	void AttachCamera(Camera3* _cameraPtr);
	void DetachCamera();

	int GetHealth(void) const;
	void setHealth(int h) { m_dHealth = h; }
	float getMana(void) const { return m_dMana; }
	void setMana(float m) { m_dMana = m; m_dManaRegen = 1; }
	int GetScore(void) const;
	void SetScore(int s) { m_dScore = s; }
	void SetSlowTime(bool slowtime) { if (slowtime && this->timeslowDuration == 0) { this->doSlowTime = true; this->speedingUpTime = false; } /*else { this->doSlowTime = true; this->speedingUpTime = true; }*/ }

	SPELLMOD_TYPE GetSpellMod() { return spellMod; }
	void SetSpellMod(SPELLMOD_TYPE spellMod) { this->spellMod = spellMod; }

	float terrainHeight = 0.f;

	float camBobRotate, camBobHeight;
	double camBobTime, camBobMaxTimeWalk, camBobMaxTimeRun;
	bool screenShakeOn = false;
	bool rocketMode = false;

	Vector3 rocketPosition, rocketUp, rocketRight, rocketTarget;
	bool FPSMode = false;
	float FirstHeight = 0;

	void setSpellModLimit(SPELLMOD_TYPE smtype) { spellModLimit = smtype; }

	float getManaCost()
	{
		if (spellModLimit != SMTYPE_TOTAL)
		{
			switch (spellModLimit)
			{
			case SMTYPE_NORMAL: return 10;
				break;
			case SMTYPE_BURST: return 25;
				break;
			case SMTYPE_SPECIAL: return 50;
				break;
			default: return 10;
				break;
			}
		}

		switch (spellMod)
		{
		case SMTYPE_NORMAL: return 10;
			break;
		case SMTYPE_BURST: return 25;
			break;
		case SMTYPE_SPECIAL: return 50;
			break;
		default: return 10;
			break;
		}
		return 10;
	}
	std::vector<unsigned char> getHeightMap() { return attachedCamera->m_heightMap; };
	void setScreenShakeTime(float time);
	void setScreenShakeIntensity(float amt) { this->screenShakeIntensity = amt; }
private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 up;
	Vector3 maxBoundary, minBoundary;

	Vector3 screenshakeOffset;
	Vector3 cameraSway;

	bool hasMoved, hasRan;

	SPELLMOD_TYPE spellMod;
	SPELLMOD_TYPE spellModLimit = SMTYPE_TOTAL;
	int m_dHealth;
	float m_dMana;
	float m_dManaRegen;
	int m_dScore;
	double m_dSpeed;
	double m_dAcceleration;

	bool m_bJumpUpwards;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dFallSpeed;
	bool m_bFallDownwards;
	double m_dFallAcceleration;

	double m_dElapsedTime;
	Camera3* attachedCamera;

	Vector3 leftArmOffset, rightArmOffset;
	Vector3 leftArmRotation, rightArmRotation;//half bugged but it works well enough for basic anims
	PLR_ANIM_STATE currentAnimState;
	float animFrame;
	SPELL_TYPE spelltype;
	CEntity* currentNPC;//the npc that the player is currently talkin to
	
	float screenShakeIntensity;
	float screenShakeDuration;
	float screenShakeElapsedTime;

	float timeslowDuration;
	bool doSlowTime;
	bool speedingUpTime;
public:
	// Camera Sway
	float m_fCameraSwayAngle;
	float m_fCameraSwayDeltaAngle;
	float m_fCameraSwayAngle_LeftLimit, m_fCameraSwayAngle_RightLimit;	//	The limits for left and right sway
	bool m_bCameraSwayDirection;	// false = left, true = right
	int climbHeight;
};
