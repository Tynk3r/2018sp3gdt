#include "Mouse.h"
#include <iostream>
using namespace std;

#include "MouseController.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Entities/NPC.h"
const bool _CONTROLLER_MOUSE_DEBUG = false;

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

// Create this controller
bool CMouse::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	if (_CONTROLLER_MOUSE_DEBUG)
		cout << "CMouse::Create()" << endl;

	return false;
}


// Read from the controller
int CMouse::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	if (_CONTROLLER_MOUSE_DEBUG)
		cout << "CMouse::Read()" << endl;

	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	// Process the keys for customisation
	if (mouse_diff_x != 0.0)
		Look_LeftRight(deltaTime, true, mouse_diff_x);
	if (mouse_diff_y != 0.0)
		Look_UpDown(deltaTime, true, mouse_diff_y);
	if (thePlayerInfo->GetCurrentNPC() != NULL)
	{
		CNPC* npc = static_cast<CNPC*>(thePlayerInfo->GetCurrentNPC());
		npc->UpdateDialogue(deltaTime);
		if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB) && npc->isDoneWithLine())
		{
			if (npc->finishedDialogue())
			{
				npc->ResetDialogue();
				thePlayerInfo->SetCurrentNPC();
			}
			else
				npc->goToNextDialogue();
		}
		/*if (this->isDoneWithLine())
		{
		this->goToNextDialogue();
		this->charIndex = 0;
		}*/
	}
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB))//if pressed lmb
	{
		if (thePlayerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_IDLE)
		{
			thePlayerInfo->SetAnimState(CPlayerInfo::PLR_ANIM_RIGHTARM_CASTHOLDING);
		}
	}
	else if (MouseController::GetInstance()->IsButtonUp(MouseController::LMB))
	{
		if (thePlayerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_RIGHTARM_CASTHOLDING)
		{
			thePlayerInfo->SetAnimState(CPlayerInfo::PLR_ANIM_RIGHTARM_CASTING);
		}
	}
	if (MouseController::GetInstance()->IsButtonDown(MouseController::RMB))//if pressed lmb
	{
		if (thePlayerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_IDLE)
		{
			thePlayerInfo->SetAnimState(CPlayerInfo::PLR_ANIM_LEFTARM_CASTHOLDING);
		}
	}
	else if (MouseController::GetInstance()->IsButtonUp(MouseController::RMB))
	{
		if (thePlayerInfo->GetAnimState() == CPlayerInfo::PLR_ANIM_LEFTARM_CASTHOLDING)
		{
			thePlayerInfo->SetAnimState(CPlayerInfo::PLR_ANIM_LEFTARM_CASTING);
		}
	}
	/*if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != thePlayerInfo->GetWeapon())
		Change(deltaTime);

	// if Mouse Buttons were activated, then act on them
	if( (thePlayerInfo->GetCurrentWeapon() == 0) && (MouseController::GetInstance()->IsButtonDown(MouseController::LMB)))		
			FirePrimary(deltaTime);
	else if(MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
			FirePrimary(deltaTime);
	else if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
		FireSecondary(deltaTime);*/
	return 0;
}
