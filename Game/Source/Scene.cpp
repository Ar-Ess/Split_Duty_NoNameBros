#include "App.h"
#include "Scene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "World.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Combat.h"
#include "DialogueManager.h"
#include "Transition.h"

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
	player1 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER1);
	player2 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER2);

	player1->SetUp(40, 40, 30, 10, 0, 0, 0, 10, 0, 0);

	world = new World();

	combatScene = new Combat();

	enviroment = GRASSY_LANDS;

	SetScene(LOGO_SCENE);

	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);

	//ENEMY SET                                          ENEMY CLASS           -----------------COMBAT RECT----------------        ---WORLD RECT---  LVL EXP  HP STR DEF  VEL
	app->entityManager->enemies.start->data->SetUp(EnemyClass::SMALL_WOLF, { INIT_SMALLWOLF_POSX, INIT_SMALLWOLF_POSY, 86, 44 }, {1000, 180, 70, 42}, 2, 200, 30, 30, 10, 20);

	app->entityManager->enemies.start->next->data->SetUp(EnemyClass::BIRD, { INIT_BIRD_POSX, INIT_BIRD_POSY, 40, 75 }, { 1200, 180, 70, 42 }, 2, 200, 30, 30, 10, 20);
	app->entityManager->enemies.start->next->next->data->SetUp(EnemyClass::MANTIS, { INIT_MANTIS_POSX, INIT_MANTIS_POSY, 56, 75 }, { 1400, 180, 70, 42 }, 2, 200, 30, 30, 10, 20);

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
	else if (currScene == LEVEL_UP) UpdateLevelUp();
	else if (currScene == WORLD) UpdateWorld();

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

	if (currScene == LOGO_SCENE)
	{
		app->tex->UnLoad(logo);
	}
	else if (currScene == MAIN_MENU)
	{

	}
	else if (currScene == COMBAT)
	{
		combatScene->Restart();
	}
	else if (currScene == LEVEL_UP)
	{

	}
	else if (currScene == WORLD)
	{
		world->Restart();
	}

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
	//else if (scene == COMBAT) SetCombat();
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
}

void Scene::SetScene(Scenes scene, Enemy* enemy)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == COMBAT) SetCombat(enemy);
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
}

void Scene::SetScene(Scenes scene, Places place)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	//else if (scene == COMBAT) SetCombat();
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(place);
}

void Scene::SetScene(Scenes scene, unsigned short int exp)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	//else if (scene == COMBAT) SetCombat();
	else if (scene == LEVEL_UP) SetLevelUp(exp);
	//else if (scene == WORLD) SetWorld(place);
}

void Scene::SetLogoScene()
{
	logo = app->tex->Load("Assets/Textures/logo_nonamebros.png");
	timer = 0;
}

void Scene::SetMainMenu()
{
	app->audio->SetMusic(SoundTrack::MAINMENU_TRACK);
}

void Scene::SetCombat(Enemy* enemySet)
{
	combatScene->enemy = enemySet;
	combatScene->Start();

	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;
	
	if (attackButton == nullptr)
	{
		attackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		attackButton->bounds = { app->guiManager->margin ,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		attackButton->text = "AttackButton";
		attackButton->SetObserver(this);
	}

	if (moveButton == nullptr)
	{
		moveButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		moveButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding)* 1),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		moveButton->text = "MoveButton";
		moveButton->SetObserver(this);
	}

	if (itemButton == nullptr)
	{
		itemButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		itemButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 2),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		itemButton->text = "ItemButton";
		itemButton->SetObserver(this);
		app->scene->itemButton->state = GuiControlState::LOCKED;
	}

	if (escapeButton == nullptr)
	{
		escapeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		escapeButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 3),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		escapeButton->text = "EscapeButton";
		escapeButton->SetObserver(this);
	}

	if (splitButton == nullptr)
	{
		splitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		splitButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 4),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		splitButton->text = "SplitButton";
		splitButton->SetObserver(this);
	}

	if (secondAttackButton == nullptr)
	{
		secondAttackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		secondAttackButton->bounds = { app->guiManager->margin ,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		secondAttackButton->text = "SecondAttackButton";
		secondAttackButton->SetObserver(this);
	}

	if (protectButton == nullptr)
	{
		protectButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		protectButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 1),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		protectButton->text = "ProtectButton";
		protectButton->SetObserver(this);
	}

	if (buffButton == nullptr)
	{
		buffButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		buffButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 2),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		buffButton->text = "BuffButton";
		buffButton->SetObserver(this);
	}

	//if (smallMeatButton == nullptr)
	//{
	//	smallMeatButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	smallMeatButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 2),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	smallMeatButton->text = "ItemButton";
	//	smallMeatButton->SetObserver(this);
	//}

	//if (largeMeatButton == nullptr)
	//{
	//	largeMeatButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	largeMeatButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 3),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	largeMeatButton->text = "EscapeButton";
	//	largeMeatButton->SetObserver(this);
	//}

	//if (featherButton == nullptr)
	//{
	//	featherButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	featherButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 4),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	featherButton->text = "SplitButton";
	//	featherButton->SetObserver(this);
	//}

	//if (mantisButton == nullptr)
	//{
	//	mantisButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	mantisButton->bounds = { app->guiManager->margin ,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	mantisButton->text = "SecondAttackButton";
	//	mantisButton->SetObserver(this);
	//}

	//if (enemySplitButton == nullptr)
	//{
	//	enemySplitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	enemySplitButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 1),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	enemySplitButton->text = "ProtecteButton";
	//	enemySplitButton->SetObserver(this);
	//}

	//if (moneyButton == nullptr)
	//{
	//	moneyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
	//	moneyButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 2),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
	//	moneyButton->text = "BuffButton";
	//	moneyButton->SetObserver(this);
	//}

	if (attackText == nullptr)
	{
		attackText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		attackText->bounds = attackButton->bounds;
		attackText->SetTextFont(app->fontTTF->defaultFont);
		attackText->SetString("ATTACK");
	}

	if (moveText == nullptr)
	{
		moveText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		moveText->bounds = { moveButton->bounds };
		moveText->SetTextFont(app->fontTTF->defaultFont);
		moveText->SetString("MOVE");
	}

	if (itemsText == nullptr)
	{
		itemsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		itemsText->bounds = { itemButton->bounds };
		itemsText->SetTextFont(app->fontTTF->defaultFont);
		itemsText->SetString("ITEM(beta)");
	}

	if (escapeText == nullptr)
	{
		escapeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		escapeText->bounds = { escapeButton->bounds };
		escapeText->SetTextFont(app->fontTTF->defaultFont);
		escapeText->SetString("ESCAPE");
	}

	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { splitButton->bounds };
		splitText->SetTextFont(app->fontTTF->defaultFont);
		splitText->SetString("SPLIT");
	}

	if (protectText == nullptr)
	{
		protectText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		protectText->bounds = { protectButton->bounds };
		protectText->SetTextFont(app->fontTTF->defaultFont);
		protectText->SetString("PROTECT");
	}

	if (buffText == nullptr)
	{
		buffText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		buffText->bounds = { buffButton->bounds };
		buffText->SetTextFont(app->fontTTF->defaultFont);
		buffText->SetString("BUFFS");
	}
}

