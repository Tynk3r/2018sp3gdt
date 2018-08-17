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
	, m_dSpeed(200.f)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(30.f)
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
	setType(E_PLAYER);

	// Set the current values
	setPos(Vector3(100, 0, 0));
	setTarget(Vector3(0, 0, 50));
	setScale(Vector3(20, 10, 20));
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	camBobHeight = 0;
	camBobRotate = 0;
	camBobTime = 0;
	camBobMaxTimeWalk = 0.8;
	camBobMaxTimeRun = 0.6;

	m_fCameraSwayAngle = 0.0f;
	m_fCameraSwayDeltaAngle = 0.3f;
	m_fCameraSwayAngle_LeftLimit = -3.0f;
	m_fCameraSwayAngle_RightLimit = 3.0f;
	m_bCameraSwayDirection = false;

	screenshakeOffset.SetZero();
	cameraSway.SetZero();

	hasMoved = false;
	hasRan = false;
	canCast = false;

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
	setCollider(true);
	currentAnimState = PLR_ANIM_IDLE;
	animFrame = 0;
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
		m_dJumpSpeed = 30.f;
	}
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
	setPos(defaultPosition);
	setTarget(defaultTarget);
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
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
	setPos(getPos() + Vector3(0.f, (float)(m_dJumpSpeed * m_dElapsedTime + 0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime), 0.f));
	setTarget(getTarget() + Vector3(0.f, (float)(m_dJumpSpeed * m_dElapsedTime + 0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime), 0.f));
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
	setPos(getPos() + Vector3(0.f, (float)(m_dFallSpeed * m_dElapsedTime + 0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime), 0.f));
	setTarget(getTarget() + Vector3(0.f, (float)(m_dFallSpeed * m_dElapsedTime + 0.5 * m_dJumpAcceleration * m_dElapsedTime * m_dElapsedTime), 0.f));
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * m_dElapsedTime;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (getPos().y < 0.f)
	{
		Vector3 viewDirection = getTarget() - getPos();
		setPos(Vector3(getPos().x, 0.f, getPos().z));
		setTarget(getPos() + viewDirection);
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
		m_dElapsedTime = 0.0;
	}
}

void CPlayerInfo::SetAnimState(PLR_ANIM_STATE state)
{
	this->currentAnimState = state;
}

void CPlayerInfo::SetCanCast(bool cancast)
{
	this->canCast = cancast;
}

Vector3 CPlayerInfo::GetScreenshake() const
{
	return screenshakeOffset;
}

