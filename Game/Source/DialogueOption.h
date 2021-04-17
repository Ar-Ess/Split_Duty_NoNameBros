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
				   optionText(text), returnCode(returnCode), optionTexture(nullptr), optionButton(nullptr) {};

	virtual ~DialogueOption() {};

	//Utils
	void OptionPlacingX()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS3)
		{
			this->optionPos.x = (w / 4) - (this->optionButton->bounds.w / 2);
		}
		else
		{
			this->optionPos.x = 3 * (w / 4) - (this->optionButton->bounds.w / 2);
		}
	}
	void OptionPlacingY()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS2)
		{
			this->optionPos.y = 5 * (h / 6) - this->optionButton->bounds.h;
		}
		else
		{
			this->optionPos.y = 5 * (h / 6) + (this->optionButton->bounds.h / 2);
		}
	}

public: 

	int				nextNodeID;
	int				returnCode;

	SString			optionText;	

	GuiButton*		optionButton;
	SDL_Texture*	optionTexture;

	iPoint			optionPos{ 0,0 };
	SDL_Rect		optionRect{ 0,0,0,0 };

	OptionsPos      optPlacing;

	DialogueNode*	nextNode = nullptr;
};

#endif // __DIALOGUEOPTION_H__
