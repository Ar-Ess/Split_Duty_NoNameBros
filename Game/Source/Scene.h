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

enum Scenes
{
	NONE = -1,
	LOGO_SCENE,
	MAIN_MENU,
	COMBAT
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

private: //Combat Buttons

	void RestartPressState();

	GuiButton* attackButton = nullptr;
	bool attackPressed = false;

	GuiButton* moveButton = nullptr;
	bool movePressed = false;

	GuiButton* itemButton = nullptr;
	bool itemPressed = false;

	GuiButton* scapeButton = nullptr;
	bool scapePressed = false;

	GuiButton* reapButton = nullptr;
	bool reapPressed = false;

private:
	friend class Combat;

	Player* player1;
	Combat* combatScene;

private: //Scene Manager
	Scenes currScene = NONE;
	Scenes prevScene = NONE;

	void SetScene(Scenes scene);
	void SetScene(Scenes scene, Enemy* enemy);

	//Setters
	void SetLogoScene();
	void SetMainMenu();
	void SetCombat(Enemy* enemy);

	//Updaters
	void UpdateLogoScene();
	void UpdateMainMenu();
	void UpdateCombat();

private: //debug
	void DebugSteps(); // Future Debug Module implementation

private:
	bool OnGuiMouseClickEvent(GuiControl* control);
};

#endif // __SCENE_H__