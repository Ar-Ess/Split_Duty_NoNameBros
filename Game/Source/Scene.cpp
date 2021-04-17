#define _CRT_SECURE_NO_WARNINGS

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

	world = new World();

	combatScene = new Combat();

	enviroment = GRASSY_LANDS;

	SetScene(LOGO_SCENE);

	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);

	//ENEMY SET                                          ENEMY CLASS           -----------------COMBAT RECT----------------        ---WORLD RECT---  LVL EXP  HP STR DEF  VEL
	app->entityManager->enemies.start->data->SetUp(EnemyClass::SMALL_WOLF, { INIT_SMALLWOLF_POSX, INIT_SMALLWOLF_POSY, 86, 44 }, {1000, 180, 70, 42}, 2, 200, 25, 15, 10, 20);

	app->entityManager->enemies.start->next->data->SetUp(EnemyClass::BIRD, { INIT_BIRD_POSX, INIT_BIRD_POSY, 40, 75 }, { 1200, 180, 70, 42 }, 2, 400, 30, 15, 15, 40);
	app->entityManager->enemies.start->next->next->data->SetUp(EnemyClass::MANTIS, { INIT_MANTIS_POSX, INIT_MANTIS_POSY, 56, 75 }, { 1400, 180, 70, 42 }, 2, 400, 40, 20, 5, 20);

	if (FILE* file = fopen("save_game.xml", "r"))
	{
		fclose(file);
		activeContinue = true;
	}
	else
	{
		activeContinue = false;
	}

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
	else if (currScene == OPTIONS_MENU) UpdateOptionsMenu();
	else if (currScene == COMBAT) UpdateCombat();
	else if (currScene == LEVEL_UP) UpdateLevelUp();
	else if (currScene == WORLD) UpdateWorld();
	else if (currScene == PAUSE_MENU) UpdatePauseMenu();

	return true;
}

bool Scene::PostUpdate()
{
	app->win->FullScreenLogic();

	return !exit;
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
		app->tex->UnLoad(menu);
	}
	else if (currScene == OPTIONS_MENU)
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
	else if (scene == OPTIONS_MENU) SetOptionsMenu();
	//else if (scene == COMBAT) SetCombat();
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}

void Scene::SetScene(Scenes scene, Enemy* enemy)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == OPTIONS_MENU) SetOptionsMenu();
	else if (scene == COMBAT) SetCombat(enemy);
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}

void Scene::SetScene(Scenes scene, Places place)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == OPTIONS_MENU) SetOptionsMenu();
	//else if (scene == COMBAT) SetCombat();
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(place);
	else if (scene == PAUSE_MENU) SetPauseMenu();
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
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}


void Scene::SetLogoScene()
{
	logo = app->tex->Load("Assets/Textures/logo_nonamebros.png");
	timer = 0;
}

void Scene::SetMainMenu()
{
	app->audio->SetMusic(SoundTrack::MAINMENU_TRACK);
	menu = app->tex->Load("Assets/Screens/tittle_screen.png");

	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;

	if (newGameButton == nullptr)
	{
		newGameButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		newGameButton->bounds = { 640 - buttonPrefab.w / 2 , 350,buttonPrefab.w,buttonPrefab.h };
		newGameButton->text = "NewGameButton";
		newGameButton->SetObserver(this);
	}

	if (continueButton == nullptr)
	{
		continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		continueButton->bounds = { 640 - buttonPrefab.w / 2 , 430,buttonPrefab.w,buttonPrefab.h };
		continueButton->text = "ContinueButton";
		continueButton->SetObserver(this);
		if (!activeContinue) continueButton->state = GuiControlState::LOCKED;
	}

	if (optionsButton == nullptr)
	{
		optionsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		optionsButton->bounds = { 640 - buttonPrefab.w / 2 , 510, buttonPrefab.w, buttonPrefab.h };
		optionsButton->text = "OptionsButton";
		optionsButton->SetObserver(this);
		optionsButton->state = GuiControlState::LOCKED;
	}

	if (exitButton == nullptr)
	{
		exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		exitButton->bounds = { 640 - buttonPrefab.w / 2 , 590, buttonPrefab.w,buttonPrefab.h };
		exitButton->text = "ExitButton";
		exitButton->SetObserver(this);
	}

	if (newGameText == nullptr)
	{
		newGameText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		newGameText->bounds = newGameButton->bounds;
		newGameText->SetTextFont(app->fontTTF->defaultFont);
		newGameText->SetString("NEW GAME");
		newGameText->CenterAlign();
	}

	if (continueText == nullptr)
	{
		continueText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		continueText->bounds = continueButton->bounds;
		continueText->SetTextFont(app->fontTTF->defaultFont);
		continueText->SetString("CONTINUE");
		continueText->CenterAlign();
	}

	if (optionsText == nullptr)
	{
		optionsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsText->bounds = optionsButton->bounds;
		optionsText->SetTextFont(app->fontTTF->defaultFont);
		optionsText->SetString("OPTIONS");
		optionsText->CenterAlign();
	}

	if (exitText == nullptr)
	{
		exitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		exitText->bounds = exitButton->bounds;
		exitText->SetTextFont(app->fontTTF->defaultFont);
		exitText->SetString("EXIT");
		exitText->CenterAlign();
	}
}

