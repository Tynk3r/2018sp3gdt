
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneManager.h"

#include "SceneStartMenu.h"
#include "SceneGameMenu.h"
#include "SceneInGameMenu.h"
#include "SceneControls.h"
#include "SceneControls1.h"
#include "SceneTerrain.h"
#include "SceneRange.h"
#include "SceneBoss.h"
#include "SceneRangeMoving.h"

#include "SceneLevel1Basics.h"
#include "SceneLevel2Burst.h"
#include "SceneLevel4Flight.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;
bool Application::wrapAroundEnabled = false;
bool Application::shouldContinue = true;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if (((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width-m_window_deadzone)) && wrapAroundEnabled)
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if (((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height-m_window_deadzone)) && wrapAroundEnabled)
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

    return false;
}
	
void Application::UpdateInput()
{
	//update mouse input
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
	//update keyboard input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));

	//update joystick input
	if (JoystickController::GetInstance()->IsJoystickPresent())
	{
		JoystickController::GetInstance()->UpdateJoystickStatus(JoystickController::GetInstance()->IsJoystickPresent(),
			glfwGetJoystickAxes(GLFW_JOYSTICK_1, &(JoystickController::GetInstance()->axesCount)),
			glfwGetJoystickButtons(GLFW_JOYSTICK_1, &(JoystickController::GetInstance()->buttonCount)));
	}
}

void Application::PostUpdateInput()
{
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
	JoystickController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "SUPER MAGIC@ GAMEEEEEEEEE!!!!!!!!!!!!!@", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		JoystickController::GetInstance()->UpdateJoystickStatus(1,
			glfwGetJoystickAxes(GLFW_JOYSTICK_1, &(JoystickController::GetInstance()->axesCount)),
			glfwGetJoystickButtons(GLFW_JOYSTICK_1, &(JoystickController::GetInstance()->buttonCount)));
	}

	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Click", "Sound//stone.mp3");
}

void Application::Run()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//Main Loop
	// init scenes
	Scene *scene1 = new SceneStartMenu();
	Scene *scene2 = new SceneGameMenu();
	Scene *scene3 = new SceneInGameMenu();
	Scene *scene4 = new SceneControls();
	Scene *scene5 = new SceneControls1();
	Scene *scene6 = new SceneTerrain();
	Scene *scene7 = new SceneRange();
	Scene *scene8 = new SceneBoss();
	Scene *scene9 = new SceneRangeMoving();

	Scene *scene10 = new SceneLevel1();
	Scene *scene11 = new SceneLevel2();

	CSceneManager* sceneManager = CSceneManager::Instance();
	sceneManager->AddScene(scene1);
	sceneManager->AddScene(scene2);
	sceneManager->AddScene(scene3);
	sceneManager->AddScene(scene4);
	sceneManager->AddScene(scene5);
	sceneManager->AddScene(scene6);
	sceneManager->AddScene(scene7);
	sceneManager->AddScene(scene8);
	sceneManager->AddScene(scene9);
	sceneManager->AddScene(scene10);
	sceneManager->AddScene(scene11);
	sceneManager->GoToScene(CSceneManager::SCENE_START_MENU);
	sceneManager->InitScene();
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	static bool leftButtonDebounce = false;
	while (!glfwWindowShouldClose(m_window) && shouldContinue)// && !Application::IsKeyPressed(VK_ESCAPE))
	{
		switch (sceneManager->GetCurrentSceneID())
		{
		case CSceneManager::SCENE_START_MENU:
		case CSceneManager::SCENE_GAME_MENU:
		case CSceneManager::SCENE_IN_GAME_MENU:
		case CSceneManager::SCENE_CONTROLS:
		case CSceneManager::SCENE_CONTROLS1:
				wrapAroundEnabled = false;
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
		case CSceneManager::SCENE_TERRAIN:
		case CSceneManager::SCENE_RANGE:
		case CSceneManager::SCENE_LEVEL1:
		case CSceneManager::SCENE_LEVEL2:
		default:
				wrapAroundEnabled = true;
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
		}

		GetMouseUpdate();
		UpdateInput();
		//scene1->Update(m_timer.getElapsedTime());
		//scene1->Render();
		CSceneManager::Instance()->Update(&m_timer);
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  
		PostUpdateInput();
	}
	scene1->Exit();
	delete scene1;

	
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
