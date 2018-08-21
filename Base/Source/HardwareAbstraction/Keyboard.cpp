#include "Keyboard.h"
#include "../SoundEngine.h"

#include <iostream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../TimeTrackerManager.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

CKeyboard::CKeyboard()
{
}


CKeyboard::~CKeyboard()
{
}

// Create this controller
bool CKeyboard::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	//CSoundEngine::GetInstance()->AddSound("walking", "Sound//walking.mp3");
	CSoundEngine* sEngine = CSoundEngine::GetInstance();
	sEngine->AddSound("walking", "Sound//walking.mp3");
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CKeyboard::Create()" << endl;
	return false;
}	


// Read from the controller
int CKeyboard::Read(const float deltaTime)
{

	CController::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
		cout << "CKeyboard::Read()" << endl;
	
	// Process the keys for customisation
	
	if ((KeyboardController::GetInstance()->IsKeyDown('W')) && (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)))
		Move_FrontBack(deltaTime, true, 2.0f);
	else if (KeyboardController::GetInstance()->IsKeyDown('W'))
		Move_FrontBack(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown('S'))
	{
	//	CSoundEngine* sEngine = CSoundEngine::GetInstance();
	//	CSoundEngine::GetInstance()->PlayASound("walking");

	//	std::cout << "is it playimngfsafs" <<  sEngine->GetSoundEngine()->isCurrentlyPlaying("walking") << std::endl;
		Move_FrontBack(deltaTime, false);
	}
	else
	{
	//	CSoundEngine::GetInstance()->stopGame();
	}
	if (KeyboardController::GetInstance()->IsKeyDown('A'))
		Move_LeftRight(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown('D'))
		Move_LeftRight(deltaTime, false);

	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		Look_UpDown(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		Look_UpDown(deltaTime, false);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		Look_LeftRight(deltaTime, true);
	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		Look_LeftRight(deltaTime, false);

	// If the user presses SPACEBAR, then make him jump
	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE) &&
		thePlayerInfo->isOnGround())
		Jump(true);

	// Update the weapons
	/*if (KeyboardController::GetInstance()->IsKeyReleased('R'))
		Reload(deltaTime);*/

	// If the user presses R key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		Reset();

	TimeTrackerManager* tTracker = TimeTrackerManager::GetInstance();
	if (KeyboardController::GetInstance()->IsKeyDown('N'))
		tTracker->setSpeed(tTracker->getSpeed() - deltaTime);
	if (KeyboardController::GetInstance()->IsKeyDown('M'))
		tTracker->setSpeed(tTracker->getSpeed() + deltaTime);


	return 0;
}
