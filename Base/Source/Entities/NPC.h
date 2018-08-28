#pragma once
#include "Vector3.h"
#include "Entity.h"
#include "../PlayerInfo/PlayerInfo.h"
#include <string>

class CNPC : public CEntity
{
public:
	enum NPC_TYPE {
		NPC_WIZARD,
		NPC_WIZARDBASIC,
		NPC_WIZARDLEVEL2,
		NPC_WIZARDLEVEL3,
		NPC_WIZARDBOSS
	};

	CNPC(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1), Vector3 target = Vector3(0, 0, 1));
	CNPC(NPC_TYPE npctype, Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1), Vector3 target = Vector3(0, 0, 1));
	~CNPC();

	void Init();
	void Update(double dt);

	NPC_TYPE getType(void) { return this->type; }
	void setNPCType(NPC_TYPE type) { this->type = type; }
	//void setState(NPC_TYPE s) { state = s; }
	//void tempMoveBack(float dt);
	string getCurrentLine();
	void setPlayerRef(CPlayerInfo* playerRef);
	void goToNextDialogue();
	bool isDoneWithLine();
	bool finishedDialogue();
	void ResetDialogue();
	void UpdateDialogue(double dt);
private:
	NPC_TYPE type;
	CPlayerInfo* playerRef;
	vector<string> dialogues;
	int charIndex;//how many chars to show for the current line
	int dialogueIndex;//the current line of dialogue
};

