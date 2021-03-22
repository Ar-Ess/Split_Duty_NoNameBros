#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "Module.h"
#include "Dialogue.h"
#include "DialogueNode.h"
#include "DialogueOption.h"

#include "Log.h"
#include "List.h"

class DialogueManager : public Module
{
public: //functions
	// Constructor and destructor //
	DialogueManager();
	virtual ~DialogueManager() {}
	////////////////////////////////
	// Flow Functions
	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();
	/////////////////////////////////	
private: //functions
	//////CREATORS//////
	/*Dialogue CreateDialogue(pugi::xml_node&);*/
	DialogueOption* CreateOptions(SString text, int nextNodeId);
	DialogueNode* CreateNode(SString text, int id);

	void CreateDialogue(int dialogueID);

public: //variables

private: // variables

	List<Dialogue*> dialoguesList;
};

#endif // __DIALOGUEMANAGER_H__
