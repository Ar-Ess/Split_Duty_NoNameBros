#ifndef __FINDQUEST_H__
#define __FINDQUEST_H__

#include "Quest.h"
#include "NPC.h"

class FindQuest : public Quest
{
public:
	FindQuest() {}

	FindQuest(uint16 id, uint16 reward, uint16 npcID, const char* textDescription, const char* textTitle);
	~FindQuest() {}

	void QuestLogic();

public:
	uint16 npcID;
};
#endif //!__FINDQUEST_H__