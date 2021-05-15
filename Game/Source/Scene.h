#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Point.h"
#include "DynArray.h"

#include "Spline.h"
#include "EasingFunctions.h"

class GuiButton;
class GuiString;
struct SDL_Texture;
class Enemy;
class Player;
class Combat;
class World;
class Inventory;
class LevelUp;
class OptionsMenu;
class GameOverScene;
enum Places;

enum Scenes
{
	NONE = -1,
	LOGO_SCENE,
	MAIN_MENU,
	OPTIONS_MENU,
	COMBAT,
	LEVEL_UP,
	WORLD,
	PAUSE_MENU,
	END_SCREEN,
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

	bool CleanUp(Scenes scene);

	Scenes GetCurrScene() const
	{
		return currScene;
	}

public: //Combat Buttons

	void RestartPressState();

	GuiButton* newGameButton = nullptr;

	GuiButton* continueButton = nullptr;
	bool continuePressed = false;

	GuiButton* optionsButton = nullptr;

	GuiButton* backToGameButton = nullptr;

	GuiButton* optionsPauseButton = nullptr;

	GuiButton* saveGameButton = nullptr;

	GuiButton* backToMenuButton = nullptr;

	GuiButton* exitButton = nullptr;

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

	//GuiButton* smallMeatButton = nullptr;
	bool smallMeatPressed = false;

	//GuiButton* largeMeatButton = nullptr;
	bool largeMeatPressed = false;

	//GuiButton* featherButton = nullptr;
	bool featherPressed = false;

	//GuiButton* mantisButton = nullptr;
	bool mantisPressed = false;

	//GuiButton* enemySplitButton = nullptr;
	bool enemySplitPressed = false;

	//GuiButton* moneyButton = nullptr;
	bool moneyPressed = false;

	GuiString* newGameText = nullptr;

	GuiString* continueText = nullptr;

	GuiString* optionsText = nullptr;

	GuiString* exitText = nullptr;

	GuiString* backToGameText = nullptr;

	GuiString* saveGameText = nullptr;

	GuiString* optionsPauseText = nullptr;

	GuiString* backToMenuText = nullptr;

	bool exit = false;

	bool activeContinue = false;

private: // Textures
	SDL_Texture* logo = nullptr;
	SDL_Texture* mainLogo = nullptr;
	SDL_Texture* menu = nullptr;
	SDL_Texture* pause = nullptr;
public:
	SDL_Texture* optionsBackground = nullptr;
public:
	friend class Combat;
	friend class Enemy;
	friend class Player;
	friend class Inventory;
	friend class LevelUpScene;

	Player* player1 = nullptr;
	Player* player2 = nullptr;
	Combat* combatScene = nullptr;
	World* world = nullptr;
	LevelUp* levelUpScene = nullptr;
	OptionsMenu* optionsMenu = nullptr;
	GameOverScene* endScene = nullptr;
	Environments enviroment;
	iPoint prevCam = { 0, 0 };
	Spline spline;
	EasingFunctions easing;
	int iterations, totalIterations;
	bool easingActive = false;

	bool sp = false;

private: //Scene Manager
	friend class World;
	friend class App;
	friend class Map; //Erase, just test

	Scenes currScene = NONE;
	Scenes prevScene = NONE;

	void SetScene(Scenes scene);
	void SetScene(Scenes scene, Enemy* enemy);
	void SetScene(Scenes scene, Places place);
	void SetScene(Scenes scene, unsigned short int exp);

	//Setters
	void SetLogoScene();
	void SetMainMenu();
	void SetOptionsMenu();
	void SetCombat(Enemy* enemy);
	void SetLevelUp(unsigned short int experience);
	void SetWorld(Places place);
	void SetPauseMenu();
	void SetEndScreen();

	//Updaters
	void UpdateLogoScene();
	void UpdateMainMenu();
	void UpdateOptionsMenu();
	void UpdateCombat();
	void UpdateLevelUp();
	void UpdateWorld();
	void UpdatePauseMenu();
	void UpdateEndScreen();

	float EaseLogoBetweenPoints(iPoint posA, iPoint posB,bool repeat);
	float EaseTitleBetweenPoints(iPoint posA, iPoint posB,bool repeat);


private: //debug
	void DebugSteps(); // Future Debug Module implementation
	//SDL_Texture* debugPath = nullptr;
	//DynArray<iPoint>* debugPathList;
	unsigned short int timer = 0;

private:
	bool OnGuiMouseClickEvent(GuiControl* control);
};

#endif // __SCENE_H__