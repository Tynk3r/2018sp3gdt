#include "JointInfo.h"

CJointInfo::CJointInfo(JOINT_TYPE jointType) :
	jointType(jointType),
	keyframeStart(KEYFRAME_NONE),
	keyframeGoal(KEYFRAME_NONE)
{
	std::vector<Vector3> jointInfo = GenerateJointInfo(jointType, KEYFRAME_NONE);
	this->posOffset = this->start_posOffset = this->goal_posOffset = jointInfo[0];
	this->rotation = this->start_rotation = this->goal_rotation = jointInfo[1];
	this->axisOffset = this->start_axisOffset = this->goal_axisOffset = jointInfo[2];
	switch (jointType)
	{
	case TYPE_VANILLA:
		this->jointName = "VANILLA";
		break;
	case TYPE_OCTO_NECK:
		this->jointName = "OCTO_NECK";
		break;
	}
}

CJointInfo::~CJointInfo()
{
}

void CJointInfo::setStartKF(JOINT_KEYFRAME keyframe)
{
	this->keyframeStart = keyframe;
	std::vector<Vector3> jointInfo = GenerateJointInfo(this->jointType, keyframe);
	if (jointInfo.size() == 0)
		return;
	this->start_posOffset = jointInfo[0];
	this->start_rotation = jointInfo[1];
	if (jointInfo.size() == 3)
	{
		this->start_axisOffset = jointInfo[2];
	}
}

void CJointInfo::setGoalKF(JOINT_KEYFRAME keyframe)
{
	this->keyframeGoal = keyframe;
	std::vector<Vector3> jointInfo = GenerateJointInfo(this->jointType, keyframe);
	if (jointInfo.size() == 0) 
		return;
	this->goal_posOffset = jointInfo[0];
	this->goal_rotation = jointInfo[1];
	if (jointInfo.size() == 3)
	{
		this->goal_axisOffset = jointInfo[2];
	}
}

void CJointInfo::moveToKF(JOINT_KEYFRAME keyframe)
{
	this->setStartKF(this->keyframeGoal);
	this->setGoalKF(keyframe);
}

void CJointInfo::Animate(float alpha)
{
	this->posOffset = this->start_posOffset.lerped(this->goal_posOffset, alpha);
	this->rotation = this->start_rotation.lerped(this->goal_rotation, alpha);
	this->axisOffset = this->start_axisOffset.lerped(this->goal_axisOffset, alpha);
}

Vector3 CJointInfo::getPosOffset() const
{
	return this->posOffset;
}

Vector3 CJointInfo::getAxisOffset() const
{
	return this->axisOffset;
}

Vector3 CJointInfo::getRotation() const
{
	return this->rotation;
}

CJointInfo::JOINT_KEYFRAME CJointInfo::getKFStart() const
{
	return this->keyframeStart;
}

CJointInfo::JOINT_KEYFRAME CJointInfo::getKFGoal() const
{
	return this->keyframeGoal;
}

std::vector<Vector3> CJointInfo::GenerateJointInfo(JOINT_TYPE jType, JOINT_KEYFRAME kFrame)
{
	std::vector<Vector3> jKeyframes;//order to push ; posOffset, Rotation, axisOffset(if needed)
	switch (jType)
	{
		case TYPE_OCTO_NECK:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
		case TYPE_OCTO_TRIDENT:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
		case TYPE_OCTO_TENTACLE_LOWER_RIGHT:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3(1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(1, 2, 0));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_FIREBALL_1:
				jKeyframes.push_back(Vector3(1.f, -0.05f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(3, 3, 0));
				break;
			case KEYFRAME_OCTO_FIREBALL_2:
				jKeyframes.push_back(Vector3(1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(1, 1, 0));
				break;
			case KEYFRAME_OCTO_SURPRISED:
				jKeyframes.push_back(Vector3(1.f, 0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, 0));
				break;
			case KEYFRAME_OCTO_VULNERABLE:
				jKeyframes.push_back(Vector3(1.f, 0.25f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(4, 1, 0));
				break;
			case KEYFRAME_OCTO_DEAD:
				jKeyframes.push_back(Vector3(1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, -1));
				break;
			}
			break;
		}
		case TYPE_OCTO_FIREBALL:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3(5, -0.25f, 2));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, -2));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_FIREBALL_1:
				jKeyframes.push_back(Vector3(2.5f, 4.5f, 1));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, 2));
				break;
			case KEYFRAME_OCTO_FIREBALL_2:
				jKeyframes.push_back(Vector3(-0.5f, 1.5f, 6));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 1, 2));
				break;
			case KEYFRAME_OCTO_SURPRISED:
				jKeyframes.push_back(Vector3(3, 6, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, 0));
				break;
			case KEYFRAME_OCTO_VULNERABLE:
				jKeyframes.push_back(Vector3(1.25f, 4.5f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-4, -2, 0));
				break;
			case KEYFRAME_OCTO_DEAD:
				jKeyframes.push_back(Vector3(5, 0, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(1, 0, 1));
				break;
			}
			break;
		}
		case TYPE_OCTO_TENTACLE_LOWER_LEFT:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3(-1.f, -0.15f));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-1, 2, 0));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_ICEBALL_1:
				jKeyframes.push_back(Vector3(-1.f, -0.05f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-3, 3, 0));
				break;
			case KEYFRAME_OCTO_ICEBALL_2:
				jKeyframes.push_back(Vector3(-1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-1, 1, 0));
				break;
			case KEYFRAME_OCTO_SURPRISED:
				jKeyframes.push_back(Vector3(-1.f, 0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, 0));
				break;
			case KEYFRAME_OCTO_VULNERABLE:
				jKeyframes.push_back(Vector3(-1.f, 0.25f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-4, 1, 0));
				break;
			case KEYFRAME_OCTO_DEAD:
				jKeyframes.push_back(Vector3(-1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, -1));
				break;
			}
			break;
		}
		case TYPE_OCTO_ICEBALL:
		{
			switch (kFrame)
			{
			default:
				jKeyframes.push_back(Vector3(-5, -0.25f, 2));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, -2));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			case KEYFRAME_OCTO_ICEBALL_1:
				jKeyframes.push_back(Vector3(-2.5f, 4.5f, 1));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, 2));
				break;
			case KEYFRAME_OCTO_ICEBALL_2:
				jKeyframes.push_back(Vector3(0.5f, 1.5f, 6));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 1, 2));
				break;
			case KEYFRAME_OCTO_SURPRISED:
				jKeyframes.push_back(Vector3(-3, 6, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, 0, 0));
				break;
			case KEYFRAME_OCTO_VULNERABLE:
				jKeyframes.push_back(Vector3(-1.25f, 4.5f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(4, -2, 0));
				break;
			case KEYFRAME_OCTO_DEAD:
				jKeyframes.push_back(Vector3(-5, 0, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-1, 0, 1));
				break;
			}
			break;
		}
	}
	return jKeyframes;
}
