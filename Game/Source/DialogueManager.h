#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Input.h"
#include "Dialogue.h"
#include "DialogueNode.h"
#include "DialogueOption.h"
#include "FontTTF.h"

#include "SDL/include/SDL_rect.h"
//#include "SDL/include/SDL.h"

#include "Log.h"
#include "List.h"

#define DIALOGUE_CONFIG_FILENAME		"Dialog_Config.xml"

struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;

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

	bool Update(float dt);

	bool CleanUp();
	/////////////////////////////////	
	void StartDialogue(int dialogueID);

private: //functions

	//////CREATORS//////
	DialogueOption* CreateOptions(pugi::xml_node& setter);
	DialogueNode* CreateNode(pugi::xml_node& setter);
	void CreateDialogue(pugi::xml_node& setter);

	void Draw();
	void Input();

	bool OnGuiMouseClickEvent(GuiControl* option);

	pugi::xml_node LoadDialogueConfig(pugi::xml_document&) const;

	void EndDialogue();
	void DeleteOptions();
	void DeleteOptionButtons();

public: //variables
	bool				onDialog = false;
	int					endLine = 850;
private: // variables
	int					offset = 15;
	float				dscale = 1.2f;

	Dialogue* currentDialogue;
	List<Dialogue*>		dialoguesList;

	SDL_Color           black = { 0  , 0  , 0  , 150 };
	SDL_Color           white = { 255, 255, 255, 125 };
	SDL_Color           red =	{ 255, 0  , 0  , 125 };
	SDL_Color           green = { 0  , 255, 0  , 125 };
	SDL_Color           blue =	{ 0  , 0  , 255, 125 };

	SDL_Texture*		dialogueTexture = nullptr;
};

#endif // __DIALOGUEMANAGER_H__
