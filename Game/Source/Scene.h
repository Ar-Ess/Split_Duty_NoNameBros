#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"


class GuiButton;
class GuiSlider;
class GuiCheckBox;
class GuiString;
struct SDL_Texture;
class Enemy;
class Player;
class Combat;
class World;
enum Places;

enum Scenes
{
	NONE = -1,
	LOGO_SCENE,
	MAIN_MENU,
	COMBAT,
	LEVEL_UP,
	WORLD
};

enum Environments
{
	GRASSY_LANDS,
	AUTUM_FALLS,
	MOSSY_LANDS
};

class Scene : public Module
{
public:

	Scene();

	virtual ~Scene();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	Scenes GetCurrScene() const
	{
		return currScene;
	}

public: //Combat Buttons

	void RestartPressState();

	GuiButton* attackButton = nullptr;
	bool attackPressed = false;

	GuiButton* moveButton = nullptr;
	bool movePressed = false;

	GuiButton* itemButton = nullptr;
	bool itemPressed = false;

	GuiButton* escapeButton = nullptr;
	bool escapePressed = false;

	GuiButton* splitButton = nullptr;
	bool splitPressed = false;

	GuiButton* secondAttackButton = nullptr;
	bool secondAttackPressed = false;

	GuiButton* protectButton = nullptr;
	bool protectPressed = false;

	GuiButton* buffButton = nullptr;
	bool buffPressed = false;

	GuiButton* smallMeatButton = nullptr;
	bool smallMeatPressed = false;

	GuiButton* largeMeatButton = nullptr;
	bool largeMeatPressed = false;

	GuiButton* featherButton = nullptr;
	bool featherPressed = false;

	GuiButton* mantisButton = nullptr;
	bool mantisPressed = false;

	GuiButton* enemySplitButton = nullptr;
	bool enemySplitPressed = false;

	GuiButton* moneyButton = nullptr;
	bool moneyPressed = false;

	GuiString* attackText = nullptr;

	GuiString* moveText = nullptr;

	GuiString* itemsText = nullptr;

	GuiString* escapeText = nullptr;

	GuiString* splitText = nullptr;

	GuiString* protectText = nullptr;

	GuiString* buffText = nullptr;

private: // Textures
	SDL_Texture* logo = nullptr;

public:
	friend class Combat;
	friend class Enemy;
	friend class Player;

	Player* player1 = nullptr;
	Player* player2 = nullptr;
	Combat* combatScene = nullptr;
	World* world = nullptr;
	Environments enviroment;

private: //Scene Manager
	friend class World;

	Scenes currScene = NONE;
	Scenes prevScene = NONE;

	void SetScene(Scenes scene);
	void SetScene(Scenes scene, Enemy* enemy);
	void SetScene(Scenes scene, Places place);
	void SetScene(Scenes scene, unsigned short int exp);

	//Setters
	void SetLogoScene();
	void SetMainMenu();
	void SetCombat(Enemy* enemy);
	void SetLevelUp(unsigned short int experience);
	void SetWorld(Places place);

	//Updaters
	void UpdateLogoScene();
	void UpdateMainMenu();
	void UpdateCombat();
	void UpdateLevelUp();
	void UpdateWorld();

private: //debug
	void DebugSteps(); // Future Debug Module implementation

	unsigned short int timer = 0;

private:
	bool OnGuiMouseClickEvent(GuiControl* control);
};

#endif // __SCENE_H__