#pragma once
#include "Vector3.h"
#include <string>

class CJointInfo
{
public:
	
private:
	std::string jointName;

	Vector3 posOffset;
	Vector3 axisOffset;
	Vector3 rotation;
};