Vector3 CPlayerInfo::GetCameraSway() const
{
	return cameraSway;
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
		Vector3 viewDirection = getTarget() - getPos();
		switch (theCurrentPosture)
		{
		case STAND:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			setPos(getPos() + Vector3(0.f, 8.0f, 0.f));
			setTarget(getPos() + viewDirection);
			m_dSpeed = 200.f;
			break;
		case CROUCH:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			setPos(getPos() + Vector3(0.f, -3.0f, 0.f));
			setTarget(getPos() + viewDirection);
			m_dSpeed = 150.f;
			break;
		case PRONE:
			//position.y = m_pTerrain->Get0.f(Vector3(position.x, 0.0f, position.z));
			setPos(getPos() + Vector3(0.f, -5.0f, 0.f));
			setTarget(getPos() + viewDirection);
			m_dSpeed = 50.f;
			break;
		default:
			break;
		}
	}

	// Constrain the position
	Constrain();
	UpdateJumpUpwards(dt);
	UpdateFreeFall(dt);
		
	double timeLim = camBobMaxTimeWalk;

	if (hasMoved && camBobTime <= 0.0) camBobTime = timeLim;

	if (camBobTime > 0.0 && camBobTime <= timeLim && hasMoved)
	{
		camBobTime -= dt;
		camBobHeight = -Math::FAbs(0.01 * sinf(((timeLim - camBobTime) / timeLim) * (2 + hasRan * 0.4) * Math::PI));
		camBobRotate = 0.008 * sinf(((timeLim - camBobTime) / timeLim) * (2 + hasRan * 0.4) * Math::PI);
	}
	else
	{
		camBobHeight = 0;
		camBobRotate = 0;
		camBobTime = 0.0;
	}

	if (up.y < 0) up = -up;

	if (cameraSway.x > 0)
	{
		cameraSway.x -= 2 * dt;
		if (cameraSway.x < 0) cameraSway.x = 0;
	}
	else if (cameraSway.x < 0)
	{
		cameraSway.x += 2 * dt;
		if (cameraSway.x > 0) cameraSway.x = 0;
	}

	if (screenShakeOn) { screenshakeOffset.Set(Math::RandFloatMinMax(-2.5, 2.5), Math::RandFloatMinMax(-2.5, 2.5), Math::RandFloatMinMax(-2.5, 2.5)); }
	else { screenshakeOffset.SetZero(); }

	// Update minimap rotation angle
	Vector3 viewUV = (getTarget() - getPos()).Normalized();/*
	CMinimap::GetInstance()->SetAngle(atan2(viewUV.z, viewUV.x) * 57.2883513685549146);*/

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		Vector3 camDir = viewUV;
		float crot = cosf(camBobRotate);
		float srot = sinf(camBobRotate);
		camDir.Set(camDir.x * crot + camDir.z * srot, camDir.y, -camDir.x * srot + camDir.z * crot);

		attachedCamera->position = getPos() + Vector3(0.f, terrainHeight + 75, 0.f) + screenshakeOffset;
		attachedCamera->target = getPos() + camDir + Vector3(0, camBobHeight, 0) + Vector3(0.f, terrainHeight + 75, 0.f) + screenshakeOffset;
		attachedCamera->up = up.Normalized();
	}
	
	//Update Right & Left Arm offset & rotations
	float mouseDelta_x, mouseDelta_y;
	MouseController::GetInstance()->GetMouseDelta(mouseDelta_x, mouseDelta_y);
	{
		float swayMaxAmt = 0.85f;//the max amt of sway 
		Vector3 newLeftArmPos, newRightArmPos;// the new "offsets" of the arms, they vary depending on the anim's curernt anim state
		Vector3 newLeftArmRot, newRightArmRot;//new rotations
		float alpha = Math::Min((float)dt*10.f, 0.4f);
		switch (this->currentAnimState)
		{
		case PLR_ANIM_CASTING:
			this->animFrame += (float)dt;
			newRightArmPos = Vector3(0, 0.15f, 0);//newLeftArmPos.lerped(Vector3(0,0.1f,0), alpha*1.)
			newRightArmRot = Vector3(40, 0, -150);
			if (this->animFrame > 0.4f)
			{
				this->animFrame = 0;
				this->currentAnimState = PLR_ANIM_CASTED;
				this->canCast = true;
			}
			break;
		case PLR_ANIM_CASTED:
			this->animFrame += (float)dt;
			newRightArmPos = Vector3(0, 0.1f, -0.35f);//newLeftArmPos.lerped(Vector3(0,0.1f,0), alpha*1.)
			newRightArmRot = Vector3(-10, 20, -20);
			if (this->animFrame > 0.5f)
			{
				this->animFrame = 0;
				this->currentAnimState = PLR_ANIM_IDLE;
			}
			break;
		}
		float arm_sway_x = Math::Clamp(-mouseDelta_x*0.1f, -swayMaxAmt, swayMaxAmt);// the "sway" of the arms, varies depending on the mouse deltas
		float arm_sway_y = Math::Clamp(mouseDelta_y*0.1f, -swayMaxAmt, swayMaxAmt);
		Vector3 leftArmGoal = newLeftArmPos + Vector3(arm_sway_x, arm_sway_y);
		Vector3 rightArmGoal = newRightArmPos + Vector3(arm_sway_x, arm_sway_y);
		leftArmOffset = leftArmOffset.lerped(leftArmGoal, alpha);
		rightArmOffset = rightArmOffset.lerped(rightArmGoal, alpha);
		leftArmRotation = leftArmRotation.lerped(newLeftArmRot, alpha);
		rightArmRotation = rightArmRotation.lerped(newRightArmRot, alpha);
	}
	CEntity::Update(dt);
}

