#include "SceneManager.h"

// Init Everything to 0 first
CSceneManager* CSceneManager::instance = nullptr;
CSceneManager::CSceneManager() {
	this->currentSceneID = Scenes::START_MENU;
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
	if (sceneList.size() >= Scenes::TOTAL_SCENES) {
		std::cout << "Scenes in Vector is more than Enum!!" << std::endl;
		std::cout << "new Scene was NOT Added, check your Enum AGAIN!" << std::endl;
	}
	// If not more than Enum
	else {
		// Add new Scene to Vector
		this->sceneList.push_back(newScene);
		// Init the new scene
		InitScene();
	}
}

void CSceneManager::InitScene() {
	// Init Latest Scene
	sceneList[sceneList.size() - 1]->Init();
}

void CSceneManager::GoToScene(Scenes SceneID) {
	// Not more than total ammount of Scenes
	if (SceneID < Scenes::TOTAL_SCENES) {
		this->currentSceneID = SceneID;
	}
}

void CSceneManager::Update(StopWatch* m_timer) {
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

	// Check if in Main Game
	if (currentSceneID == Scenes::START_MENU) {
		/*if (sceneList[currentSceneID]->waveCount) {

		}*/
	}
}

int CSceneManager::GetCurrentSceneID() {
	// Returns currentSceneID
	return currentSceneID;
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