#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "KillQuest.h"
#include "GatherQuest.h"
#include "FindQuest.h"

#define QUEST_CONFIG_FILENAME		"Quest_Config.xml"

class Quest;

class QuestManager : public Module
{
public://FUNCTIONS
	QuestManager();
	virtual ~QuestManager() 
	{

	};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void CheckKillQuest(Enemy* e);
	void CheckGatherQuest(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xsplitedEnemy, int xmoney);

private://FUNCTIONS

	void CreateQuestMap(pugi::xml_node& setter);

	void ActivateQuest(int id);
	void DeactivateQuest(int id);
	void CompleteQuest(int id);


	pugi::xml_node LoadQuestConfig(pugi::xml_document&) const;

public://VARIABLES
private://VARIABLES
	Quest*						currentQuest = nullptr;

	std::map<int, Quest*>		questList;
	std::map<int, Quest*>		activeQuest;
	std::map<int, Quest*>		finishedQuest;
};
#endif // __QUESTMANAGER_H__
