#include "NPC.h"
#include "MouseController.h"
CNPC::CNPC(Vector3 pos, Vector3 scale, Vector3 target) :
	CEntity(),
	type(NPC_WIZARD),
	playerRef(NULL),
	charIndex(0),
	dialogueIndex(0)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setTarget(target);
	this->setOriginPos(pos);
	this->Init();
}

CNPC::CNPC(NPC_TYPE npctype, Vector3 pos, Vector3 scale, Vector3 target) :
	CEntity(),
	type(npctype),
	playerRef(NULL),
	charIndex(0),
	dialogueIndex(0)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setTarget(target);
	this->setOriginPos(pos);
	this->Init();
}


CNPC::~CNPC()
{
}

void CNPC::Init()
{
	setCollider(true);
	setType(E_NPC);
	switch (this->type)
	{
	case NPC_WIZARD:
		this->dialogues.push_back("hello i am a wizardz!!!");
		this->dialogues.push_back("shoot the fire barrels with ice");
		this->dialogues.push_back("shoot the ice barrels with fire");
		break;
	case NPC_WIZARDBASIC:
		this->dialogues.push_back("Your training has just begun!");
		this->dialogues.push_back("Destroy all these targets!");
		this->dialogues.push_back("You'll have some restrictions..");
		this->dialogues.push_back("Don't let your Time-gic run out!");
		break;
	case NPC_WIZARDLEVEL2:
		this->dialogues.push_back("Emergency! Octowitches spotted!");
		this->dialogues.push_back("These grounds are under attack!");
		this->dialogues.push_back("Defeat them with your magic!");
		this->dialogues.push_back("They can be stunned if you..");
		this->dialogues.push_back("..interrupt their attack..");
		this->dialogues.push_back("..by attacking it with..");
		this->dialogues.push_back("..the opposite element!");
		this->dialogues.push_back("Remember to destroy all targets.");
		break;
	case NPC_WIZARDLEVEL3:
		this->dialogues.push_back("Oh boy, this haze is thick!");
		this->dialogues.push_back("Use fire to see the way!");
		this->dialogues.push_back("Ice will create more haze.. ");
		this->dialogues.push_back("..use it to obscure Octowitches!");
		this->dialogues.push_back("Follow the signs to not get lost.");
		this->dialogues.push_back("Spells can leave ground markers..");
		this->dialogues.push_back("Use all that you have to succeed!");
		this->dialogues.push_back("You must head to the central nest.");
		this->dialogues.push_back("You will obtain your dragon there.");
		this->dialogues.push_back("However, it angers the Blood Sea.");
		this->dialogues.push_back("Be prepared to fly high to flee!");
		this->dialogues.push_back("There are 4 land targets...and");
		this->dialogues.push_back("1 air target, remember!");
		this->dialogues.push_back("Summon Dragon with 20 mana..");
		this->dialogues.push_back("..using the 'U' button");
		this->dialogues.push_back("Afterwards, fly into the sky!");
		this->dialogues.push_back("I believe in you!!");
		break;
	case NPC_WIZARDBOSS:
		this->dialogues.push_back("The time has come!");
		this->dialogues.push_back("Its the Inker of Worlds..");
		this->dialogues.push_back("..The Octopus God!!");
		this->dialogues.push_back("Stun him by interrupting his");
		this->dialogues.push_back("attacks, then go all out!!");
		break;
	default:
		this->dialogues.push_back("hello i am a wizardz!!!");
		this->dialogues.push_back("shoot the fire barrels with ice");
		this->dialogues.push_back("shoot the ice barrels with fire");
		break;
	}
	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this);
}

void CNPC::Update(double dt)
{
	CEntity::Update(dt);
	if (this->playerRef != NULL)
	{
		CPlayerInfo* plr = this->playerRef;
		Vector3 npctoplayer = (plr->getPos() - this->getPos());
		try 
		{ 
			float lengthSQ = npctoplayer.LengthSquared();
			if (lengthSQ < 100 * 100)//curernt hardcoded sphere "area" detection
			{
				if (MouseController::GetInstance()->IsButtonDown(MouseController::MMB) && plr->GetCurrentNPC() == NULL)
				{
					plr->SetCurrentNPC(this);
				}
			}
			else
			{
				
			}
		}
		catch (exception) {}
	}
}

string CNPC::getCurrentLine()
{
	string toreturn(this->dialogues[this->dialogueIndex]);
	int lengtho = this->dialogues[this->dialogueIndex].size();
	toreturn.substr(0, lengtho - this->charIndex);
	return toreturn;
}

void CNPC::setPlayerRef(CPlayerInfo * playerRef)
{
	this->playerRef = playerRef;
}

void CNPC::goToNextDialogue()
{
	if (this->dialogueIndex < this->dialogues.size() - 1)
	{
		this->dialogueIndex++;
		this->charIndex = 0;
	}
}

bool CNPC::isDoneWithLine()
{
	if (charIndex >= (int)(this->dialogues[this->dialogueIndex].length()) - 1)
		return true;
	return false;
}

bool CNPC::finishedDialogue()
{
	if (this->dialogueIndex >= this->dialogues.size() - 1)
		return true;
	return false;
}

void CNPC::ResetDialogue()
{
	this->charIndex = this->dialogueIndex = 0;
}

void CNPC::UpdateDialogue(double dt)
{
	this->charIndex += Math::Max((float)dt * 5.f, 1.f);
	if (this->isDoneWithLine())
		this->charIndex = (int)(this->dialogues[this->dialogueIndex].length() - 1);
	/*if (this->isDoneWithLine())
	{
		this->goToNextDialogue();
		this->charIndex = 0;
	}*/
}
