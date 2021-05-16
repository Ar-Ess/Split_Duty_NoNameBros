#include "GatherQuest.h"

GatherQuest::GatherQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle, ItemType iType) :
	Quest(QuestType::GATHER)
{
	this->count = NULL;

	this->id = id;
	this->reward = reward;
	this->goal = goal;

	this->textDescription = textDescription;
	this->textTitle = textTitle;
	this->iType = iType;
}

void GatherQuest::QuestLogic()
{
	if (this->itemPicked)
	{
		this->count += this->itemAmount;
		this->itemPicked = false;
		this->itemAmount = NULL;
	}
	if (this->count == this->goal)
	{
		this->SetCompleted();
	}
	if (this->IsCompleted())
	{
		short int exp = this->reward;
	}
}
	
