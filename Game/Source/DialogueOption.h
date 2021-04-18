#ifndef __DIALOGUEOPTION_H__
#define __DIALOGUEOPTION_H__

#include "DialogueNode.h"
#include "GuiButton.h"

enum OptionsPos
{
	NO_POS = -1,
	POS1,
	POS2,
	POS3,
	POS4
};

class DialogueOption : public DialogueNode
{
public:
	//default constructor
	DialogueOption() {};

	DialogueOption(int nextNode, int returnCode, SString text) : nextNodeID(nextNode),
				   text(text), returnCode(returnCode), optionButton(nullptr) {};

	virtual ~DialogueOption() {};

	//Utils
	void OptionPlacingX()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS3)
			this->optionButton->bounds.x = (w / 4) - (this->optionButton->bounds.w / 2);
		else
			this->optionButton->bounds.x = 3 * (w / 4) - (this->optionButton->bounds.w / 2);

		this->optionText->bounds.x = this->optionButton->bounds.x;
	}
	void OptionPlacingY()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS2)
			this->optionButton->bounds.y = 5 * (h / 6) - this->optionButton->bounds.h;
		else
			this->optionButton->bounds.y = 5 * (h / 6) + (this->optionButton->bounds.h / 2);
		
		this->optionText->bounds.y = this->optionButton->bounds.y;
	}

public: 

	int				nextNodeID;
	int				returnCode;

	SString			text;
	GuiString*		optionText = nullptr;
	GuiButton*		optionButton;

	OptionsPos      optPlacing;
	DialogueNode*	nextNode = nullptr;
};

#endif // __DIALOGUEOPTION_H__
