#include "QuestManager.h"
#include "World.h"

QuestManager::QuestManager()
{
	name.Create("questmanager");
	currentQuest = nullptr;
	endline = 200;
}

QuestManager::~QuestManager()
{
	if (currentQuest != nullptr)
		currentQuest = nullptr;
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
	questTexture = app->tex->Load("Assets/Textures/UI/dialogSquare.png");

	finishedQuest.clear();
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;
	if (currentQuest != nullptr)
	{
		//LOGIC
		currentQuest->QuestLogic();

		if (currentQuest->IsCompleted())
			CompleteQuest(currentQuest->id);
	}
	return ret;
}

bool QuestManager::CleanUp()
{
	bool ret = true;
	questList.clear();
	finishedQuest.clear();

	if (!finishedQuest.empty() || !questList.empty())
		ret = false;

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
	if (questList[id] != nullptr)
	{
		if (currentQuest != nullptr)
		{
			DeactivateQuest(currentQuest->id);
		}
		questList[id]->SetActive();
		currentQuest = questList[id];
		questList.erase(id);
		currentQuest->SetPinned();
	}
}

void QuestManager::DeactivateQuest(int id)
{
	questList[id] = currentQuest;
	questList[id]->SetInactive();
	currentQuest = nullptr;
}

void QuestManager::CompleteQuest(int id)
{
	finishedQuest[id] = currentQuest;
	currentQuest = nullptr;
}

void QuestManager::CheckKillQuest(Enemy* e)
{
	if (static_cast<KillQuest*>(currentQuest)->eType == e->enemyClass)
	{
		currentQuest->enemyDefeated == true;
		return;
	}
}

void QuestManager::CheckFindQuest(int npcID)
{
	if (static_cast<FindQuest*>(currentQuest)->npcID == npcID)
		currentQuest->npcFound = true;
}

pugi::xml_node QuestManager::LoadQuestConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(QUEST_CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", QUEST_CONFIG_FILENAME, result.description());
	else ret = configFile.child("QuestSettup");

	return ret;
}
