#ifndef _QUEST_H_
#define _QUEST_H_

#include "Defs.h"
#include "List.h"
#include "GuiString.h"

enum QuestType
{
	NO_TYPE = -1,
	KILL,
	GATHER,
	FIND
};
//SDL_Color           black = { 0  , 0  , 0  , 255 };
//int							endline = 0;
enum ItemType;

class QuestManager;

class FindQuest;
class KillQuest;
class GatherQuest;

class Quest
{
public: //FUNCTIONS

	Quest() {}

	Quest(QuestType type) : type(type), trigger(false), isActive(false), isCompleted(false), isPinned(false), enemyDefeated(false), itemPicked(false), itemAmount(0), npcFound(false) {}

	virtual ~Quest() {}

	//LOGIC

	virtual void QuestLogic() {};

	void DrawPinnedQuest()
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
	uint16				id;
	uint16				reward;

	bool				trigger = false;
	bool				isActive = false;
	bool				isCompleted = false;
	bool				isPinned = false;

	const char*			textDescription;
	const char*			textTitle;

	GuiString*			title;
	GuiString*			description;

	QuestType			type;

	bool				enemyDefeated = false;
	bool				itemPicked = false;
	bool				npcFound = false;
	int					itemAmount = 0;
};
#endif // !_QUEST_H_

