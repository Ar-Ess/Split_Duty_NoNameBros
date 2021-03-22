#ifndef _DIALOGUE_H__
#define _DIALOGUE_H__

#include "List.h"
#include "Sstring.h"

class DialogueNode;
class DialogueOption;

class Dialogue
{
public:
	//default constructor
	Dialogue() {};
	Dialogue(int id) : dialogueID(id) {};
	virtual ~Dialogue() {}

	void AssignOptions();

public: 
	int						dialogueID = NULL;

	List<DialogueNode*>		nodeList;
};

#endif // _DIALOGUE_H__