#ifndef __QUESTMANAGER_H_
#define __QUESTMANAGER_H_

#include "App.h"
#include "Module.h"
#include "Log.h"
#include "GuiManager.h"
#include "KillQuest.h"
#include "GatherQuest.h"
#include "FindQuest.h"

#define QUEST_CONFIG_FILENAME		"Quest_Config.xml"

class Quest;
enum ItemType;
class Scene;

class QuestManager : public Module
{
public://FUNCTIONS
	QuestManager();
	virtual ~QuestManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void CheckKillQuest(Enemy* e);
	void CheckFindQuest(int npcID);

	void ActivateQuest(int id);
	void DeactivateQuest(int id);
	void CompleteQuest(int id);

	Quest* GetCurrentQuest() {
		return currentQuest;
	}

private://FUNCTIONS
	friend class Scene;
	friend class Inventory;
	friend class App;

	void CreateQuestMap(pugi::xml_node& setter);
	pugi::xml_node LoadQuestConfig(pugi::xml_document&) const;

public://VARIABLES
private://VARIABLES
	Quest* currentQuest = nullptr;
	int							endline = 0;
	int							offset = 15;

	SDL_Color					black = { 0  , 0  , 0  , 255 };

	std::map<int, Quest*>		questList;
	std::map<int, Quest*>		finishedQuest;

	SDL_Texture* questTexture = nullptr;
};
#endif // __QUESTMANAGER_H__
