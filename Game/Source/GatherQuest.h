#ifndef __GATHERQUEST_H__
#define __GATHERQUEST_H__

#include "Quest.h"
#include "Item.h"

class GatherQuest : public Quest
{
public:
	GatherQuest() {}

	GatherQuest(uint16 id, uint16 reward, uint16 goal, const char* textDescription, const char* textTitle);
	~GatherQuest() {}

	void QuestLogic();

public:
	uint16 count;
	uint16 goal;

	Item* itemCopy;
};
#endif //!__GATHERQUEST_H__
