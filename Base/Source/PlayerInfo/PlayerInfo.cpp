#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: CEntity()
	, m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(30.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, theCurrentPosture(STAND)
	, m_fCameraSwayAngle(0.0f)
	, m_fCameraSwayDeltaAngle(0.1f)
	, m_fCameraSwayAngle_LeftLimit(-0.3f)
	, m_fCameraSwayAngle_RightLimit(0.3f)
	, m_bCameraSwayDirection(false)
	, m_dHealth(5)
	, m_dScore(0)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	m_dHealth = 100;
	m_dScore = 0;

	// Set the current values
	position.Set(400, 0, 0);
	target.Set(0, 0, 50);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	m_fCameraSwayAngle = 0.0f;
	m_fCameraSwayDeltaAngle = 0.3f;
	m_fCameraSwayAngle_LeftLimit = -3.0f;
	m_fCameraSwayAngle_RightLimit = 3.0f;
	m_bCameraSwayDirection = false;
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 10.0;
	}
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dJumpSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t; v is m_dJumpSpeed AFTER updating using SUVAT where u is 
	// the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * m_dElapsedTime;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
		m_dElapsedTime = 0.0;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update the jump's elapsed time
	m_dElapsedTime += dt;

	// Update position and target y values.
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						  0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	target.y += (float)(m_dFallSpeed * m_dElapsedTime + 
						0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * m_dElapsedTime;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < 0.f)
	{
		Vector3 viewDirection = target - position;
		position.y = 0.f;
		target = position + viewDirection;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
		m_dElapsedTime = 0.0;
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		theCurrentPosture = (CURRENT_POSTURE)(theCurrentPosture + 1);
		if (theCurrentPosture == NUM_POSTURE)
			theCurrentPosture = STAND;
		Vector3 viewDirection = target - position;
		switch (theCurrentPosture)
		{
		case STAND:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			position.y += 8.0f;
			target = position + viewDirection;
			m_dSpeed = 40.0f;
			break;
		case CROUCH:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			position.y -= 3.0f;
			target = position + viewDirection;
			m_dSpeed = 30.0f;
			break;
		case PRONE:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			position.y -= 5.0f;
			target = position + viewDirection;
			m_dSpeed = 10.0f;
			break;
		default:
			break;
		}
	}

	// Constrain the position
	Constrain();
	UpdateJumpUpwards(dt);
	UpdateFreeFall(dt);
		
	// Do camera sway
	if (m_fCameraSwayAngle != 0.0f)
	{
		Mtx44 rotation;
		if (m_bCameraSwayDirection == false)
			rotation.SetToRotation(-m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		else if (m_bCameraSwayDirection == true)
			rotation.SetToRotation(m_fCameraSwayDeltaAngle, 0.0f, 0.0f, 1.0f);
		up = rotation * up;
	}

	// Update minimap rotation angle
	Vector3 viewUV = (target - position).Normalized();/*
	CMinimap::GetInstance()->SetAngle(atan2(viewUV.z, viewUV.x) * 57.2883513685549146);*/

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->position = position + Vector3(0.f, terrainHeight + 100, 0.f);
		attachedCamera->target = target + Vector3(0.f, terrainHeight + 100, 0.f);
		attachedCamera->up = up.Normalized();
	}
}

// Detect and process front / back movement on the controller
bool CPlayerInfo::Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier)
{
	// Add camera sway
	if (m_bCameraSwayDirection == false)
	{
		m_fCameraSwayAngle -= m_fCameraSwayDeltaAngle;
		if (m_fCameraSwayAngle < m_fCameraSwayAngle_LeftLimit * speedMultiplier)
			m_bCameraSwayDirection = !m_bCameraSwayDirection;
	}
	else
	{
		m_fCameraSwayAngle += m_fCameraSwayDeltaAngle;
		if (m_fCameraSwayAngle > m_fCameraSwayAngle_RightLimit * speedMultiplier)
			m_bCameraSwayDirection = !m_bCameraSwayDirection;
	}

	Vector3 viewVector = (target - position).Normalized();
	if (direction)
	{
		position += viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime;
		//	 Constrain the position
		Constrain();
		// Update the target
		target = position + viewVector;

		return true;
	}
	else
	{
		position -= viewVector * (float)m_dSpeed * (float)deltaTime;
		//	 Constrain the position
		Constrain();
		// Update the target
		target = position + viewVector;
		return true;
	}

	return false;
}
// Detect and process left / right movement on the controller
bool CPlayerInfo::Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	Vector3 viewVector = target - position;
	Vector3 rightUV;
	if (direction)
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position -= rightUV * (float)m_dSpeed * deltaTime;
		// Update the target
		target = position + viewVector;
		return true;
	}
	else
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		position += rightUV * (float)m_dSpeed * deltaTime;
		// Update the target
		target = position + viewVector;
		return true;
	}
	return false;
}

// Detect and process look up / down on the controller
bool CPlayerInfo::Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float pitch = (float)(-m_dSpeed * speedMultiplier * (float)deltaTime);
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	viewUV = rotation * viewUV;
	target = position + viewUV;

	return true;
}
// Detect and process look left / right on the controller
bool CPlayerInfo::Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (target - position).Normalized();
	Vector3 rightUV;

	float yaw = (float)-m_dSpeed * speedMultiplier * (float)deltaTime;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	viewUV = rotation * viewUV;
	target = position + viewUV;
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	up = rightUV.Cross(viewUV).Normalized();

	return true;
}

// Stop sway
bool CPlayerInfo::StopSway(const float deltaTime)
{
	m_bCameraSwayDirection = false;
	m_fCameraSwayAngle = 0.0f;
	up = Vector3(0.0f, 1.0f, 0.0f);
	return true;
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	
	//this messes with camera when crouch, 
	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		if (position.y != (0.f))
		{
			Vector3 viewDirection = target - position;
			//position.y = 0.f;
			switch (theCurrentPosture)
			{
			case STAND:
				position.y = 0.f;
				target = position + viewDirection;
				break;
			case CROUCH:
				position.y = 0.f - 3.0f;
				target = position + viewDirection;
				break;
			case PRONE:
				position.y = 0.f - 5.0f;
				target = position + viewDirection;
				break;
			default:
				break;
			}
		}
		
	}
}

void CPlayerInfo::AttachCamera(Camera3* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}
void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}
int CPlayerInfo::GetHealth(void) const
{
	return m_dHealth;
}
int CPlayerInfo::GetScore(void) const
{
	return m_dScore;
}
