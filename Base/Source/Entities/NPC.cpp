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
		this->dialogues.push_back("hebdasdjsa i a am n a wizaurddddddddddddd");
		this->dialogues.push_back("hrhbrnbvsabghbgbrvfvjh");
		this->dialogues.push_back("HEWWO????????????????????????????????????????????????????????????");
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
				if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB) && plr->GetCurrentNPC() == NULL)
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
	return this->dialogues[this->dialogueIndex];
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
