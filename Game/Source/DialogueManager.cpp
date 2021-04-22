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

	if (configDial.empty() == false)
	{
		pugi::xml_node dSetUp = configDial.child("Dialogs");
		CreateDialogue(dSetUp);
	}
	return ret;
}

bool DialogueManager::Start()
{	
	dialogueTexture = app->tex->Load("Assets/Textures/UI/dialogSquare.png");
	buttonPrefab = app->guiManager->buttonPrefab;
	tempString = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
	tempText.clear();
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

	DialogueNode* node = new DialogueNode(nd.child_value(), setter.attribute("ID").as_int());

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
			if (itemNode->data->optionsList.Count() != 0)
			{
				if(itemNode->data->letterCounter == 0)
					itemNode->data->letterCounter = 0;

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
	////WRITTING MACHINE LOOK//// 	 	 
	counter++;
	if(counter % 2 == 0 && currentDialogue->currentNode->nodeEnd == false)
	{
		if(currentDialogue->currentNode->letterCounter < currentDialogue->currentNode->text.size())
		{
			tempText += currentDialogue->currentNode->text.at(currentDialogue->currentNode->letterCounter);
			currentDialogue->currentNode->letterCounter++;
		}
		else
		{
			currentDialogue->currentNode->nodeEnd = true;
			counter = 0;
		}
	}
	tempString->SetString(tempText.c_str());
	tempString->SetTextFont(app->fontTTF->defaultFont);

	app->fontTTF->CalcSize(tempString->text.GetString(), tempString->bounds.w,
		tempString->bounds.h, app->fontTTF->defaultFont);

	if (tempString->bounds.w >= endLine)
	{
		int lineScale = static_cast<int>(ceil(tempString->bounds.w / endLine));
		tempString->bounds.h *= lineScale;
	}
	else
	{
		tempString->bounds.h -= (offset + 5);
	}
	tempString->bounds.w = endLine;
	SDL_Rect nodeChart = tempString->bounds;

	tempString->NodePlacing();

	nodeChart.x -= offset;
	nodeChart.y -= offset;
	nodeChart.w += (2 * offset);
	nodeChart.h += (3 * offset);
	
	//DRAWING NODE
	SDL_RenderCopy(app->render->renderer, dialogueTexture, &textureChart, &nodeChart);
	tempString->Draw();
	
	if(currentDialogue->currentNode->nodeEnd == true)
		currentDialogue->currentNode->optionsActive = true;

	if (currentDialogue->currentNode->optionsList.Count() != 0 && currentDialogue->currentNode->optionsActive == true)
	{
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
			itemOption->data->OptionPlacingX(nodeChart, dscale);
			itemOption->data->OptionPlacingY();
			itemOption->data->optionText->CenterAlign();

			//draw buttons options
			itemOption->data->optionButton->Draw(dscale, false);
			itemOption->data->optionText->Draw();	
		}
	}
}

void DialogueManager::Input()
{
	if (currentDialogue->currentNode->optionsActive == true)
	{
		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			if(itemOption->data->optionButton != nullptr)
				itemOption->data->optionButton->Update(1.0f);
		}
	}
	//else EndDialogue();
}

bool DialogueManager::OnGuiMouseClickEvent(GuiControl* option)
{
	bool ret = true;

	currentDialogue->currentNode->optionsActive = false;
	currentDialogue->currentNode->nodeEnd = false;
	currentDialogue->currentNode->letterCounter = 0;
	tempText.clear();
	tempString->UnLoadTextTexture();

	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start; 
		 itemOption != nullptr; itemOption = itemOption->next)
	{
		if (strcmp(option->text.GetString(), itemOption->data->text.GetString()) == 0)
		{
			if(itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 1)
			{
				DeleteOptionButtons();
				currentDialogue->currentNode = itemOption->data->nextNode;
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
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 3)
			{
				app->scene->combatScene->secondPlayer = false;
				currentDialogue->currentNode = itemOption->data->nextNode;
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 4)
			{
				app->scene->player1->Refill();
				currentDialogue->currentNode = itemOption->data->nextNode;
				return ret;
			}
			else if (itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 5)
			{
				app->scene->player2->Refill();
				currentDialogue->currentNode = itemOption->data->nextNode;
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