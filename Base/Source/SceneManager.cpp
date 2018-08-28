#include "SceneManager.h"

// Init Everything to 0 first
CSceneManager* CSceneManager::instance = nullptr;
CSceneManager::CSceneManager() {
	this->currentSceneID = Scenes::SCENE_START_MENU;
	this->nextSceneID = Scenes::SCENE_START_MENU;
	this->currentLevelID = Scenes::SCENE_TOTAL;
	this->added = false;
}

CSceneManager* CSceneManager::Instance() {
	if (CSceneManager::instance == nullptr) {
		CSceneManager::instance = new CSceneManager();
	}

	// Returns the Object
	return CSceneManager::instance;
}

void CSceneManager::AddScene(Scene* newScene) {

	// Warning
	if (sceneList.size() >= Scenes::SCENE_TOTAL) 
	{
		std::cout << "Scenes in Vector is more than Enum" << std::endl;
		std::cout << "new Scene was not Added, check the Enum" << std::endl;
	}
	// If not more than Enum
	else 
	{
		// Add new Scene to Vector
		this->sceneList.push_back(newScene);
		// Init the new scene
		//InitScene();
	}
}

void CSceneManager::InitScene() {
	// Init Latest Scene
	sceneList[currentSceneID]->Init();
}

void CSceneManager::GoToScene(Scenes SceneID) {
	// Not more than total ammount of Scenes
	nextSceneID = SceneID;
}

void CSceneManager::Update(StopWatch* m_timer) {
	switch (currentSceneID) {
	case SCENE_RANGE:
	case SCENE_RANGE_MOVING:
	case SCENE_RANGE_ELEMENTAL:
	case SCENE_LEVEL1:
	case SCENE_LEVEL2:
	case SCENE_LEVEL4:
	case SCENE_TERRAIN:
	case SCENE_BOSS:
		currentLevelID = currentSceneID;
	default:
		break;
	}
	// Check if currentScene is empty
	if (sceneList[currentSceneID] != nullptr) {
		// Update and Render the scene
		this->sceneList[this->currentSceneID]->Update(m_timer->getElapsedTime());
		this->sceneList[this->currentSceneID]->Render();
	}
	else {
		std::cout << "WARNING! Scene has not been Added. Check AGAIN!" << std::endl;
	}
	// Check if need to change Scene
	ChangeScene();
}

void CSceneManager::ChangeScene() {
	if (nextSceneID != currentSceneID)
	{
		if (nextSceneID != SCENE_IN_GAME_MENU)
		{
			sceneList[currentSceneID]->Exit();
			if (currentLevelID != SCENE_TOTAL && nextSceneID == SCENE_START_MENU && currentSceneID != SCENE_GAME_MENU)
			{
				sceneList[currentLevelID]->Exit();
				currentLevelID = SCENE_TOTAL;
			}
		}
		currentSceneID = nextSceneID;
		if (currentSceneID != currentLevelID)
			InitScene();
	}
}

int CSceneManager::GetCurrentSceneID() {
	// Returns currentSceneID
	return currentSceneID;
}
int CSceneManager::GetCurrentLevelID() {
	return currentLevelID;
}
Scene* CSceneManager::GetCurrentScene() {
	return sceneList[currentSceneID];
}
void CSceneManager::Exit() {
	// Set back to zero
	currentSceneID = 0;

	for (int i = sceneList.size() - 1; i > -1; i--) {
		// Exit from the Scenes
		this->sceneList[this->currentSceneID]->Exit();
		// Clean Up All the Vectors
		this->sceneList.erase(this->sceneList.begin() + i);
	}

	// Just in case 
	sceneList.clear();
}