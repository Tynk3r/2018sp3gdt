#ifndef TIME_TRACKER_MANAGER_H
#define TIME_TRACKER_MANAGER_H

#include "SingletonTemplate.h"
#include <list>

class TimeTrackerManager : public Singleton<TimeTrackerManager>
{
	friend Singleton<TimeTrackerManager>;
public:
	void Update(double _dt) { this->timeElapsed += (float)_dt * this->speed; }
	void Reset() { this->timeElapsed = 0; this->speed = 1; }
	void setSpeed(float speed) { if (speed < 0) this->speed = 0; else this->speed = speed; }
	float getElapsedTime() const { return this->timeElapsed; }
	float getSpeed() const { return this->speed; }
private:
	TimeTrackerManager() :timeElapsed(0), speed(1) {}
	virtual ~TimeTrackerManager() {}
	float timeElapsed;
	float speed;
};

#endif // TIME_TRACKER_MANAGER_H