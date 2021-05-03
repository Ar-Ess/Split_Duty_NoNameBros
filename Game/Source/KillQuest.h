#ifndef __KILLQUEST_H__
#define __KILLQUEST_H__

#include "Quest.h"
#include "Enemy.h"

class KillQuest : public Quest
{
public:
	KillQuest() {}
	~KillQuest() {}

public:
	uint16 count;
	uint16 goal;

	Enemy* enemyCopy;
};

#endif //!__KLLQUEST_H__