void Scene::SetLevelUp(unsigned short int exp)
{

}

void Scene::SetWorld(Places place)
{
	world->Start(place);
}

void Scene::UpdateLogoScene()
{
	if (timer >= 50) app->render->DrawTexture(logo, 0, 0);

	if (timer < 50)
	{
		timer++;
	}
	else if (timer < 80)
	{
		app->transition->FadeToBlackEffect(true, 30.0f);
		timer++;
	}
	else if (timer < 250)
	{
		timer++;
	}
	else if (timer >= 250 && timer < 280)
	{
		app->transition->FadeToBlackEffect(false, 30.0f);
		timer++;
	}
	else
	{
		SetScene(MAIN_MENU);
	}
}

void Scene::UpdateMainMenu()
{
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->data);
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->next->data);
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->next->next->data);
	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) SetScene(WORLD, Places::MAIN_VILLAGE);
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		app->dialogueManager->StartDialogue(1); 
		app->guiManager->debugGui = true;
	}

// Other Options
	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		app->scene->player1->godMode = !app->scene->player1->godMode;

		if (app->scene->player1->godMode)
			LOG("God mode enabled");
		else
			LOG("God mode disabled");
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		app->scene->combatScene->secondPlayer = !app->scene->combatScene->secondPlayer;;

		if (app->scene->combatScene->secondPlayer)
			LOG("Second Player enabled");
		else
			LOG("Second Player disabled");
	}
}

void Scene::UpdateCombat()
{
	combatScene->Update();

	combatScene->Draw();

	//app->guiManager->Update(1.0f); // NO USE

	combatScene->EndBattleSolving();

	RestartPressState();

	DebugSteps();
}

void Scene::UpdateLevelUp()
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) SetScene(WORLD, world->place);
}

void Scene::UpdateWorld()
{
	world->Update();

	world->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) SetScene(MAIN_MENU);
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
		else if (strcmp(control->text.GetString(), "MoveButton") == 0 && !combatScene->playerStepDenied) movePressed = true;
		else if (strcmp(control->text.GetString(), "ItemButton") == 0) itemPressed = true;
		else if (strcmp(control->text.GetString(), "EscapeButton") == 0) escapePressed = true;
		else if (strcmp(control->text.GetString(), "SplitButton") == 0) splitPressed = true;
		else if (strcmp(control->text.GetString(), "SecondAttackButton") == 0) secondAttackPressed = true;
		else if (strcmp(control->text.GetString(), "ProtectButton") == 0) protectPressed = true;
		else if (strcmp(control->text.GetString(), "BuffButton") == 0) buffPressed = true;

		break;
	}

	return true;
}

void Scene::RestartPressState()
{
	attackPressed = false;
	movePressed = false;
	itemPressed = false;
	escapePressed = false;
	splitPressed = false;
	secondAttackPressed = false;
	protectPressed = false;
	buffPressed = false;

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