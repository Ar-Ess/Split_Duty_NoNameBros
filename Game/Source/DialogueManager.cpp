#include "DialogueManager.h"

DialogueManager::DialogueManager()
{
	name.Create("dialoguemanager");
	dialoguesList.Clear();
}

bool DialogueManager::Awake(pugi::xml_node&)
{
	bool ret = true;
	// HARDCODED DIALOGUE
	CreateDialogue(1);

	return ret;
}

bool DialogueManager::Start()
{	
	return true;
}

bool DialogueManager::PreUpdate()
{
	return true;
}

bool DialogueManager::Update()
{
	return true;
}

bool DialogueManager::PostUpdate()
{
	return true;
}

bool DialogueManager::CleanUp()
{
	return true;
}

DialogueOption* DialogueManager::CreateOptions(SString text, int nextNodeId)
{
	DialogueOption* option = new DialogueOption(nextNodeId, text);	
	LOG("OPTION nextnodeID %d and text: %s", nextNodeId, text);

	return option;
}

DialogueNode* DialogueManager::CreateNode(SString text, int nodeId)
{
	DialogueNode* node = new DialogueNode(text, nodeId);
	LOG("NODE %d and text: %s", nodeId, text);

	DialogueOption* option1 = nullptr;
	DialogueOption* option2 = nullptr;

	switch (nodeId)
	{
	case 0:
		option1 = CreateOptions("for what?", 1);
		node->optionsList.Add(option1);		

		option2 = CreateOptions("shut up!", 2);
		node->optionsList.Add(option2);
		break;
	case 1:
		option1 = CreateOptions("And why are you doing this?", 3);
		node->optionsList.Add(option1);

		option2 = CreateOptions("Right, that's cool", 4);
		node->optionsList.Add(option2);
		break;
	default:
		int i = 0;
		break;
	}	
	return node;
}

void DialogueManager::CreateDialogue(int dialogueID)
{
	Dialogue* dialogue = new Dialogue(dialogueID);	

	LOG("MY DIALOGUE TREE NUMBER %d", dialogueID);

	dialogue->nodeList.Add(CreateNode("this is just a dummy dialogue", 0));
	dialogue->nodeList.Add(CreateNode("for testing purposes", 1));
	dialogue->nodeList.Add(CreateNode("ok. dw", 2));
	dialogue->nodeList.Add(CreateNode("for my project", 3));
	dialogue->nodeList.Add(CreateNode("nice", 4));

	dialogue->AssignOptions();

	dialoguesList.Add(dialogue);
}

