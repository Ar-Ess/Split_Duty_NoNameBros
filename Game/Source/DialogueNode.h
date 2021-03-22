#ifndef _DIALOGUENODE_H__
#define _DIALOGUENODE_H__

#include "Dialogue.h"

class DialogueNode : public Dialogue
{
public:
	//default constructor
	DialogueNode() {};

	DialogueNode(SString text, int id) : nodeText(text), nodeID(id) {};

	virtual ~DialogueNode() {};

public:
	int						nodeID;
		
	SString					nodeText;
	List<DialogueOption*>	optionsList;
};
#endif // __DIALOGUENODE_H__
