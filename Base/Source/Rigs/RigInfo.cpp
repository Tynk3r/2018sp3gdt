#include "RigInfo.h"

CRigInfo::CRigInfo(RIG_TYPE rigType)
{
	switch (rigType)
	{
	case RIG_BOSS:
		//this->jointinfos.insert(std::pair<std::string, CJointInfo*>("OCTO_NECK", new CJointInfo(CJointInfo::TYPE_OCTO_NECK)));
		//this->jointinfos.insert(std::pair<std::string, CJointInfo*>("OCTO_TRIDENT", new CJointInfo(CJointInfo::TYPE_OCTO_TRIDENT)));
		this->jointinfos["OCTO_TENTACLE_LOWER_RIGHT"] = new CJointInfo(CJointInfo::TYPE_OCTO_TENTACLE_LOWER_RIGHT);
		this->jointinfos["OCTO_FIREBALL"] = new CJointInfo(CJointInfo::TYPE_OCTO_FIREBALL);
		this->jointinfos["OCTO_TENTACLE_LOWER_LEFT"] = new CJointInfo(CJointInfo::TYPE_OCTO_TENTACLE_LOWER_LEFT);
		this->jointinfos["OCTO_ICEBALL"] = new CJointInfo(CJointInfo::TYPE_OCTO_ICEBALL);
		break;
	}
}

CRigInfo::~CRigInfo()
{
	std::map<std::string, CJointInfo*>::iterator it, end;
	it = this->jointinfos.begin();
	end = this->jointinfos.end();
	while (it != end)
	{
		if (it->second)
			delete it->second;
		it = this->jointinfos.erase(it);
	}
}

CJointInfo * CRigInfo::GetJoint(std::string index)
{
	return this->jointinfos[index];
}

void CRigInfo::MoveToKeyframe(CJointInfo::JOINT_KEYFRAME keyframe)
{
	std::map<std::string, CJointInfo*>::iterator it, end;
	it = this->jointinfos.begin();
	end = this->jointinfos.end();
	if (it->second->getKFGoal() == keyframe)//if the goal is gonan be the same as the start then for now just nah wont update keyframes
		return;
	for (it; it != end; ++it)
	{
		it->second->moveToKF(keyframe);
	}
}

void CRigInfo::SetStartKeyframe(CJointInfo::JOINT_KEYFRAME keyframe)
{
	std::map<std::string, CJointInfo*>::iterator it, end;
	it = this->jointinfos.begin();
	end = this->jointinfos.end();
	for (it; it != end; ++it)
	{
		it->second->setStartKF(keyframe);
	}
}

void CRigInfo::SetGoalKeyframe(CJointInfo::JOINT_KEYFRAME keyframe)
{
	std::map<std::string, CJointInfo*>::iterator it, end;
	it = this->jointinfos.begin();
	end = this->jointinfos.end();
	for (it; it != end; ++it)
	{
		it->second->setGoalKF(keyframe);
	}
}

void CRigInfo::Animate(float alpha)
{
	std::map<std::string, CJointInfo*>::iterator it, end;
	it = this->jointinfos.begin();
	end = this->jointinfos.end();
	for (it; it != end; ++it)
	{
		it->second->Animate(alpha);
	}
}
