#ifndef __KILLQUEST_H__
#define __KILLQUEST_H__

#include "Quest.h"
#include "Enemy.h"

class KillQuest : public Quest
{
public:
	KillQuest() {}
	KillQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle, EnemyClass eType);
	~KillQuest() {}

	void QuestLogic();

public:
	uint16 count;
	uint16 goal;

	EnemyClass eType;
};

#endif //!__KLLQUEST_H__
