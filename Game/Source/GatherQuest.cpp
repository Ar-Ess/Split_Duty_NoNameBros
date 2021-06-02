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
		int exp = app->scene->player1->exp + this->reward;
		int maxExp = int(floor(1000 * pow((float)app->scene->player1->lvl, 1.25f)));
		bool levelUpBool = false;

		if (exp >= maxExp)
		{
			app->scene->player1->lvl++;
			int diffExp = exp - maxExp;
			levelUpBool = true;
			app->scene->player1->exp = diffExp;
		}

		Player* p = app->scene->player1;
		if (levelUpBool)
		{
			p = app->scene->player1;
			int x = p->lvl;

			p->maxHealth = floor((x / 2) + 20);
			p->strengthStat = floor((x / 3) + 6);
			p->defenseStat = floor((x / 3) + 3);
		}
		else
			p->exp = exp;

		p = nullptr;
	}
}


