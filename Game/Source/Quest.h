#ifndef _QUEST_H_
#define _QUEST_H_

enum QuestType
{
	NO_TYPE = -1,
	KILL,
	COLLECT,
	ESCORT,
	DELIVERY
};
class Quest
{
public: //FUNCTIONS

	Quest(QuestType type) : type(type) {}
	virtual ~Quest() {}

private: //FUNCTIONS

public: //VARIABLES
	QuestType		type;
private: //VARIABLES
};
#endif // !_QUEST_H_

