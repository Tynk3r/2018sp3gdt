#include "JoystickController.h"
#include "MyMath.h"
JoystickController::JoystickController() :joystickPresent(false)
{
}

JoystickController::~JoystickController()
{
}
void JoystickController::UpdateJoystickStatus(int present, const float * axes, const unsigned char * buttons)
{
	this->joystickPresent = present;
	if (present)
	{
		for (int i = 0; i < AXES_TOTAL; ++i)
		{
			this->axes[i] = axes[i];
		}
		for (int i = 0; i < BUTTON_TOTAL; ++i)
		{
			this->buttons[i] = buttons[i];
		}
	}
}
void JoystickController::EndFrameUpdate()
{
	if (this->joystickPresent)
	{
		for (int i = 0; i < BUTTON_TOTAL; ++i)
		{
			buttonsPrev[i] = buttons[i];
		}
	}
}

bool JoystickController::IsJoystickPresent()
{
	return this->joystickPresent;
}

float JoystickController::GetJoystickDelta(JOYSTICK_AXES axes)
{
	if (!this->joystickPresent || (this->axes[axes]<0.05f && this->axes[axes] > -0.05f))
		return 0.f;
	else
		return this->axes[axes];
}

bool JoystickController::IsButtonDown(JOYSTICK_BUTTONS button)
{
	return this->joystickPresent && this->buttons[button];
}

bool JoystickController::IsButtonUp(JOYSTICK_BUTTONS button)
{
	return this->joystickPresent && !this->buttons[button];
}

bool JoystickController::IsButtonPressed(JOYSTICK_BUTTONS button)
{
	return this->joystickPresent && IsButtonDown(button) && !this->buttonsPrev[button];
}

bool JoystickController::IsButtonReleased(JOYSTICK_BUTTONS button)
{
	return this->joystickPresent && IsButtonUp(button) && this->buttonsPrev[button];
}