void Scene::SetOptionsMenu()
{

}

void Scene::SetCombat(Enemy* enemySet)
{
	if (combatScene->turnText == nullptr)
	{
		combatScene->turnText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->turnText->bounds = { 540, 40, 200, 50 };
		combatScene->turnText->SetTextFont(app->fontTTF->defaultFont);
	}

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
		itemButton->state = GuiControlState::LOCKED;
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
		buffButton->state = GuiControlState::LOCKED;
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
		attackText->CenterAlign();
	}

	if (moveText == nullptr)
	{
		moveText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		moveText->bounds = { moveButton->bounds };
		moveText->SetTextFont(app->fontTTF->defaultFont);
		moveText->SetString("MOVE");
		moveText->CenterAlign();
	}

	if (itemsText == nullptr)
	{
		itemsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		itemsText->bounds = { itemButton->bounds };
		itemsText->SetTextFont(app->fontTTF->defaultFont);
		itemsText->SetString("ITEM(alpha)");
		itemsText->CenterAlign();
	}

	if (escapeText == nullptr)
	{
		escapeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		escapeText->bounds = { escapeButton->bounds };
		escapeText->SetTextFont(app->fontTTF->defaultFont);
		escapeText->SetString("ESCAPE");
		escapeText->CenterAlign();
	}

	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { splitButton->bounds };
		splitText->SetTextFont(app->fontTTF->defaultFont);
		splitText->SetString("SPLIT");
		splitText->CenterAlign();
	}

	if (protectText == nullptr)
	{
		protectText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		protectText->bounds = { protectButton->bounds };
		protectText->SetTextFont(app->fontTTF->defaultFont);
		protectText->SetString("PROTECT");
		protectText->CenterAlign();
	}

	if (buffText == nullptr)
	{
		buffText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		buffText->bounds = { buffButton->bounds };
		buffText->SetTextFont(app->fontTTF->defaultFont);
		buffText->SetString("BUFFS");
		buffText->CenterAlign();
	}

	combatScene->turnText->CenterAlign();
}

void Scene::SetLevelUp(unsigned short int exp)
{

}

void Scene::SetWorld(Places place)
{
	world->Start(place);
}

void Scene::SetPauseMenu()
{
	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;

	if (backToGameButton == nullptr)
	{
		backToGameButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		backToGameButton->bounds = { 640 - buttonPrefab.w / 2 , 350,buttonPrefab.w,buttonPrefab.h };
		backToGameButton->text = "BackToGameButton";
		backToGameButton->SetObserver(this);
	}

	if (saveGameButton == nullptr)
	{
		saveGameButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		saveGameButton->bounds = { 640 - buttonPrefab.w / 2 , 430,buttonPrefab.w,buttonPrefab.h };
		saveGameButton->text = "SaveGameButton";
		saveGameButton->SetObserver(this);
	}

	if (optionsPauseButton == nullptr)
	{
		optionsPauseButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		optionsPauseButton->bounds = { 640 - buttonPrefab.w / 2 , 510, buttonPrefab.w, buttonPrefab.h };
		optionsPauseButton->text = "OptionsPauseButton";
		optionsPauseButton->SetObserver(this);
		optionsPauseButton->state = GuiControlState::LOCKED;
	}

	if (backToMenuButton == nullptr)
	{
		backToMenuButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		backToMenuButton->bounds = { 640 - buttonPrefab.w / 2 , 590, buttonPrefab.w,buttonPrefab.h };
		backToMenuButton->text = "BackToMenuButton";
		backToMenuButton->SetObserver(this);
	}

	if (backToGameText == nullptr)
	{
		backToGameText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		backToGameText->bounds = backToGameButton->bounds;
		backToGameText->SetTextFont(app->fontTTF->defaultFont);
		backToGameText->SetString("BACK TO GAME");
		backToGameText->CenterAlign();
	}

	if (saveGameText == nullptr)
	{
		saveGameText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		saveGameText->bounds = saveGameButton->bounds;
		saveGameText->SetTextFont(app->fontTTF->defaultFont);
		saveGameText->SetString("SAVE GAME");
		saveGameText->CenterAlign();
	}

	if (optionsPauseText == nullptr)
	{
		optionsPauseText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsPauseText->bounds = optionsPauseButton->bounds;
		optionsPauseText->SetTextFont(app->fontTTF->defaultFont);
		optionsPauseText->SetString("OPTIONS");
		optionsPauseText->CenterAlign();
	}

	if (backToMenuText == nullptr)
	{
		backToMenuText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		backToMenuText->bounds = backToMenuButton->bounds;
		backToMenuText->SetTextFont(app->fontTTF->defaultFont);
		backToMenuText->SetString("BACK TO MENU");
		backToMenuText->CenterAlign();
	}
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
// Other Options
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->scene->player1->godMode = !app->scene->player1->godMode;

		if (app->scene->player1->godMode)
			LOG("God mode enabled");
		else
			LOG("God mode disabled");
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		combatScene->secondPlayer = !combatScene->secondPlayer;

		if (combatScene->secondPlayer)
			LOG("Second Player enabled");
		else
			LOG("Second Player disabled");
	}

	app->render->DrawTexture(menu, 0, 0);

	newGameButton->Update(1.0f);
	continueButton->Update(1.0f);
	optionsButton->Update(1.0f);
	exitButton->Update(1.0f);

	newGameButton->Draw();
	continueButton->Draw();
	optionsButton->Draw();
	exitButton->Draw();

	newGameText->Draw();
	continueText->Draw();
	optionsText->Draw();
	exitText->Draw();
}

