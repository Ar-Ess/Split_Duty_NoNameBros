#ifndef _DIALOGUENODE_H__
#define _DIALOGUENODE_H__

#include "Dialogue.h"
#include "SDL/include/SDL_rect.h"

class DialogueNode : public Dialogue
{
public:
	//default constructor
	DialogueNode() {};

	DialogueNode(SString text, int id) : nodeText(text), nodeID(id),
				 optionsActive(false), nodeTexture(nullptr) {};

	virtual ~DialogueNode() {};

	//Utils
	void NodePlacing()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		this->nodePos.x = (w / 2) - (this->nodeRect.w / 2);
		this->nodePos.y = 4 * (h / 6);
	}

public:
	int						nodeID;
	bool					optionsActive = true;
		
	SString					nodeText;
	List<DialogueOption*>	optionsList;

	iPoint					nodePos{ 0,0 };

	SDL_Rect				nodeRect{ 0,0,0,0 };
	SDL_Texture*			nodeTexture;
};
#endif // __DIALOGUENODE_H__
