#pragma once
#include "Vector3.h"
#include <string>
#include <vector>

class CJointInfo
{
public:
	enum JOINT_TYPE
	{
		TYPE_VANILLA,//a "clean" blank joint
		TYPE_OCTO_NECK,//between the head and the body
		TYPE_TOTAL
	};
	enum JOINT_KEYFRAME
	{
		KEYFRAME_NONE,
		KEYFRAME_OCTO_TRIDENT_1,//rising up his trident thing
		KEYFRAME_TOTAL
	};
	CJointInfo(JOINT_TYPE jointType = TYPE_TOTAL);
	virtual ~CJointInfo();

	void setStartKF(JOINT_KEYFRAME keyframe);
	void setGoalKF(JOINT_KEYFRAME keyframe);
	void moveToKF(JOINT_KEYFRAME keyframe);//sets the <current goal> to the <future start>, and the <future goal> to be `keyframe`

	Vector3 getPosOffset() const;
	Vector3 getAxisOffset() const;
	Vector3 getRotation() const;
private:
	std::string jointName;
	JOINT_TYPE jointType;
	JOINT_KEYFRAME keyframeStart;
	JOINT_KEYFRAME keyframeGoal;

	Vector3 posOffset;
	Vector3 axisOffset;
	Vector3 rotation;

	Vector3 start_posOffset;
	//Vector3 start_axisOffset;
	Vector3 start_rotation;

	Vector3 goal_posOffset;
	//Vector3 goal_axisOffset;
	Vector3 goal_rotation;

	std::vector<Vector3> GenerateJointInfo(JOINT_TYPE jType, JOINT_KEYFRAME kFrame);
};

