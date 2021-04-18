#include "DialogueManager.h"
#include "GuiButton.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Combat.h"
#include "Player.h"


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
	pugi::xml_document dConfigFile;
	pugi::xml_node configDial;

	configDial = LoadDialogueConfig(dConfigFile);

	const char* path = configDial.child("DialogueTextures").attribute("file").as_string();
	dialogueTexture = app->tex->Load(path);

	if (configDial.empty() == false)
	{
		pugi::xml_node dSetUp = configDial.child("Dialogs");
		CreateDialogue(dSetUp);
	}
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

DialogueOption* DialogueManager::CreateOptions(pugi::xml_node& setter)
{	
	DialogueOption* option = new DialogueOption(setter.attribute("NextNode").as_int(),
		setter.attribute("ReturnCode").as_int(),SString(setter.child_value()));

	return option;
}

DialogueNode* DialogueManager::CreateNode(pugi::xml_node& setter)
{
	pugi::xml_node nd = setter.child("NPCTalk");

	DialogueNode* node = new DialogueNode(SString(nd.child_value()), setter.attribute("ID").as_int());

	for (pugi::xml_node optionSetter = setter.child("Option"); optionSetter != nullptr;
		optionSetter = optionSetter.next_sibling("Option"))
	{	
		node->optionsList.Add(CreateOptions(optionSetter));
	}
	return node;
}

void DialogueManager::CreateDialogue(pugi::xml_node& setter)
{
	for (pugi::xml_node dlg = setter.child("Dialog"); dlg != nullptr; dlg = dlg.next_sibling("Dialog"))
	{	
		Dialogue* dialogue = new Dialogue(dlg.attribute("ID").as_int());	

		for (pugi::xml_node nodeSetter = dlg.child("Node"); nodeSetter != nullptr; 
			 nodeSetter = nodeSetter.next_sibling("Node"))
		{
			dialogue->nodeList.Add(CreateNode(nodeSetter));
		}
		dialogue->AssignOptions();
		dialogue->currentNode = dialogue->nodeList.start->data;
	
		dialoguesList.Add(dialogue);
	}
}

void DialogueManager::EndDialogue()
{
	if (currentDialogue != nullptr)
	{
		currentDialogue->currentNode = currentDialogue->nodeList.start->data;
		for (ListItem<DialogueNode*>* itemNode = currentDialogue->nodeList.start;
			itemNode != nullptr; itemNode = itemNode->next)
		{
			if (itemNode->data->optionsList.Count() != 0)
			{
				DeleteOptions();
			}		
			itemNode->data->nodeText->text.Clear();
			itemNode->data->nodeText->UnLoadTextTexture();
			itemNode->data->nodeText = nullptr;
		}
		currentDialogue->currentNode->optionsActive = false;
		currentDialogue = nullptr;

		onDialog = false;
	}
}

void DialogueManager::DeleteOptionButtons()
{
	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
		itemOption != nullptr; itemOption = itemOption->next)
	{
		if (itemOption->data->optionButton != nullptr)
		{
			itemOption->data->optionButton->text.Clear();
			itemOption->data->optionButton = nullptr;
		}
	}
}

void DialogueManager::DeleteOptions()
{
	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
		itemOption != nullptr; itemOption = itemOption->next)
	{
		if (itemOption->data->optionButton != nullptr)
		{
			itemOption->data->optionButton->text.Clear();
			itemOption->data->optionButton = nullptr;
		}
		if (itemOption->data->optionText != nullptr)
		{
			itemOption->data->optionText->text.Clear();
			itemOption->data->optionText->UnLoadTextTexture();
			itemOption->data->optionText = nullptr;
		}
	}
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
			itemNode->data->nodeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
			itemNode->data->nodeText->SetString(itemNode->data->text.GetString());
			itemNode->data->nodeText->bounds = { 0,0,0,0 };
			itemNode->data->nodeText->SetTextFont(app->fontTTF->defaultFont);

			app->fontTTF->CalcSize(itemNode->data->nodeText->text.GetString(), itemNode->data->nodeText->bounds.w,
				itemNode->data->nodeText->bounds.h, app->fontTTF->defaultFont);

			int lineScale = static_cast<int>(ceil(itemNode->data->nodeText->bounds.w / endLine));

			itemNode->data->nodeText->bounds.w = endLine;
			itemNode->data->nodeText->bounds.h *= lineScale;

			if (itemNode->data->optionsList.Count() != 0)
			{
				int optPlace = 0;
				for (ListItem<DialogueOption*>* itemOption = itemNode->data->optionsList.start;
					itemOption != nullptr; itemOption = itemOption->next)
				{
					itemOption->data->optionText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
					itemOption->data->optionText->SetString(itemOption->data->text.GetString());
					itemOption->data->optionText->SetTextFont(app->fontTTF->defaultFont);

					itemOption->data->optPlacing = OptionsPos(optPlace);
					optPlace++;
				}
			}
			else continue;
		}
		onDialog = true;
	}
}

