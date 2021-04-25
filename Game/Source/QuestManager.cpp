#include "QuestManager.h"

QuestManager::QuestManager()
{
	name.Create("questmanager");
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
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;
	return ret;
}

bool QuestManager::CleanUp()
{
	bool ret = true;
	return ret;
}
