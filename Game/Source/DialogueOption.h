#ifndef __DIALOGUEOPTION_H__
#define __DIALOGUEOPTION_H__

#include "DialogueNode.h"

class DialogueOption : public DialogueNode
{
public:
	//default constructor
	DialogueOption() {};

	DialogueOption(int nextNode, SString text) : nextNodeID(nextNode), optionText(text) {};

	virtual ~DialogueOption() {};

public: 

	int				nextNodeID;
	
	SString			optionText;	
	DialogueNode*	nextNode = nullptr;
};

#endif // __DIALOGUEOPTION_H__
