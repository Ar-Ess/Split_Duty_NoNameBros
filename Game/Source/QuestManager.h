#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "Quest.h"

#define QUEST_CONFIG_FILENAME		"Quest_Config.xml"

class QuestManager : public Module
{
public://FUNCTIONS
	QuestManager();
	virtual ~QuestManager() {};

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

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
