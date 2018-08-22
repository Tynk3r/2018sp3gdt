#include "RigInfo.h"

CRigInfo::CRigInfo(RIG_TYPE rigType)
{
	switch (rigType)
	{
	case RIG_BOSS:
		this->jointinfos["OCTO_NECK"] = new CJointInfo(CJointInfo::TYPE_OCTO_NECK);
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
		this->jointinfos.erase(it->first);
	}
}

CJointInfo * CRigInfo::GetJoint(std::string index)
{
	return this->jointinfos[index];
}
