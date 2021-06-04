#ifndef _QUEST_H__
#define _QUEST_H__

#include "Defs.h"
#include "List.h"
#include "GuiString.h"
#include "Scene.h"

class QuestManager;

enum QuestType
{
	NO_TYPE = -1,
	KILL,
	GATHER,
	FIND
};

class Quest
{
public: //FUNCTIONS

	Quest() {}

	Quest(QuestType type) : type(type), isActive(false), isCompleted(false), isPinned(false), enemyDefeated(false), npcFound(false) {}

	virtual ~Quest() {}

	//LOGIC

	virtual void QuestLogic() {};

	void DrawCurrentQuest()
	{
		this->title->bounds.x = 25;
		this->title->bounds.y = 50;
		this->title->Draw();

		this->description->bounds.x = 25;
		this->description->bounds.y = this->title->bounds.y + this->title->bounds.h + 50;
		this->description->Draw();
	}

	//GETTERS
	bool IsActive()
	{
		return this->isActive;
	}
	bool IsCompleted()
	{
		return this->isCompleted;
	}
	bool IsPinned()
	{
		return this->isPinned;
	}

	//SETTERS
	void SetActive()
	{
		this->isActive = true;
	}
	void SetInactive()
	{
		this->isActive = false;
	}
	void SetCompleted()
	{
		this->isCompleted = true;
	}
	void SetPinned()
	{
		this->isPinned = true;
	}

private: //FUNCTIONS

public: //VARIABLES
	friend class Player;

	uint16				id;
	uint16				reward;

	bool				isActive = false;
	bool				isCompleted = false;
	bool				isPinned = false;

	const char*			textDescription;
	const char*			textTitle;

	GuiString*			title;
	GuiString*			description;

	QuestType			type;

	bool				enemyDefeated = false;
	bool				npcFound = false;

	SDL_Texture* texture = nullptr;

};
#endif // !_QUEST_H_