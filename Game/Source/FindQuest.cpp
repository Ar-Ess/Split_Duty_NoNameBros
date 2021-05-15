#include "FindQuest.h"

FindQuest::FindQuest(uint16 id, uint16 reward, uint16 npcID, const char* textDescription, const char* textTitle) : Quest(QuestType::FIND)
{
	this->id = id;
	this->reward = reward;
	this->npcID = npcID;

	this->textDescription = textDescription;
	this->textTitle = textTitle;
}

void FindQuest::QuestLogic()
{
}
