
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "JoystickController.h"

struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	void UpdateInput();//right now it only updates joystick
	void PostUpdateInput();
	
	static bool IsKeyPressed(unsigned short key);
	static bool GetMouseUpdate();
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);
	const static int GetWindowWidth() { return m_window_width; }
	const static int GetWindowHeight() { return m_window_height; }
	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;
	static bool wrapAroundEnabled;
	void setCont(bool c) { shouldContinue = c; }

private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 100;
	const static int m_window_width = 1280;
	const static int m_window_height = 720;
	static bool shouldContinue;
	//Declare a window object
	StopWatch m_timer;
};

#endif