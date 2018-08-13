#ifndef JOYSTICK_CONTROLLER_H
#define JOYSTICK_CONTROLLER_H

#include "SingletonTemplate.h"
#include <bitset>
#include <windows.h>

class JoystickController : public Singleton<JoystickController> //currently only hardcoded to support 1 (dualshock) controller
{
	friend Singleton<JoystickController>;
public:
	enum JOYSTICK_AXES
	{
		LEFTSTICK_X,
		LEFTSTICK_Y,
		RIGHTSTICK_X,
		RIGHTSTICK_Y,
		LEFTTRIGGER,
		RIGHTTRIGGER,

		AXES_TOTAL
	};
	enum JOYSTICK_BUTTONS //dualshock™ controller format
	{
		BUTTON_0,//square button
		BUTTON_1,//x button
		BUTTON_2,//circle button
		BUTTON_3,//triangle button
		BUTTON_4,//left bumper
		BUTTON_5,//right bumper
		BUTTON_6,//left trigger
		BUTTON_7,//right trigger
		BUTTON_8,//share button or somethin
		BUTTON_9,//options button
		BUTTON_10,//left thumbstick
		BUTTON_11,//right thumbstick
		BUTTON_12,//PS button
		BUTTON_13,//touchscreen
		BUTTON_14,//DPAD_up
		BUTTON_15,//DPAD_right
		BUTTON_16,//DPAD_down
		BUTTON_17,//DPAD_left
		BUTTON_TOTAL
	};
	// System Interface
	void UpdateJoystickStatus(int present, const float *axes, const unsigned char *buttons);
	void EndFrameUpdate();
	
	// User Interface
	bool IsJoystickPresent();
	float GetJoystickDelta(JOYSTICK_AXES axes);
	bool IsButtonDown(JOYSTICK_BUTTONS button);
	bool IsButtonUp(JOYSTICK_BUTTONS button);
	bool IsButtonPressed(JOYSTICK_BUTTONS button);
	bool IsButtonReleased(JOYSTICK_BUTTONS button);

	int axesCount;
	int buttonCount;
private:
	JoystickController();
	~JoystickController();

	bool joystickPresent;
	float axes[AXES_TOTAL];
	bool buttons[BUTTON_TOTAL];
	bool buttonsPrev[BUTTON_TOTAL];
};

#endif // JOYSTICK_CONTROLLER_H