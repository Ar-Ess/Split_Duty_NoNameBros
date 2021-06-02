#include "GatherQuest.h"
#include "Player.h"

GatherQuest::GatherQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle, ItemType iType) :
	Quest(QuestType::GATHER)
{
	this->id = id;
	this->reward = reward;
	this->goal = goal;

	this->textDescription = textDescription;
	this->textTitle = textTitle;
	this->iType = iType;
}

void GatherQuest::QuestLogic()
{
	bool done = false;
	switch (iType)
	{
	case ItemType::LITTLE_BEEF_I:
		done = (app->scene->player1->smallMeatCount >= this->goal);
		break;
	case ItemType::BIF_BEEF_I:
		done = (app->scene->player1->largeMeatCount >= this->goal);
		break;
	case ItemType::FEATHER_I:
		done = (app->scene->player1->featherCount >= this->goal);
		break;
	case ItemType::MANTIS_I:
		done = (app->scene->player1->mantisRodCount >= this->goal);
		break;
	case ItemType::COINS_I:
		done = (app->scene->player1->moneyCount >= this->goal);
		break;
	case ItemType::SPLIT_I:
		done = (app->scene->player1->splitedEnemyCount >= this->goal);
		break;
	}
	if (done)
	{
		this->SetCompleted();
	}
	if (this->IsCompleted())
	{
		app->scene->player1->moneyCount += reward;
	}
}


