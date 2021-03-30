#include "DialogueManager.h"
#include "GuiButton.h"
#include "Textures.h"
#include "GuiManager.h"


DialogueManager::DialogueManager()
{
	name.Create("dialoguemanager");
	active = true;
	currentDialogue = nullptr;
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

bool DialogueManager::Update(float dt)
{
	bool ret = true;
	if (currentDialogue != nullptr)
	{
		Draw();
		Input();
	}
	return ret;
}

bool DialogueManager::CleanUp()
{
	for (ListItem<Dialogue*>* dialogueItem = dialoguesList.start;
		dialogueItem != nullptr; dialogueItem = dialogueItem->next)
	{
		for (ListItem<DialogueNode*>* nodeItem = dialogueItem->data->nodeList.start;
			nodeItem != nullptr; nodeItem = nodeItem->next)
		{
			if (nodeItem->data->optionsList.Count() != 0)
			{
				nodeItem->data->optionsList.Clear();
			}
			else continue;
		}
		dialogueItem->data->nodeList.Clear();
	}
	dialoguesList.Clear();
	return true;
}

DialogueOption* DialogueManager::CreateOptions(SString text, int nextNodeId, int retCode)
{
	DialogueOption* option = new DialogueOption(nextNodeId, retCode, text.GetString());
	LOG("OPTION nextnodeID %d and text: %s", nextNodeId, text.GetString());

	return option;
}

DialogueNode* DialogueManager::CreateNode(SString text, int nodeId)
{
	DialogueNode* node = new DialogueNode(text, nodeId);
	LOG("NODE %d and text: %s", nodeId, text.GetString());

	DialogueOption* option1 = nullptr;
	DialogueOption* option2 = nullptr;

	switch (nodeId)
	{
	case 0:
		option1 = CreateOptions("for what?", 1, 1);
		node->optionsList.Add(option1);		

		option2 = CreateOptions("shut up!", 2, 1);
		node->optionsList.Add(option2);
		break;
	case 1:
		option1 = CreateOptions("And why are you doing this?", 3, 1);
		node->optionsList.Add(option1);

		option2 = CreateOptions("Right, that's cool", 4, 1);
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
	dialogue->currentNode = dialogue->nodeList.start->data;

	dialoguesList.Add(dialogue);
}

void DialogueManager::EndDialogue()
{
	if (currentDialogue != nullptr)
	{
		for (ListItem<DialogueNode*>* itemNode = currentDialogue->nodeList.start;
			itemNode != nullptr; itemNode = itemNode->next)
		{
			if (itemNode->data->optionsList.Count() != 0)
			{
				for (ListItem<DialogueOption*>* itemOption = itemNode->data->optionsList.start;
					itemOption != nullptr; itemOption = itemOption->next)
				{
					app->tex->UnLoad(itemOption->data->optionTexture);
					if (itemOption->data->optionTexture != nullptr) LOG("OPTION TEXTURE NOT DELETED");
				}
			}		
			app->tex->UnLoad(itemNode->data->nodeTexture);
			if (itemNode->data->nodeTexture != nullptr) LOG("NODE TEXTURE NOT DELETED");
		}
	}
	currentDialogue->currentNode = currentDialogue->nodeList.start->data;
	currentDialogue = nullptr;
}

void DialogueManager::StartDialogue(int dialogueID)
{
	if (currentDialogue == nullptr)
	{
		currentDialogue = dialoguesList.At(--dialogueID)->data;
		currentDialogue->currentNode = currentDialogue->nodeList.start->data;

		for (ListItem<DialogueNode*>* itemNode = currentDialogue->nodeList.start;
			itemNode != nullptr; itemNode = itemNode->next)
		{
			itemNode->data->nodeTexture = app->fontTTF->Print(itemNode->data->nodeText.GetString(), white,
										  app->fontTTF->defaultFont);

			if (itemNode->data->nodeTexture == nullptr) LOG("NODE TEXTURE NOT LOADED");

			if (itemNode->data->optionsList.Count() != 0)
			{
				for (ListItem<DialogueOption*>* itemOption = itemNode->data->optionsList.start;
					itemOption != nullptr; itemOption = itemOption->next)
				{
					itemOption->data->optionTexture = app->fontTTF->Print(itemOption->data->optionText.GetString(), 
													  white, app->fontTTF->defaultFont);

					if (itemOption->data->optionTexture == nullptr) LOG("OPTION TEXTURE NOT LOADED");
				}
			}
			else continue;
		}
	}
}

void DialogueManager::Draw()
{
	SDL_Rect tempRect{ 0,0,0,0 };
	iPoint nodePos{ 100,100 };
	int offset = 15;

	app->fontTTF->CalcSize(currentDialogue->currentNode->nodeText.GetString(),
						   tempRect.w, tempRect.h, app->fontTTF->defaultFont);

	SDL_Rect nodeChart{ nodePos.x,nodePos.y, tempRect.w + (2 * offset), tempRect.h + (2 * offset) };

	//DRAWING NODE
	app->render->DrawRectangle(nodeChart, red);
	app->render->DrawTexture(currentDialogue->currentNode->nodeTexture,
							 nodeChart.x + offset, nodeChart.y + offset, &tempRect);

	if (currentDialogue->currentNode->optionsList.Count() != 0)
	{
		currentDialogue->currentNode->optionsActive = true;
		int i = 0;
		iPoint optionPos{ 100, 150 };

		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			itemOption->data->optionButton = new GuiButton(itemOption->data->nodeID, { 0,0,0,0 }, 
														   itemOption->data->optionText.GetString());
			//DRAWING OPTIONS
			app->fontTTF->CalcSize(itemOption->data->optionText.GetString(), itemOption->data->optionButton->bounds.w, 
								   itemOption->data->optionButton->bounds.h, app->fontTTF->defaultFont);

			itemOption->data->optionButton->SetObserver(this);

			SDL_Rect optionChart{ 0,0,itemOption->data->optionButton->bounds.w, itemOption->data->optionButton->bounds.h };

			switch (i)
			{
			case 0:
				itemOption->data->optionButton->bounds.x = optionPos.x = 100;
				itemOption->data->optionButton->bounds.y = optionPos.y = 150;
				itemOption->data->optionButton->bounds.w += 15;
				itemOption->data->optionButton->bounds.h += 15; 

				itemOption->data->optionButton->Draw();
				app->render->DrawTexture(itemOption->data->optionTexture, (optionPos.x + offset), (optionPos.y + offset),
					                     &optionChart);
				break;
			case 1:
				itemOption->data->optionButton->bounds.x = optionPos.x = 100;
				itemOption->data->optionButton->bounds.y = optionPos.y = 200;
				itemOption->data->optionButton->bounds.w += 15;
				itemOption->data->optionButton->bounds.h += 15;

				itemOption->data->optionButton->Draw();
				app->render->DrawTexture(itemOption->data->optionTexture, (optionPos.x + offset), (optionPos.y + offset),
										 &optionChart);
				break;
			default:
				break;
			}
			i++;
		}
	}
}

void DialogueManager::Input()
{
	if (currentDialogue != nullptr && currentDialogue->currentNode->optionsActive)
	{
		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			itemOption->data->optionButton->Update(.0f);
		}		
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) EndDialogue();
}

bool DialogueManager::OnGuiMouseClickEvent(GuiControl* option)
{
	bool ret = true;

	currentDialogue->currentNode->optionsActive = false;

	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start; 
		 itemOption != nullptr; itemOption = itemOption->next)
	{
		if (strcmp(option->text.GetString(), itemOption->data->optionText.GetString()) == 0)
		{
			if(itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 1)
			{
				currentDialogue->currentNode = itemOption->data->nextNode;
			}
			else if (itemOption->data->returnCode == 0)
			{
				//node with continue option
				//==> go to next node
			}
			else if (itemOption->data->returnCode == -1)
			{
				EndDialogue();
			}
		}

		//DELETE GUI OPTION BUTTONS HERE
		//INITIALIZE BUTTON? THEN RELEASE?
		app->guiManager->DestroyGuiControl(itemOption->data->optionButton);
		if (itemOption->data->optionButton != nullptr) LOG("OPTION BUTTON NOT DELETED");
	}

	return ret;
}

