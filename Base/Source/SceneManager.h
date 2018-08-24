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
		SCENE_START_MENU,
		SCENE_GAME_MENU,
		SCENE_IN_GAME_MENU,
		SCENE_CONTROLS,
		SCENE_CONTROLS1,
		SCENE_TERRAIN,
		SCENE_RANGE,
		SCENE_BOSS,
		SCENE_RANGE_MOVING,
		SCENE_LEVEL1,
		SCENE_LEVEL2,
		SCENE_LEVEL4,
		SCENE_RANGE_ELEMENTAL,
		SCENE_INSTRUCTIONS,
		SCENE_INSTRUCTIONS1,

		SCENE_TOTAL
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
	// Get Current Level ID
	int GetCurrentLevelID();

	// Init the Scene
	void InitScene();

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
	int nextSceneID;
	int currentLevelID;

	// Change Scene?
	void ChangeScene();

	// Added anot
	bool added;
};