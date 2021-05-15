#include "KillQuest.h"

KillQuest::KillQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle, EnemyClass eType) : Quest(QuestType::KILL)
{
	this->count = NULL;

	this->id = id;
	this->reward = reward;
	this->goal = goal;

	this->textDescription = textDescription;
	this->textTitle = textTitle;

	this->eType = EnemyClass(eType);

}

void KillQuest::QuestLogic()
{
	
}
