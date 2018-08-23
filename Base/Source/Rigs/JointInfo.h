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

		TYPE_OCTO_TRIDENT,

		TYPE_OCTO_TENTACLE_LOWER_RIGHT,
		TYPE_OCTO_FIREBALL,

		TYPE_OCTO_TENTACLE_LOWER_LEFT,
		TYPE_OCTO_ICEBALL,

		TYPE_TOTAL
	};
	enum JOINT_KEYFRAME
	{
		KEYFRAME_NONE,
		KEYFRAME_OCTO_TRIDENT_1,//rising up his trident thing
		KEYFRAME_OCTO_FIREBALL_1,//raising his right arm 
		KEYFRAME_OCTO_FIREBALL_2,//kinda does a throwing motion with his right arm
		KEYFRAME_OCTO_ICEBALL_1,//raising his left arm 
		KEYFRAME_OCTO_ICEBALL_2,//kinda does a throwing motion with his left arm
		KEYFRAME_OCTO_SURPRISED,//like hes very surprised so he puts his tentacles up i guess
		KEYFRAME_OCTO_VULNERABLE,//octo is vulnerabl!!!!
		KEYFRAME_OCTO_DEAD,//octo dead
		KEYFRAME_TOTAL
	};
	CJointInfo(JOINT_TYPE jointType = TYPE_TOTAL);
	virtual ~CJointInfo();

	void setStartKF(JOINT_KEYFRAME keyframe);
	void setGoalKF(JOINT_KEYFRAME keyframe);
	void moveToKF(JOINT_KEYFRAME keyframe);//sets the <current goal> to the <future start>, and the <future goal> to be `keyframe`

	void Animate(float alpha);

	Vector3 getPosOffset() const;
	Vector3 getAxisOffset() const;
	Vector3 getRotation() const;
	JOINT_KEYFRAME getKFStart() const;
	JOINT_KEYFRAME getKFGoal() const;
private:
	std::string jointName;
	JOINT_TYPE jointType;
	JOINT_KEYFRAME keyframeStart;
	JOINT_KEYFRAME keyframeGoal;

	Vector3 posOffset;
	Vector3 axisOffset;
	Vector3 rotation;

	Vector3 start_posOffset;
	Vector3 start_axisOffset;
	Vector3 start_rotation;

	Vector3 goal_posOffset;
	Vector3 goal_axisOffset;
	Vector3 goal_rotation;

	std::vector<Vector3> GenerateJointInfo(JOINT_TYPE jType, JOINT_KEYFRAME kFrame);
};