// Detect and process front / back movement on the controller
bool CPlayerInfo::Move_FrontBack(const float deltaTime, const bool direction, const float speedMultiplier)
{
	hasMoved = true;
	if (speedMultiplier > 1) hasRan = true;
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

	Vector3 viewVector = (getTarget() - getPos()).Normalized();
	Vector3 tempPos = getPos();
	if (direction)
	{
		setPos(getPos() + viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime);
		std::list<CEntity*>::iterator it, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if (EntityManager::GetInstance()->CheckAABBCollision(*it, this))
			{
				setPos(getPos() - (viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime) - (viewVector * (float)deltaTime * (*it)->getScale().LengthSquared()));
				break;
			}
		}
		//	 Constrain the position
		Constrain();
		// Update the target
		setTarget(getPos() + viewVector);
		return true;
	}
	else
	{
		setPos(getPos() - viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime);
		std::list<CEntity*>::iterator it, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if (EntityManager::GetInstance()->CheckAABBCollision(*it, this))
			{
				setPos(getPos() + viewVector * (float)m_dSpeed * speedMultiplier * (float)deltaTime + (viewVector * (float)deltaTime * (*it)->getScale().LengthSquared()));
				break;
			}
		}
		//	 Constrain the position
		Constrain();
		// Update the target
		setTarget(getPos() + viewVector);
		return true;
	}

	return false;
}
// Detect and process left / right movement on the controller
bool CPlayerInfo::Move_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	hasMoved = true;

	Vector3 viewVector = getTarget() - getPos();
	Vector3 rightUV;
	if (direction)
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		setPos(getPos() - rightUV * (float)m_dSpeed * deltaTime);
		std::list<CEntity*>::iterator it, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if (EntityManager::GetInstance()->CheckAABBCollision(*it, this))
			{
				setPos(getPos() + rightUV * (float)m_dSpeed * deltaTime + (rightUV * (float)deltaTime * (*it)->getScale().LengthSquared()));
				break;
			}
		}
		// Update the target
		setTarget(getPos() + viewVector);
		return true;
	}
	else
	{
		rightUV = (viewVector.Normalized()).Cross(up);
		rightUV.y = 0;
		rightUV.Normalize();
		setPos(getPos() + rightUV * (float)m_dSpeed * deltaTime);
		std::list<CEntity*>::iterator it, end;
		end = EntityManager::GetInstance()->entityList.end();
		for (it = EntityManager::GetInstance()->entityList.begin(); it != end; ++it)
		{
			if (EntityManager::GetInstance()->CheckAABBCollision(*it, this))
			{
				setPos(getPos() - rightUV * (float)m_dSpeed * deltaTime - (rightUV * (float)deltaTime * (*it)->getScale().LengthSquared()));
				break;
			}
		}
		// Update the target
		setTarget(getPos() + viewVector);
		return true;
	}
	return false;
}

// Detect and process look up / down on the controller
bool CPlayerInfo::Look_UpDown(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	Vector3 viewUV = (getTarget() - getPos()).Normalized();
	Vector3 rightUV;

	float pitch = (float)(-m_dSpeed * speedMultiplier * (float)deltaTime) * 0.4f;
	rightUV = viewUV.Cross(up);
	rightUV.y = 0;
	rightUV.Normalize();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	if (atan2((rotation * viewUV).y, 1) > Math::PI / 6) rotation.SetToRotation(Math::RadianToDegree(Math::PI / 6 - atan2(viewUV.y, 1)), rightUV.x, rightUV.y, rightUV.z);
	else if (atan2((rotation * viewUV).y, 1) < -Math::PI / 6) rotation.SetToRotation(Math::RadianToDegree(-Math::PI / 6 - atan2(viewUV.y, 1)), rightUV.x, rightUV.y, rightUV.z);
	viewUV = rotation * viewUV;
	setTarget(getPos() + viewUV);
	up = rightUV.Cross(viewUV).Normalized();

	return true;
}
// Detect and process look left / right on the controller
bool CPlayerInfo::Look_LeftRight(const float deltaTime, const bool direction, const float speedMultiplier)
{
	if (speedMultiplier == 0.0f)
		return false;

	cameraSway.x -= speedMultiplier / 40;
	if (cameraSway.x > 1.0) cameraSway.x = 1.0;
	else if (cameraSway.x < -1.0) cameraSway.x = -1.0;
	Vector3 viewUV = (getTarget() - getPos()).Normalized();
	Vector3 rightUV;

	float yaw = (float)-m_dSpeed * speedMultiplier * (float)deltaTime * 0.4f;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	viewUV = rotation * viewUV;
	setTarget(getPos() + viewUV);
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
		if (getPos().y != (0.f))
		{
			Vector3 viewDirection = getTarget() - getPos();
			//position.y = 0.f;
			switch (theCurrentPosture)
			{
			case STAND:
				setPos(Vector3(getPos().x, 0.f, getPos().z));
				setTarget(getPos() + viewDirection);
				break;
			case CROUCH:
				setPos(Vector3(getPos().x, -3.f, getPos().z));
				setTarget(getPos() + viewDirection);
				break;
			case PRONE:
				setPos(Vector3(getPos().x, -5.f, getPos().z));
				setTarget(getPos() + viewDirection);
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

Vector3 CPlayerInfo::GetLeftArmOffset() const
{
	return this->leftArmOffset;
}

Vector3 CPlayerInfo::GetRightArmOffset() const
{
	return this->rightArmOffset;
}

Vector3 CPlayerInfo::GetLeftArmRotation() const
{
	return this->leftArmRotation;
}

Vector3 CPlayerInfo::GetRightArmRotation() const
{
	return this->rightArmRotation;
}

CPlayerInfo::PLR_ANIM_STATE CPlayerInfo::GetAnimState() const
{
	return this->currentAnimState;
}

bool CPlayerInfo::CanCast() const
{
	return this->canCast;
}
