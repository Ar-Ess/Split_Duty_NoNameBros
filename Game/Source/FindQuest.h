#ifndef __FINDQUEST_H__
#define __FINDQUEST_H__

#include "Quest.h"
#include "NPC.h"

class FindQuest : public Quest
{
public:
	FindQuest() {}
	~FindQuest() {}

public:
	NPC* npcCopy;
};
#endif //!__FINDQUEST_H__
