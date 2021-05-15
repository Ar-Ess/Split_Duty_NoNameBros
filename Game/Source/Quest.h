#ifndef _QUEST_H_
#define _QUEST_H_

#include "Defs.h"
#include "List.h"
#include "GuiString.h"

//class SubQuest;

enum QuestType
{
	NO_TYPE = -1,
	KILL,
	GATHER,
	FIND
};

enum ItemType;

class QuestManager;
class Quest
{
public: //FUNCTIONS

	Quest() {}

	Quest(QuestType type) : type(type), trigger(false), isActive(false), isCompleted(false), isPinned(false), enemyDefeated(false) {}

	virtual ~Quest() {}

	//LOGIC

	virtual void QuestLogic() {};

	void DrawPinnedQuest()
	{

	}

	void TriggerQuest()
	{
		if(trigger == false)
			this->trigger = true;

		this->SetActive();
		this->SetPinned();
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
	bool				enemyDefeated = false;

	const char*			textDescription;
	const char*			textTitle;

	GuiString			title;
	GuiString			description;

	QuestType			type;
};
#endif // !_QUEST_H_

