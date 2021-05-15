#include "GatherQuest.h"

GatherQuest::GatherQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle) : 
	Quest(QuestType::GATHER)
{
	this->count = NULL;

	this->id = id;
	this->reward = reward;
	this->goal = goal;

	this->textDescription = textDescription;
	this->textTitle = textTitle;

}

void GatherQuest::QuestLogic()
{

}
