#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "Module.h"
#include "Dialogue.h"

#include "List.h"

class DialogueManager : public Module
{
public: //functions

	// Constructor and destructor //
	DialogueManager();

	virtual ~DialogueManager() {}
	////////////////////////////////

	// Flow Functions
	//bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();
	/////////////////////////////////

private: //functions

public: //variables

private: // variables

	//List<Dialogue> dialogues;
};

#endif // __DIALOGUEMANAGER_H__
