#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "GuiManager.h"
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
	void CheckGatherQuest(ItemType type, int amount);
	void CheckFindQuest(int npcID);

	void ActivateQuest(int id);
	void DeactivateQuest(int id);
	void CompleteQuest(int id);

private://FUNCTIONS

	void CreateQuestMap(pugi::xml_node& setter);
	pugi::xml_node LoadQuestConfig(pugi::xml_document&) const;

public://VARIABLES
private://VARIABLES
	Quest*						currentQuest = nullptr;
	int							endline = 0;
	int							offset = 15;

	SDL_Color           black = { 0  , 0  , 0  , 255 };

	std::map<int, Quest*>		questList;
	std::map<int, Quest*>		activeQuest;
	std::map<int, Quest*>		finishedQuest;
};
#endif // __QUESTMANAGER_H__