void Scene::UpdateOptionsMenu()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) SetScene(MAIN_MENU);
}

void Scene::UpdateCombat()
{
	combatScene->Update();

	combatScene->Draw();

	combatScene->EndBattleSolving();

	RestartPressState();

	DebugSteps();
}

void Scene::UpdateLevelUp()
{
	/*if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)*/ SetScene(WORLD, world->place);
}

void Scene::UpdateWorld()
{
	world->Update();

	world->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		prevCam = {app->render->camera.x, app->render->camera.y};
		SetScene(PAUSE_MENU);
	}
}

void Scene::UpdatePauseMenu()
{
	backToGameButton->Update(1.0f);
	saveGameButton->Update(1.0f);
	optionsPauseButton->Update(1.0f);
	backToMenuButton->Update(1.0f);

	backToGameButton->Draw();
	saveGameButton->Draw();
	optionsPauseButton->Draw();
	backToMenuButton->Draw();

	backToGameText->Draw();
	saveGameText->Draw();
	optionsPauseText->Draw();
	backToMenuText->Draw();
}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currScene)
	{
	case LOGO_SCENE:
		break;

	case MAIN_MENU:

		if (strcmp(control->text.GetString(), "NewGameButton") == 0)
		{
			remove("save_game.xml");
			SetScene(WORLD, Places::MAIN_VILLAGE);
			player1->RestartPlayer();
			player2->RestartPlayer();
			world->AlignCameraPosition();
			continueButton->state = GuiControlState::LOCKED;
		}
		else if (strcmp(control->text.GetString(), "ContinueButton") == 0)
		{
			app->LoadGameRequest();
		}
		else if (strcmp(control->text.GetString(), "OptionsButton") == 0) SetScene(OPTIONS_MENU);
		else if (strcmp(control->text.GetString(), "ExitButton") == 0) exit = true;
		break;

	case COMBAT:

		if (strcmp(control->text.GetString(), "AttackButton") == 0) attackPressed = true;
		else if (strcmp(control->text.GetString(), "MoveButton") == 0) movePressed = true;
		else if (strcmp(control->text.GetString(), "ItemButton") == 0) itemPressed = true;
		else if (strcmp(control->text.GetString(), "EscapeButton") == 0) escapePressed = true;
		else if (strcmp(control->text.GetString(), "SplitButton") == 0) splitPressed = true;
		else if (strcmp(control->text.GetString(), "SecondAttackButton") == 0) secondAttackPressed = true;
		else if (strcmp(control->text.GetString(), "ProtectButton") == 0) protectPressed = true;
		else if (strcmp(control->text.GetString(), "BuffButton") == 0) buffPressed = true;
		break;

	case PAUSE_MENU:
		if (strcmp(control->text.GetString(), "BackToGameButton") == 0)
		{
			continuePressed = true;
			SetScene(WORLD, world->place);
			app->render->camera.x = prevCam.x;
			app->render->camera.y = prevCam.y;
			continuePressed = false;
		}
		else if (strcmp(control->text.GetString(), "SaveGameButton") == 0)
		{
			app->SaveGameRequest();
			continueButton->state = GuiControlState::NORMAL;
		}
		else if (strcmp(control->text.GetString(), "OptionsPauseButton") == 0) SetScene(OPTIONS_MENU);
		else if (strcmp(control->text.GetString(), "BackToMenuButton") == 0) SetScene(MAIN_MENU);
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