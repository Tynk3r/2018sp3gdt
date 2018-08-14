#pragma once
#include "../Entities/Entity.h"
#include "Vector3.h"
#include "../Camera3.h"

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
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
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

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
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

	// Update Jump Upwards
	void UpdateJumpUpwards(double dt = 0.0333f);
	// Update FreeFall
	void UpdateFreeFall(double dt = 0.0333f);
	// Update
	void Update(double dt = 0.0333f);

	// Hardware Abstraction methods
	// Detect and process front / back movement on the controller
	bool Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process left / right movement on the controller
	bool Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process look up / down on the controller
	bool Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Detect and process look left / right on the controller
	bool Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier = 1.0f);
	// Stop sway
	bool StopSway(const float deltaTime);

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	void AttachCamera(Camera3* _cameraPtr);
	void DetachCamera();

	int GetHealth(void) const;
	int GetScore(void) const;

	float terrainHeight = 0.f;

private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
	Vector3 maxBoundary, minBoundary;

	int m_dHealth;
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

public:
	// Camera Sway
	float m_fCameraSwayAngle;
	float m_fCameraSwayDeltaAngle;
	float m_fCameraSwayAngle_LeftLimit, m_fCameraSwayAngle_RightLimit;	//	The limits for left and right sway
	bool m_bCameraSwayDirection;	// false = left, true = right
};