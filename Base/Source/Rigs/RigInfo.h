#pragma once
#include "Vector3.h"
#include "JointInfo.h"
#include <string>
#include <map>

class CRigInfo
{
public:
	enum RIG_TYPE
	{
		RIG_NONE,
		RIG_BOSS,
	};
	CRigInfo(RIG_TYPE rigType = RIG_NONE);
	~CRigInfo();
	CJointInfo* GetJoint(std::string index);
private:
	std::map<std::string, CJointInfo*> jointinfos;
};

