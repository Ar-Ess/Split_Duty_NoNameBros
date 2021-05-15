#include "KillQuest.h"

KillQuest::KillQuest(Quest* quest) : Quest(QuestType::KILL)
{
	
}

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
	if (this->enemyDefeated == true)
	{ 
		this->count++;	
		this->enemyDefeated = false;
	}
	if (this->count == this->goal) 
		this->SetCompleted();
	if (this->IsCompleted() == true)
		short int exp = this->reward;
}
