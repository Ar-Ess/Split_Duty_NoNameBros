#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

class GuiButton;
class GuiSlider;
class GuiCheckBox;
class GuiString;
struct SDL_Texture;

enum Scenes
{
	NONE = -1,
	LOGO_SCENE,
	MAIN_MENU
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

private: //Scene Manager
	Scenes currScene = NONE;
	Scenes prevScene = NONE;

	void SetScene(Scenes scene);

	//Setters
	void SetLogoScene();
	void SetMainMenu();

	//Updaters
	void UpdateLogoScene();
	void UpdateMainMenu();


private:
	bool OnGuiMouseClickEvent(GuiControl* control);

	GuiSlider* testSlider;
};

#endif // __SCENE_H__