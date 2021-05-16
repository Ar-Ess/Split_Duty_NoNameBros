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
	if (this->enemyDefeated == true)
	{ 
		this->count++;	
		this->enemyDefeated = false;
	}
	if (this->count == this->goal)
	{
		this->SetCompleted();
	}
	if (this->IsCompleted() == true)
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
