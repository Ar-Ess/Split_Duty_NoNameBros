#include "QuestManager.h"

QuestManager::QuestManager()
{
	name.Create("questmanager");
	currentQuest = nullptr;
}

bool QuestManager::Awake(pugi::xml_node&)
{
	bool ret = true;

	pugi::xml_document qConfigFile;
	pugi::xml_node configQuests;

	configQuests = LoadQuestConfig(qConfigFile);

	//FILL QUEST MAP
	if (!configQuests.empty())
	{
		pugi::xml_node qSetUp = configQuests.child("Quests");
		CreateQuestMap(qSetUp);
	}

	activeQuest.clear();
	finishedQuest.clear();

	return ret;
}

bool QuestManager::Start()
{
	bool ret = true;
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

			//INPUT

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

		Quest* newQuest = new Quest(id, reward, description, title, type);

		questList[id] = newQuest;
	}

}

void QuestManager::ActivateQuest(int id)
{
	questList[id]->SetActive();
	activeQuest[id] = questList[id];
}

void QuestManager::DeactivateQuest(int id)
{
	questList[id]->SetInactive();
	activeQuest.erase(id);
}

void QuestManager::CompleteQuest(int id)
{
	questList[id]->SetCompleted();
	finishedQuest[id] = questList[id];

}

pugi::xml_node QuestManager::LoadQuestConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(QUEST_CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", QUEST_CONFIG_FILENAME, result.description());
	else ret = configFile.child("QuestSettup");

	return ret;
}
