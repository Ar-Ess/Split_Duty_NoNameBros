#ifndef _DIALOGUENODE_H__
#define _DIALOGUENODE_H__

#include "Dialogue.h"
#include "SDL/include/SDL_rect.h"

class DialogueNode : public Dialogue
{
public:
	//default constructor
	DialogueNode() {};

	DialogueNode(SString text, int id) : text(text), nodeID(id),
				 optionsActive(false) {};

	virtual ~DialogueNode() {};

	//Utils
	void NodePlacing()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		this->nodeText->bounds.x = (w / 2) - (this->nodeText->bounds.w / 2);
		this->nodeText->bounds.y = 5 * (h / 8);
	}

public:
	int						nodeID;
	bool					optionsActive = true;

	SString					text;
	GuiString*				nodeText = nullptr;

	List<DialogueOption*>	optionsList;
};
#endif // __DIALOGUENODE_H__
