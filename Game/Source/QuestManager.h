#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "List.h"

#include "Quest.h"

class QuestManager : public Module
{
public://FUNCTIONS
	QuestManager()
	{
		name.Create("questmanager");
	}

	virtual ~QuestManager() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

private://FUNCTIONS

public://VARIABLES

private://VARIABLES

	Quest*				currentQuest;

	//List<Quest*>		questList;
	//Map<Quest*>		questMap;
};
#endif // __QUESTMANAGER_H__
