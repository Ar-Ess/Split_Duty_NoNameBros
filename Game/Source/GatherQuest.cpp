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
	switch (iType)
	{
	case LITTLE_BEEF_I:
		items->smallMeatCount;
		break;
	case BIF_BEEF_I:
		items->largeMeatCount;
		break;
	case FEATHER_I:
		items->featherCount;
		break;
	case MANTIS_I:
		items->mantisRodCount;
		break;
	case COINS_I:
		items->moneyCount;
		break;
	case SPLIT_I:
		items->splitedEnemyCount;
		break;
	default:
		break;
	}
}
	
