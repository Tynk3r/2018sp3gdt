#include "JointInfo.h"

CJointInfo::CJointInfo(JOINT_TYPE jointType) :
	jointType(jointType),
	keyframeStart(KEYFRAME_NONE),
	keyframeGoal(KEYFRAME_NONE)
{
	std::vector<Vector3> jointInfo = GenerateJointInfo(jointType, KEYFRAME_NONE);
	this->posOffset = jointInfo[0];
	this->rotation = jointInfo[1];
	this->axisOffset = jointInfo[2];
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
	
}

void CJointInfo::setGoalKF(JOINT_KEYFRAME keyframe)
{
}

void CJointInfo::moveToKF(JOINT_KEYFRAME keyframe)
{
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

std::vector<Vector3> CJointInfo::GenerateJointInfo(JOINT_TYPE jType, JOINT_KEYFRAME kFrame)
{
	std::vector<Vector3> jKeyframes;//order to push ; posOffset, Rotation, axisOffset(if needed)
	switch (jType)
	{
		case TYPE_OCTO_NECK:
		{
			switch (kFrame)
			{
			case KEYFRAME_NONE:
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
			case KEYFRAME_NONE:
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
			case KEYFRAME_NONE:
				jKeyframes.push_back(Vector3(1.f, -0.15f, 0));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(1, 2, 0));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
		case TYPE_OCTO_FIREBALL:
		{
			switch (kFrame)
			{
			case KEYFRAME_NONE:
				jKeyframes.push_back(Vector3(5, -0.25f, 2));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, -2));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
		case TYPE_OCTO_TENTACLE_LOWER_LEFT:
		{
			switch (kFrame)
			{
			case KEYFRAME_NONE:
				jKeyframes.push_back(Vector3(-1.f, -0.15f));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(-1, 2, 0));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
		case TYPE_OCTO_ICEBALL:
		{
			switch (kFrame)
			{
			case KEYFRAME_NONE:
				jKeyframes.push_back(Vector3(-5, -0.25f, 2));
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3(0, -1, -2));
				break;
			case KEYFRAME_OCTO_TRIDENT_1:
				jKeyframes.push_back(Vector3());
				jKeyframes.push_back(Vector3());
				break;
			}
			break;
		}
	}
	return jKeyframes;
}