void DialogueManager::Draw()
{
	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;
	SDL_Rect nodeChart = currentDialogue->currentNode->nodeText->bounds;

	currentDialogue->currentNode->NodePlacing();
	currentDialogue->currentNode->nodeText->CenterDialogue();

	nodeChart.x -= offset;
	nodeChart.y -= offset;
	nodeChart.w += (2 * offset);
	nodeChart.h += (3 * offset);

	SDL_Rect textureChart{ 0,0,910,113 };
	
	//DRAWING NODE
	app->render->DrawTexture(dialogueTexture, nodeChart.x, nodeChart.y, 1.0, &textureChart);
	app->render->DrawRectangle(nodeChart, black, true, false);
	currentDialogue->currentNode->nodeText->Draw();
	
	if (currentDialogue->currentNode->optionsList.Count() != 0)
	{
		if(currentDialogue->currentNode->optionsActive == false)currentDialogue->currentNode->optionsActive = true;

		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			if (!itemOption->data->optionButton)
			{
				itemOption->data->optionButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
				itemOption->data->optionButton->bounds = { 0,0,int(buttonPrefab.w * dscale),int(buttonPrefab.h * dscale) - offset };
				itemOption->data->optionButton->text = itemOption->data->optionText->text.GetString();
				itemOption->data->optionButton->SetObserver(this);
			}

			itemOption->data->optionText->bounds = itemOption->data->optionButton->bounds;
			itemOption->data->OptionPlacingX();
			itemOption->data->OptionPlacingY();
			itemOption->data->optionText->CenterAlign();

			itemOption->data->optionButton->Draw(1.2f, false);
			itemOption->data->optionText->Draw();	
		}
	}
}

void DialogueManager::Input()
{
	if (currentDialogue != nullptr && currentDialogue->currentNode->optionsActive == true)
	{
		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			if(itemOption->data->optionButton != nullptr)
				itemOption->data->optionButton->Update(1.0f);
		}
	}
	else EndDialogue();
}

bool DialogueManager::OnGuiMouseClickEvent(GuiControl* option)
{
	bool ret = true;

	currentDialogue->currentNode->optionsActive = false;

	//bool optionHitted = false;

	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start; 
		 itemOption != nullptr; itemOption = itemOption->next)
	{
		if (strcmp(option->text.GetString(), itemOption->data->text.GetString()) == 0)//&& !optionHitted)
		{
			if(itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 1)
			{
				DeleteOptionButtons();
				currentDialogue->currentNode = itemOption->data->nextNode;
				//optionHitted = true;
				return ret;
			}
			else if (itemOption->data->returnCode == 0)
			{
				//node with continue option
				//==> go to next node
				return ret;
			}
			else if (itemOption->data->returnCode == -1)
			{
				EndDialogue();
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 2)
			{
				app->scene->combatScene->secondPlayer = true;
				currentDialogue->currentNode = itemOption->data->nextNode;
				//optionHitted = true;
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 3)
			{
				app->scene->combatScene->secondPlayer = false;
				currentDialogue->currentNode = itemOption->data->nextNode;
				//optionHitted = true;
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 4)
			{
				app->scene->player1->Refill();
				currentDialogue->currentNode = itemOption->data->nextNode;
				//optionHitted = true;
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 5)
			{
				app->scene->player2->Refill();
				currentDialogue->currentNode = itemOption->data->nextNode;
				//optionHitted = true;
				return ret;
			}
		}
	}
	return ret;
}

pugi::xml_node DialogueManager::LoadDialogueConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(DIALOGUE_CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", DIALOGUE_CONFIG_FILENAME, result.description());
	else ret = configFile.child("DialogSettup");

	return ret;
	
}