#include "QuestManager.h"

QuestManager::QuestManager()
{
	name.Create("questmanager");
	currentQuest = nullptr;
	endline = 200;
}

bool QuestManager::Awake(pugi::xml_node&)
{
	bool ret = true;
	return ret;
}

bool QuestManager::Start()
{
	bool ret = true;
	pugi::xml_document qConfigFile;
	pugi::xml_node configQuests;

	configQuests = LoadQuestConfig(qConfigFile);
	//FILL QUEST MAP
	if (!configQuests.empty())
	{
		questList.clear();
		pugi::xml_node qSetUp = configQuests.child("Quests");
		CreateQuestMap(qSetUp);
	}

	activeQuest.clear();
	finishedQuest.clear();
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;
	if (!activeQuest.empty())
	{
 		for (std::map<int, Quest*>::iterator it = activeQuest.begin(); it != activeQuest.end(); ++it)
		{
			//LOGIC
			it->second->QuestLogic();

			if (it->second->IsCompleted() == true)
				CompleteQuest(it->second->id);
			if (it->second->IsActive() == false)
				DeactivateQuest(it->second->id);
			
			//DRAW INFO
			if(it->second->IsPinned())
 				it->second->DrawPinnedQuest();
		}
	}
	return ret;
}

bool QuestManager::CleanUp()
{
	bool ret = true;

	activeQuest.clear();
	questList.clear();
	finishedQuest.clear();

	if (!activeQuest.empty() || !finishedQuest.empty() || !questList.empty())
		ret = false;

	activeQuest.~map();
	questList.~map();
	finishedQuest.~map();

	return true;
}

void QuestManager::CreateQuestMap(pugi::xml_node& setter)
{
	for (pugi::xml_node qst = setter.child("Quest"); !qst.empty(); qst = qst.next_sibling("Quest"))
	{
		uint16 id = qst.attribute("ID").as_uint();
		uint16 reward = qst.attribute("reward").as_uint();
		QuestType type = QuestType(qst.attribute("type").as_int());

		const char* title = qst.attribute("title").as_string();
		const char* description = qst.child_value();

		uint16 goal = 0;
		uint16 enemy = 0;
		uint16 npc = 0;
		uint16 item = 0;

		KillQuest* newKillQuest = nullptr;
		GatherQuest* newGathQuest = nullptr;
		FindQuest* newFindQuest = nullptr;

		switch (type)
		{
		case KILL:
			goal = qst.attribute("goal").as_uint();
			enemy = qst.attribute("enemy").as_uint();
			newKillQuest = new KillQuest(id, reward, goal, description, title, EnemyClass(enemy));
			questList[id] = newKillQuest;
			break;
		case GATHER:
			goal = qst.attribute("goal").as_uint();
			item = qst.attribute("item").as_uint();
			newGathQuest = new GatherQuest(id, reward, goal, description, title, ItemType(item));
			questList[id] = newGathQuest;
			break;
		case FIND:
			npc = qst.attribute("npcId").as_uint();
			newFindQuest = new FindQuest(id, reward, npc, description, title);
			questList[id] = newFindQuest;
			break;
		}

		questList[id]->title = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		questList[id]->title->SetString(title, black, endline);
		questList[id]->title->SetTextFont(app->fontTTF->defaultFont2);
		app->fontTTF->CalcSize(questList[id]->textTitle, questList[id]->title->bounds.w, questList[id]->title->bounds.h, app->fontTTF->defaultFont);

		if (questList[id]->title->bounds.w >= endline)
		{
			int lineScale = static_cast<int>(ceil(questList[id]->title->bounds.w / endline));
			questList[id]->title->bounds.h *= lineScale;
		}
		else
		{
			questList[id]->title->bounds.h -= (offset + 5);
		}
		questList[id]->title->bounds.w = endline;

		questList[id]->description = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		questList[id]->description->SetString(description, black, endline);
		questList[id]->description->SetTextFont(app->fontTTF->defaultFont2);
		app->fontTTF->CalcSize(questList[id]->textDescription, questList[id]->description->bounds.w, questList[id]->description->bounds.h, app->fontTTF->defaultFont);

		if (questList[id]->description->bounds.w >= endline)
		{
			int lineScale = static_cast<int>(ceil(questList[id]->description->bounds.w / endline));
			questList[id]->description->bounds.h *= lineScale;
		}
		else
		{
			questList[id]->description->bounds.h -= (offset + 5);
		}
		questList[id]->description->bounds.w = endline;
	}
}

