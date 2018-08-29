#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <vector>
class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	std::vector<unsigned char> m_heightMap;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, std::vector<unsigned char> &heightMap);
	void InitMenu(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif