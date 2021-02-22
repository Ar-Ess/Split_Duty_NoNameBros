#include "App.h"
#include "Scene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Entity.h"

#include "GuiManager.h"
#include "GuiString.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "FontTTF.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

Scene::~Scene()
{}

bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Scene::Start()
{
	SetScene(LOGO_SCENE);

	return true;
}

bool Scene::PreUpdate()
{
	/*
	// L12b: Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathFinding->CreatePath(origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}
	*/
	return true;
}

bool Scene::Update(float dt)
{
	// L12b: Debug pathfinding
	/*
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = app->pathFinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
	}
	*/

	if (currScene == LOGO_SCENE) UpdateLogoScene();
	else if (currScene == MAIN_MENU) UpdateMainMenu();

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	app->win->FullScreenLogic();

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	prevScene = currScene;
	currScene = scene;

	CleanUp();

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();

}

void Scene::SetLogoScene()
{

}

void Scene::SetMainMenu()
{

}

void Scene::UpdateLogoScene()
{

}

void Scene::UpdateMainMenu()
{

}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currScene)
	{
	case LOGO_SCENE:
		break;

	case MAIN_MENU:
		break;
	}

	return true;
}