void QuestManager::ActivateQuest(int id)
{
 	questList[id]->SetActive();
	activeQuest[id] = questList[id];
	questList.erase(id);
	activeQuest[id]->SetPinned();

	/*activeQuest[id]->title = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
	activeQuest[id]->title->SetString(activeQuest[id]->textTitle, black, endline);
	activeQuest[id]->title->SetTextFont(app->fontTTF->defaultFont2);
	app->fontTTF->CalcSize(activeQuest[id]->textTitle, activeQuest[id]->title->bounds.w, activeQuest[id]->title->bounds.h, app->fontTTF->defaultFont);

	if (activeQuest[id]->title->bounds.w >= endline)
	{
		int lineScale = static_cast<int>(ceil(activeQuest[id]->title->bounds.w / endline));
		activeQuest[id]->title->bounds.h *= lineScale;
	}
	else
	{
		activeQuest[id]->title->bounds.h -= (offset + 5);
	}
	activeQuest[id]->title->bounds.w = endline;

	activeQuest[id]->description = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
	activeQuest[id]->description->SetString(activeQuest[id]->textDescription, black, endline);
	activeQuest[id]->description->SetTextFont(app->fontTTF->defaultFont2);
	app->fontTTF->CalcSize(activeQuest[id]->textDescription, activeQuest[id]->description->bounds.w, activeQuest[id]->description->bounds.h, app->fontTTF->defaultFont);

	if (activeQuest[id]->description->bounds.w >= endline)
	{
		int lineScale = static_cast<int>(ceil(activeQuest[id]->description->bounds.w / endline));
		activeQuest[id]->description->bounds.h *= lineScale;
	}
	else
	{
		activeQuest[id]->description->bounds.h -= (offset + 5);
	}
	activeQuest[id]->description->bounds.w = endline;*/

}

void QuestManager::DeactivateQuest(int id)
{
	questList[id] = activeQuest[id];
	questList[id]->SetInactive();
	activeQuest.erase(id);
}

void QuestManager::CompleteQuest(int id)
{
	finishedQuest[id] = activeQuest[id];
	activeQuest.erase(id);
}

void QuestManager::CheckKillQuest(Enemy* e)
{
	for (std::map<int, Quest*>::iterator it = activeQuest.begin(); it != activeQuest.end(); ++it)
	{
		if (it->second->type == KILL)
		{			
			if (static_cast<KillQuest*>(it->second)->eType == e->enemyClass)
			{
				it->second->enemyDefeated == true;
				return;
			}
		}
		else continue;
	}
}

void QuestManager::CheckGatherQuest(ItemType type, int amount)
{
	for (std::map<int, Quest*>::iterator it = activeQuest.begin(); it != activeQuest.end(); ++it)
	{
		if (it->second->type == GATHER)
		{
			if (static_cast<GatherQuest*>(it->second)->iType == type)
			{
				it->second->itemPicked = true;
				it->second->itemAmount = amount;
				return;
			}
			else continue;
		}		
	}
}

void QuestManager::CheckFindQuest(int npcID)
{
	for (std::map<int, Quest*>::iterator it = activeQuest.begin(); it != activeQuest.end(); ++it)
	{
		if (it->second->type == FIND)
		{
			if (static_cast<FindQuest*>(it->second)->npcID == npcID)
				it->second->npcFound = true;
		}
	}
}

pugi::xml_node QuestManager::LoadQuestConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(QUEST_CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", QUEST_CONFIG_FILENAME, result.description());
	else ret = configFile.child("QuestSettup");

	return ret;
}
