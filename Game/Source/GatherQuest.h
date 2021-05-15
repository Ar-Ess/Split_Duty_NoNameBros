#ifndef __GATHERQUEST_H__
#define __GATHERQUEST_H__

#include "Quest.h"
#include "Player.h"

class GatherQuest : public Quest
{
public:
	GatherQuest() {}

	GatherQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle, ItemType iType);
	~GatherQuest() {}

	void QuestLogic();

public:
	uint16 count;
	uint16 goal;

	ItemType iType;
	Player* items;
};
#endif //!__GATHERQUEST_H__
