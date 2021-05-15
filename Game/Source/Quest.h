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

//enum SubQuestType
//{
//	NO_ACTION = -1,
//	KILL_ACTION,
//	COLLECT_ACTION,
//	FIND_ACTION,
//};

class Quest
{
public: //FUNCTIONS

	Quest() {}

	Quest(QuestType type) : type(type), trigger(false), isActive(false), isCompleted(false), isPinned(false) {}

	//Quest(uint16 id, uint16 reward, const char* textDescription, const char* textTitle, QuestType type);

	virtual ~Quest() {}

	//LOGIC

	virtual void QuestLogic() {};

	void DrawPinnedQuest()
	{

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

	GuiString			title;
	GuiString			description;

	QuestType			type;
};
	//SubQuest*			currentEvent = nullptr;
	//
	//List<SubQuest*>		eventList;

//class SubQuest : public Quest
//{
//public: //FUCNTIONS
//
//	SubQuest(uint16 id) :
//				id(id) {}
//	virtual ~SubQuest() {}
//
//private: //FUNCTIONS
//
//public: //VARIABLES
//
//private: //VARIABLES
//
//	uint16				id;
//	//SubQuestType		type;
//
//	const char*			textDescription;
//
//	GuiString			description;
//
//	SubQuest*			nextEvent = nullptr;
//};
#endif // !_QUEST_H_

