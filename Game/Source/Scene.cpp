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
#include "Player.h"
#include "Enemy.h"
#include "Combat.h"

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

	player1 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

	combatScene = new Combat();

	SetScene(LOGO_SCENE);

	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);

	app->entityManager->enemies.start->data->SetUp(EnemyClass::SMALL_WOLF, { INIT_SMALLWOLF_POSX, INIT_SMALLWOLF_POSY, 86, 44 }, 2, 200, 30, 30, 10, 20);
	app->entityManager->enemies.start->next->data->SetUp(EnemyClass::BIRD, { INIT_BIRD_POSX, INIT_BIRD_POSY, 40, 75 }, 2, 200, 30, 30, 10, 20);

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
	else if (currScene == COMBAT) UpdateCombat();

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

	if (currScene == COMBAT) combatScene->Restart();

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();

}

void Scene::SetScene(Scenes scene, Enemy* enemy)
{
	prevScene = currScene;
	currScene = scene;

	CleanUp();

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == COMBAT) SetCombat(enemy);
}

void Scene::SetLogoScene()
{

}

void Scene::SetMainMenu()
{

}

void Scene::SetCombat(Enemy* enemySet)
{
	combatScene->enemy = enemySet;
	combatScene->Start();

	app->guiManager->debugGui = true;

	if (attackButton == nullptr)
	{
		attackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		attackButton->bounds = { 0, 0, 105, 27 };
		attackButton->text = "AttackButton";
		attackButton->SetObserver(this);
	}

	if (moveButton == nullptr)
	{
		moveButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		moveButton->bounds = { 200, 0, 105, 27 };
		moveButton->text = "MoveButton";
		moveButton->SetObserver(this);
	}

	if (itemButton == nullptr)
	{
		itemButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		itemButton->bounds = { 400, 0, 105, 27 };
		itemButton->text = "ItemButton";
		itemButton->SetObserver(this);
	}

	if (scapeButton == nullptr)
	{
		scapeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		scapeButton->bounds = { 600, 0, 105, 27 };
		scapeButton->text = "ScapeButton";
		scapeButton->SetObserver(this);
	}

	if (splitButton == nullptr)
	{
		splitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		splitButton->bounds = { 800, 0, 135, 67 };
		splitButton->text = "SplitButton";
		splitButton->SetObserver(this);
	}

	combatScene->character1Spritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/female_character_spritesheet.png");
}

void Scene::UpdateLogoScene()
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) SetScene(MAIN_MENU);
}

void Scene::UpdateMainMenu()
{
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->data);
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->next->data);
}

void Scene::UpdateCombat()
{
	combatScene->Update();

	combatScene->Draw();

	if (combatScene->playerScape) SetScene(MAIN_MENU);

	RestartPressState();

	DebugSteps();
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

	case COMBAT:

		if (strcmp(control->text.GetString(), "AttackButton") == 0) attackPressed = true;
		else if (strcmp(control->text.GetString(), "MoveButton") == 0) movePressed = true;
		else if (strcmp(control->text.GetString(), "ItemButton") == 0) itemPressed = true;
		else if (strcmp(control->text.GetString(), "ScapeButton") == 0) scapePressed = true;
		else if (strcmp(control->text.GetString(), "SplitButton") == 0) splitPressed = true;

		break;
	}

	return true;
}

void Scene::RestartPressState()
{
	attackPressed = false;
	movePressed = false;
	itemPressed = false;
	scapePressed = false;
	splitPressed = false;
}

// Debug functions (future in debug module)

void Scene::DebugSteps()
{
	app->render->DrawLine(249, 498, 249, 510, {255, 255, 255, 255});
	app->render->DrawLine(419, 498, 419, 510, {255, 255, 255, 255});
	app->render->DrawLine(589, 498, 589, 510, {255, 255, 255, 255});
	app->render->DrawLine(759, 498, 759, 510, {255, 255, 255, 255});
	app->render->DrawLine(1031, 498, 1031, 510, {255, 0, 0, 255});
}