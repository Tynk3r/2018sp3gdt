#pragma once
#include <vector>
#include <iostream>
#include "Scene.h"
#include "timer.h"

/**

- This class is supposed to Init ALL scenes first Before doing anything,
make sure that has been done first.


**/

class CSceneManager {
public:
	// Total Amount of Scenes
	enum Scenes {
		START_MENU,
		GAME_MENU,
		GAME,
		CONTROLS,
		IN_GAME_MENU,
		CONTROLS1,

		TOTAL_SCENES
	};

	CSceneManager();

	// Returns Current Object
	static CSceneManager* Instance();

	// Add new Scene to Vector
	void AddScene(Scene* newScene);

	// Set to A Specific Scene
	void GoToScene(Scenes SceneID);

	// Update Current Scene
	void Update(StopWatch* dt);

	// Get Current SceneID
	int GetCurrentSceneID();

	// Get Current Scene
	Scene* GetCurrentScene();

	// Exit
	void Exit();
private:
	// The Actual instance
	static CSceneManager* instance;

	// The vector to contain all the scenes
	std::vector<Scene*> sceneList;

	// To Store the Current Scene enum
	int currentSceneID;

	// Init the Scene
	void InitScene();

	// Change Scene?
	void ChangeScene();

	// Added anot
	bool added;
};