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
#include "Inventory.h"
#include "LevelUp.h"
#include "OptionsMenu.h"
#include "Map.h"
#include "Pathfinding.h"

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
	sp = false;
	return ret;
}

bool Scene::Start()
{
	app->fontTTF->Load("Assets/Fonts/manaspace.regular.ttf", 14);

	//debugPath = app->tex->Load("Assets/Maps/collisions.png");

	player1 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER1);
	player2 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER2);

	world = new World();

	combatScene = new Combat();

	world->inventory = new Inventory();

	levelUpScene = new LevelUp();

	optionsMenu = new OptionsMenu();

	combatScene->debugCombat = false;
	world->debugCollisions = false;
	app->guiManager->debugGui = false;

	enviroment = GRASSY_LANDS;

	SetScene(LOGO_SCENE);

	////ENEMY SET                                            -----------------COMBAT RECT----------------                        ---WORLD RECT---              LVL EXP  HP STR DEF  VEL
	//app->entityManager->enemies.start->data->SetUp({ SMALLWOLF_C_X, SMALLWOLF_C_Y, SMALLWOLF_C_W, SMALLWOLF_C_H }, {1000, 180, SMALLWOLF_W_W, SMALLWOLF_W_H}, 2, 200, 25, 15, 10, 20);

	if (FILE* file = fopen("save_game.xml", "r"))
	{
		fclose(file);
		activeContinue = true;
	}
	else
	{
		activeContinue = false;
	}
	////DEBUGGING SPLINE////
	pugi::xml_document doc;
	spline.LoadSplines(doc);

	app->render->scale = 1; //Qui toqui aquesta linia de codi, la 98, i m'entero, no viu un dia més :) <3

	return true;
}

bool Scene::PreUpdate()
{
	// L12b: Debug pathfing
	/*static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = world->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || app->input->GetControl(A) == KeyState::KEY_DOWN)
	{
		if(originSelected == true)
		{
			PathFinding::GetInstance()->CreatePath(*debugPathList, origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}*/

	return true;
}

bool Scene::Update(float dt)
{
	// L12b: Debug pathfinding
	/*int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = world->map->WorldToMap(p.x, p.y);
	p = world->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = debugPathList;

	const SDL_Rect debugPathRect = {0, 0, 16, 16};
	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = world->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugPath, pos.x, pos.y, 1, &debugPathRect);
	}*/
	
	//LOG("X:%d Y:%d", player1->colliderWorld.x, player1->colliderWorld.y);
	

	if (currScene == LOGO_SCENE) UpdateLogoScene();
	else if (currScene == MAIN_MENU) UpdateMainMenu();
	else if (currScene == OPTIONS_MENU) UpdateOptionsMenu();
	else if (currScene == COMBAT) UpdateCombat();
	else if (currScene == LEVEL_UP) UpdateLevelUp();
	else if (currScene == WORLD) UpdateWorld();
	else if (currScene == PAUSE_MENU) UpdatePauseMenu();

	//DEBUGGING SPLINE
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		if (sp == true)
			sp = false;
		else
			sp = true;
	}
	if (sp == true)
	{
		spline.DrawSpline(0);
		spline.DrawSplineControlPoints(0);
		spline.HandleInput(0);
	}

	return true;
}

bool Scene::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		combatScene->debugCombat = !combatScene->debugCombat;
		world->debugCollisions = !world->debugCollisions;
		app->guiManager->debugGui = !app->guiManager->debugGui;
	}

	return !exit;
}

bool Scene::CleanUp(Scenes nextScene)
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
		app->tex->UnLoad(app->guiManager->buttonSpriteSheet);
		app->tex->UnLoad(app->guiManager->sliderSpriteSheet);
		app->tex->UnLoad(optionsBackground);
	}
	else if (currScene == COMBAT)
	{
		combatScene->Restart();
	}
	else if (currScene == WORLD)
	{
		world->Restart(nextScene);
		world->inventory->Restart();
	}
	else if (currScene == PAUSE_MENU)
	{
		app->tex->UnLoad(pause);
	}
	else if (currScene == LEVEL_UP)
	{
		app->tex->UnLoad(app->scene->levelUpScene->buttonsTexture);
	}

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	CleanUp(scene);

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == OPTIONS_MENU) SetOptionsMenu();
	else if (scene == COMBAT) SetCombat(nullptr);
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}

void Scene::SetScene(Scenes scene, Enemy* enemy)
{
	CleanUp(scene);

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
	CleanUp(scene);

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == OPTIONS_MENU) SetOptionsMenu();
	else if (scene == COMBAT) SetCombat(nullptr);
	else if (scene == LEVEL_UP) SetLevelUp(0);
	else if (scene == WORLD) SetWorld(place);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}

void Scene::SetScene(Scenes scene, unsigned short int exp)
{
	CleanUp(scene);

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == COMBAT) SetCombat(nullptr);
	else if (scene == LEVEL_UP) SetLevelUp(exp);
	else if (scene == WORLD) SetWorld(Places::NO_PLACE);
	else if (scene == PAUSE_MENU) SetPauseMenu();
}


void Scene::SetLogoScene()
{
	logo = app->tex->Load("Assets/Screens/logo_screen.png");
	timer = 0;
}

void Scene::SetMainMenu()
{
	app->audio->SetFx(Effect::MAIN_MENU_FX);
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
	optionsBackground = app->tex->Load("Assets/Screens/options.png");
	app->guiManager->buttonSpriteSheet = app->tex->Load("Assets/Textures/UI/options.png");
	app->guiManager->sliderSpriteSheet = app->tex->Load("Assets/Textures/UI/slider.png");

	SDL_Rect buttonPrefab = { 740,140 -12,60,60 };
	SDL_Rect textPrefab = { 480,buttonPrefab.y +20 ,130,30 };
	SDL_Rect sliderPrefab = { 470,150,335,40 };
	iPoint off = { 0,85 };
	iPoint sliderOff = { 20,20 };

	if (optionsMenu->dFullScreenCheckBox == nullptr)
	{
		optionsMenu->dFullScreenCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX);
		optionsMenu->dFullScreenCheckBox->bounds = { buttonPrefab.x,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h};
		optionsMenu->dFullScreenCheckBox->text = "DesktopFullScreenCheckBox";
		optionsMenu->dFullScreenCheckBox->SetObserver(this);
	}

	if (optionsMenu->fullScreenCheckBox == nullptr)
	{
		optionsMenu->fullScreenCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX);
		optionsMenu->fullScreenCheckBox->bounds = { buttonPrefab.x,buttonPrefab.y+off.y,buttonPrefab.w,buttonPrefab.h };
		optionsMenu->fullScreenCheckBox->text = "FullScreenCheckBox";
		optionsMenu->fullScreenCheckBox->SetObserver(this);
	}

	if (optionsMenu->vSyncCheckBox == nullptr)
	{
		optionsMenu->vSyncCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX);
		optionsMenu->vSyncCheckBox->bounds = { buttonPrefab.x,buttonPrefab.y + off.y*2,buttonPrefab.w,buttonPrefab.h };
		optionsMenu->vSyncCheckBox->text = "VSyncCheckBox";
		optionsMenu->vSyncCheckBox->SetObserver(this);
	}

	if (optionsMenu->fxVolumeSlider == nullptr)
	{
		optionsMenu->fxVolumeSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
		optionsMenu->fxVolumeSlider->bounds = { sliderPrefab.x,sliderPrefab.y + off.y*4,sliderPrefab.w,sliderPrefab.h };
		optionsMenu->fxVolumeSlider->SetSlider({optionsMenu->fxVolumeSlider->bounds.x + optionsMenu->fxVolumeSlider->bounds.w -sliderOff.x, optionsMenu->fxVolumeSlider->bounds.y-sliderOff.y, buttonPrefab.w, buttonPrefab.h});
		optionsMenu->fxVolumeSlider->text = "FxVolumeSlider";
		optionsMenu->fxVolumeSlider->SetMaxValue(100);
		optionsMenu->fxVolumeSlider->SetMinValue(0);
		optionsMenu->fxVolumeSlider->SetValue(90);
		optionsMenu->fxVolumeSlider->SetObserver(this);
	}

	if (optionsMenu->musicVolumeSlider == nullptr)
	{
		optionsMenu->musicVolumeSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
		optionsMenu->musicVolumeSlider->bounds = { sliderPrefab.x,sliderPrefab.y + off.y*3,sliderPrefab.w,sliderPrefab.h };
		optionsMenu->musicVolumeSlider->SetSlider({ optionsMenu->musicVolumeSlider->bounds.x + optionsMenu->musicVolumeSlider->bounds.w -sliderOff.x, optionsMenu->musicVolumeSlider->bounds.y-sliderOff.y,  buttonPrefab.w, buttonPrefab.h });
		optionsMenu->musicVolumeSlider->text = "MusicVolumeSlider";
		optionsMenu->musicVolumeSlider->SetMaxValue(100);
		optionsMenu->musicVolumeSlider->SetMinValue(0);
		optionsMenu->musicVolumeSlider->SetValue(90);
		optionsMenu->musicVolumeSlider->SetObserver(this);
	}

	if (optionsMenu->returnMenuButton == nullptr)
	{
		optionsMenu->returnMenuButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		optionsMenu->returnMenuButton->bounds = { buttonPrefab.x -175  ,buttonPrefab.y + off.y*5 -15,buttonPrefab.w,buttonPrefab.h };
		optionsMenu->returnMenuButton->text = "ReturnMenuButton";
		optionsMenu->returnMenuButton->SetObserver(this);
		
	}

	if (optionsMenu->fullScreenText == nullptr)
	{
		optionsMenu->fullScreenText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsMenu->fullScreenText->bounds = { textPrefab.x,textPrefab.y,textPrefab.w,textPrefab.h };
		optionsMenu->fullScreenText->SetTextFont(app->fontTTF->defaultFont);
		optionsMenu->fullScreenText->SetString("FULL SCREEN");
	}

	if (optionsMenu->dFullScreenText == nullptr)
	{
		optionsMenu->dFullScreenText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsMenu->dFullScreenText->bounds = { textPrefab.x,textPrefab.y+off.y,textPrefab.w,textPrefab.h };
		optionsMenu->dFullScreenText->SetTextFont(app->fontTTF->defaultFont);
		optionsMenu->dFullScreenText->SetString("D FULL SCREEN");
	}

	if (optionsMenu->vSyncText == nullptr)
	{
		optionsMenu->vSyncText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsMenu->vSyncText->bounds = { textPrefab.x,textPrefab.y+off.y*2,textPrefab.w,textPrefab.h };
		optionsMenu->vSyncText->SetTextFont(app->fontTTF->defaultFont);
		optionsMenu->vSyncText->SetString("V SYNC");
	}

	if (optionsMenu->returnText == nullptr)
	{
		optionsMenu->returnText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		optionsMenu->returnText->bounds = { textPrefab.x,textPrefab.y + off.y * 5 -25,325,textPrefab.h };
		optionsMenu->returnText->SetTextFont(app->fontTTF->defaultFont);
		optionsMenu->returnText->SetString("RETURN");
		optionsMenu->returnText->CenterAlign();
	}

	//optionsMenu->fxVolumeSlider->SetValue(app->audio->VolumeToValue(app->audio->GetFxVolume(), optionsMenu->fxVolumeSlider->GetMaxValue()));
	//optionsMenu->musicVolumeSlider->SetValue(app->audio->VolumeToValue(app->audio->GetMusicVolume(), optionsMenu->musicVolumeSlider->GetMaxValue()));
	//optionsMenu->fxVolumeSlider->UpdatePosition();
	//optionsMenu->musicVolumeSlider->UpdatePosition();
}

void Scene::SetCombat(Enemy* enemySet)
{
	if (combatScene->turnText == nullptr)
	{
		combatScene->turnText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->turnText->bounds = { 541, 43, 200, 50 };
		combatScene->turnText->SetTextFont(app->fontTTF->defaultFont);
	}

	SDL_ShowCursor(0);

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

	// INVENTORY BUTTONS
	if (combatScene->smallMeatButton == nullptr)
	{
		combatScene->smallMeatButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->smallMeatButton->bounds = { 303, 220, 108, 108 };
		combatScene->smallMeatButton->text = "SmallMeatButton";
		combatScene->smallMeatButton->SetObserver(this);
	}

	if (combatScene->largeMeatButton == nullptr)
	{
		combatScene->largeMeatButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->largeMeatButton->bounds = { 303 + (113 * 1), 220, 108, 108 };
		combatScene->largeMeatButton->text = "LargeMeatButton";
		combatScene->largeMeatButton->SetObserver(this);
	}

	if (combatScene->featherButton == nullptr)
	{
		combatScene->featherButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->featherButton->bounds = { 303 + (113 * 2), 220, 108, 108 };
		combatScene->featherButton->text = "FeatherButton";
		combatScene->featherButton->SetObserver(this);
	}

	if (combatScene->mantisButton == nullptr)
	{
		combatScene->mantisButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->mantisButton->bounds = { 303 + (113 * 3), 220, 108, 108 };
		combatScene->mantisButton->text = "MantisButton";
		combatScene->mantisButton->SetObserver(this);
	}

	if (combatScene->enemySplitButton == nullptr)
	{
		combatScene->enemySplitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->enemySplitButton->bounds = { 303 + (113 * 4), 220, 108, 108 };
		combatScene->enemySplitButton->text = "EnemySplitButton";
		combatScene->enemySplitButton->SetObserver(this);
	}

	if (combatScene->moneyButton == nullptr)
	{
		combatScene->moneyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		combatScene->moneyButton->bounds = { 303 + (113 * 5), 220, 108, 108 };
		combatScene->moneyButton->text = "MoneyButton";
		combatScene->moneyButton->SetObserver(this);
	}

	// TEXTS

	if (combatScene->attackText == nullptr)
	{
		combatScene->attackText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->attackText->bounds = attackButton->bounds;
		combatScene->attackText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->attackText->SetString("ATTACK");
		combatScene->attackText->CenterAlign();
	}

	if (combatScene->moveText == nullptr)
	{
		combatScene->moveText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->moveText->bounds = { moveButton->bounds };
		combatScene->moveText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->moveText->SetString("MOVE");
		combatScene->moveText->CenterAlign();
	}

	if (combatScene->itemsText == nullptr)
	{
		combatScene->itemsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->itemsText->bounds = { itemButton->bounds };
		combatScene->itemsText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->itemsText->SetString("ITEMS");
		combatScene->itemsText->CenterAlign();
	}

	if (combatScene->escapeText == nullptr)
	{
		combatScene->escapeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->escapeText->bounds = { escapeButton->bounds };
		combatScene->escapeText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->escapeText->SetString("ESCAPE");
		combatScene->escapeText->CenterAlign();
	}

	if (combatScene->splitText == nullptr)
	{
		combatScene->splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->splitText->bounds = { splitButton->bounds };
		combatScene->splitText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->splitText->SetString("SPLIT");
		combatScene->splitText->CenterAlign();
	}

	if (combatScene->protectText == nullptr)
	{
		combatScene->protectText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->protectText->bounds = { protectButton->bounds };
		combatScene->protectText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->protectText->SetString("PROTECT");
		combatScene->protectText->CenterAlign();
	}

	if (combatScene->buffText == nullptr)
	{
		combatScene->buffText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->buffText->bounds = { buffButton->bounds };
		combatScene->buffText->SetTextFont(app->fontTTF->defaultFont);
		combatScene->buffText->SetString("BUFFS");
		combatScene->buffText->CenterAlign();
	}

	SDL_Rect descriptionRect = { 315, 356, 663, 156 };

	if (combatScene->smallMeatDescription == nullptr)
	{
		combatScene->smallMeatDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->smallMeatDescription->bounds = descriptionRect; //NO
		combatScene->smallMeatDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->smallMeatDescription->SetString("Small Wolf Meat: \n\nFrom the oldest to the youngest, since the start of the times, wolf meat \nhas been the most wanted of all times. One out of one barwo/men \nrecomend this type of meat. It seems that was given to those warriors \nwho came intact from a raid, as a present for their majestry \n\n - USE: Heals the 30 percent of the Max Health of the hero");
	}

	if (combatScene->largeMeatDescription == nullptr)
	{
		combatScene->largeMeatDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->largeMeatDescription->bounds = descriptionRect;
		combatScene->largeMeatDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->largeMeatDescription->SetString("Large Wolf Meat: \n\nWe are not talking about something easy to eat, this meat has the god's \napprovement. It is needed a high quality cheff to admire the flavour \nof this aliment. Either way, it gives the enough vitamins to fight \nduring 12 nights without falling asleep. \n\n - USE: Heals the 60 percent of the Max Health of the hero");
	}

	if (combatScene->featherDescription == nullptr)
	{
		combatScene->featherDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->featherDescription->bounds = descriptionRect;
		combatScene->featherDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->featherDescription->SetString("Slight Feather: \n\nAir is something not visible, but either we can feel it with the wind, which has \nthe power of the destruction of any and enery live structure. \nThis feather is like the air, slight and soft, but has the power to cancel \nthe air friction of every possible wind \n\n - USE: Softens hero's jump and heals a 15 percent of it ");
	}

	if (combatScene->mantisRodDescription == nullptr)
	{
		combatScene->mantisRodDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->mantisRodDescription->bounds = descriptionRect;
		combatScene->mantisRodDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->mantisRodDescription->SetString("Mantis Rod: \n\nFrom the infinite damage of a god attack, life itself needed a way to stop them, \nso it created the infinite defense, which was materialized into \nthe arm of a mantis. But, all good things have an end, that's why \nlife made that arm the most fragile one. What a paradox! \n\n - USE: Makes hero invencible to one enemy hit during one turn");
	}

	if (combatScene->enemySplitDescription == nullptr)
	{
		combatScene->enemySplitDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->enemySplitDescription->bounds = descriptionRect;
		combatScene->enemySplitDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->enemySplitDescription->SetString("Splited Enemy: \n\nFrom the power was concerned to the hero, it has the power to split enemy aura \nand cage it for its sake. We do not know why the hero has the \nability to do that, but we know those auras can be used in our benefit.\n That's his will, and not anyone else, even hero's one. \n\n - USE: Invoque an aliade enemy that will attack your opponent");
	}

	if (combatScene->moneyDescription == nullptr)
	{
		combatScene->moneyDescription = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		combatScene->moneyDescription->bounds = descriptionRect;
		combatScene->moneyDescription->SetTextFont(app->fontTTF->fonts.At(1)->data);
		combatScene->moneyDescription->SetString("Coin: \n\nJust a coin \n\n\n\n\n - USE: Use a coin an loose it");
	}

	app->audio->SetMusic(SoundTrack::MAINCOMBAT_TRACK);
}

void Scene::SetLevelUp(unsigned short int exp)
{
	app->scene->levelUpScene->buttonsTexture = app->tex->Load("Assets/Textures/UI/upgrade-buttons.png");

	SDL_Rect buttonPrefab = { 1000,200,52,55 };
	iPoint off= { 0,61 };

	if (app->scene->levelUpScene->upgradeHealthButton == nullptr)
	{
		app->scene->levelUpScene->upgradeHealthButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeHealthButton->bounds = { buttonPrefab.x ,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeHealthButton->text = "+";
		app->scene->levelUpScene->upgradeHealthButton->SetObserver(this);
	}
	if (app->scene->levelUpScene->upgradeAttackButton == nullptr)
	{
		app->scene->levelUpScene->upgradeAttackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeAttackButton->bounds = { buttonPrefab.x ,buttonPrefab.y +off.y,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeAttackButton->text = "+";
		app->scene->levelUpScene->upgradeAttackButton->SetObserver(this);
	}
	if (app->scene->levelUpScene->upgradeDefenseButton == nullptr)
	{
		app->scene->levelUpScene->upgradeDefenseButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeDefenseButton->bounds = { buttonPrefab.x ,buttonPrefab.y + off.y*2,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeDefenseButton->text = "+";
		app->scene->levelUpScene->upgradeDefenseButton->SetObserver(this);
	}
	if (app->scene->levelUpScene->upgradeSpeedButton == nullptr)
	{
		app->scene->levelUpScene->upgradeSpeedButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeSpeedButton->bounds = { buttonPrefab.x ,buttonPrefab.y +off.y*3,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeSpeedButton->text = "+";
		app->scene->levelUpScene->upgradeSpeedButton->SetObserver(this);
	}
	if (app->scene->levelUpScene->upgradeLuckButton == nullptr)
	{
		app->scene->levelUpScene->upgradeLuckButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeLuckButton->bounds = { buttonPrefab.x ,buttonPrefab.y + off.y*4,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeLuckButton->text = "+";
		app->scene->levelUpScene->upgradeLuckButton->SetObserver(this);
	}
	if (app->scene->levelUpScene->upgradeStabButton == nullptr)
	{
		app->scene->levelUpScene->upgradeStabButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		app->scene->levelUpScene->upgradeStabButton->bounds = { buttonPrefab.x ,buttonPrefab.y + off.y*5,buttonPrefab.w,buttonPrefab.h };
		app->scene->levelUpScene->upgradeStabButton->text = "+";
		app->scene->levelUpScene->upgradeStabButton->SetObserver(this);
	}

	if (app->scene->levelUpScene->upgradePointsText == nullptr)
	{
		app->scene->levelUpScene->upgradePointsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		app->scene->levelUpScene->upgradePointsText->bounds = { 1000,200,200,30 };
		app->scene->levelUpScene->upgradePointsText->SetTextFont(app->fontTTF->defaultFont);
		app->scene->levelUpScene->upgradePointsText->SetString("UPGRADE POINTS: ");
		
	}

	LOG("level up scene buttons init");
}

void Scene::SetWorld(Places place)
{
	world->Start(place);
	world->inventory->Start();

	//inventory buttons
	SDL_Rect buttonPrefab = { 255,606,60,60 };
	if (world->inventory->quitQuestButton == nullptr)
	{
		world->inventory->quitQuestButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->quitQuestButton->bounds = buttonPrefab;
		world->inventory->quitQuestButton->text = "Quit quest";
		world->inventory->quitQuestButton->SetObserver(this);
	}

	buttonPrefab = { 614, 36, 49, 54 };
	if (world->inventory->exitInventoryButton == nullptr)
	{
		world->inventory->exitInventoryButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->exitInventoryButton->bounds = buttonPrefab;
		world->inventory->exitInventoryButton->text = "Exit inventory";
		world->inventory->exitInventoryButton->SetObserver(this);
	}

	//
	buttonPrefab = { 100,150,134,88 };
	iPoint off = { 188,133 };
	if (world->inventory->littlebeefButton == nullptr)
	{
		world->inventory->littlebeefButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->littlebeefButton->bounds = buttonPrefab;
		world->inventory->littlebeefButton->text = "EatSmallMeat";
		world->inventory->littlebeefButton->SetObserver(this);
	}
	if (world->inventory->bigBeefButton == nullptr)
	{
		world->inventory->bigBeefButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->bigBeefButton->bounds = { buttonPrefab.x + off.x,buttonPrefab.y,buttonPrefab.w,buttonPrefab.y };
		world->inventory->bigBeefButton->text = "EatLargeMeat";
		world->inventory->bigBeefButton->SetObserver(this);
	}
	if (world->inventory->featherButton == nullptr)
	{
		world->inventory->featherButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->featherButton->bounds = { buttonPrefab.x ,buttonPrefab.y + off.y,buttonPrefab.w,buttonPrefab.y };
		//world->inventory->littlebeefButton->text = "EatSmallMeat";
		world->inventory->featherButton->SetObserver(this);
	}
	if (world->inventory->mantisButton == nullptr)
	{
		world->inventory->mantisButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->mantisButton->bounds = { buttonPrefab.x + off.x,buttonPrefab.y + off.y,buttonPrefab.w,buttonPrefab.y };
		//world->inventory->exitInventoryButton->text = "Exit inventory";
		world->inventory->mantisButton->SetObserver(this);
	}
	if (world->inventory->coinButton == nullptr)
	{
		world->inventory->coinButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->coinButton->bounds = { buttonPrefab.x,buttonPrefab.y + off.y*2,buttonPrefab.w,buttonPrefab.y };
		//world->inventory->exitInventoryButton->text = "Exit inventory";
		world->inventory->coinButton->SetObserver(this);
	}
	if (world->inventory->splitButton == nullptr)
	{
		world->inventory->splitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		world->inventory->splitButton->bounds = { buttonPrefab.x + off.x,buttonPrefab.y + off.y * 2,buttonPrefab.w,buttonPrefab.y };
		//world->inventory->exitInventoryButton->text = "Exit inventory";
		world->inventory->splitButton->SetObserver(this);
	}
}

void Scene::SetPauseMenu()
{
	app->audio->TransitionVolumeMusic();
	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;
	pause = app->tex->Load("Assets/Screens/pause_menu_screen.png");

	if (backToGameButton == nullptr)
	{
		backToGameButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		backToGameButton->bounds = { 640 - buttonPrefab.w / 2 , 200,buttonPrefab.w,buttonPrefab.h };
		backToGameButton->text = "BackToGameButton";
		backToGameButton->SetObserver(this);
	}

	if (saveGameButton == nullptr)
	{
		saveGameButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		saveGameButton->bounds = { 640 - buttonPrefab.w / 2 , 295,buttonPrefab.w,buttonPrefab.h };
		saveGameButton->text = "SaveGameButton";
		saveGameButton->SetObserver(this);
	}

	if (optionsPauseButton == nullptr)
	{
		optionsPauseButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		optionsPauseButton->bounds = { 640 - buttonPrefab.w / 2 , 382, buttonPrefab.w, buttonPrefab.h };
		optionsPauseButton->text = "OptionsPauseButton";
		optionsPauseButton->SetObserver(this);
		optionsPauseButton->state = GuiControlState::LOCKED;
	}

	if (backToMenuButton == nullptr)
	{
		backToMenuButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		backToMenuButton->bounds = { 640 - buttonPrefab.w / 2 , 482, buttonPrefab.w,buttonPrefab.h };
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
		if (timer == 51) app->audio->SetFx(Effect::LOGO_SCENE_FX);
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
	optionsMenu->Update();
	optionsMenu->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControl(B) == KEY_DOWN || app->input->GetControl(BACK) == KEY_DOWN) SetScene(MAIN_MENU);
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
	SetScene(WORLD, world->place);

	app->scene->levelUpScene->Update();
}

void Scene::UpdateWorld()
{
	world->Update();

	if (world->inventoryOpen) world->inventory->Update();

	if (world->levelUpOpen) levelUpScene->Update();

	world->Draw();

	if (world->inventoryOpen) world->inventory->Draw();

	if (world->levelUpOpen) levelUpScene->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControl(B) == KEY_DOWN || app->input->GetControl(BACK) == KEY_DOWN)
	{
		if (!world->inventoryOpen)
		{
			if (!app->dialogueManager->onDialog)
			{
				prevCam = { app->render->camera.x, app->render->camera.y };
				SetScene(PAUSE_MENU);
			}
		}
		else
		{
			world->inventoryOpen = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player1->godMode = !player1->godMode;
		world->godMode = !world->godMode;

		if (player1->godMode) player1->playerSpeed = 14;
		else player1->playerSpeed = PLAYER_SPEED;
	}
}

void Scene::UpdatePauseMenu()
{
	backToGameButton->Update(1.0f);
	saveGameButton->Update(1.0f);
	optionsPauseButton->Update(1.0f);
	backToMenuButton->Update(1.0f);

	app->render->DrawTexture(pause, 0, 0);

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

	case OPTIONS_MENU:
		if (strcmp(control->text.GetString(), "ReturnMenuButton") == 0)
		{
			if (prevScene == MAIN_MENU) SetScene(MAIN_MENU);
			else if (prevScene == PAUSE_MENU) SetScene(PAUSE_MENU);
		}
		else if (strcmp(control->text.GetString(), "DesktopFullScreenCheckBox") == 0)
		{
			app->win->dFullScreen = !app->win->dFullScreen;
			app->win->SetWinDFullScreen(app->win->dFullScreen);

			if (app->win->dFullScreen) optionsMenu->fullScreenCheckBox->state = GuiControlState::DISABLED;
			else optionsMenu->fullScreenCheckBox->state = GuiControlState::NORMAL;
		}
		else if (strcmp(control->text.GetString(), "FullScreenCheckBox") == 0)
		{
			app->win->fullScreen = !app->win->fullScreen;
			app->win->SetWinFullScreen(app->win->fullScreen);

			if (app->win->fullScreen) optionsMenu->dFullScreenCheckBox->state = GuiControlState::DISABLED;
			else optionsMenu->dFullScreenCheckBox->state = GuiControlState::NORMAL;
		}
		else if (strcmp(control->text.GetString(), "VSyncCheckBox") == 0) app->render->SetVSync(!app->render->vSync);
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
		else if (strcmp(control->text.GetString(), "SmallMeatButton") == 0) smallMeatPressed = true;
		else if (strcmp(control->text.GetString(), "LargeMeatButton") == 0) largeMeatPressed = true;
		else if (strcmp(control->text.GetString(), "FeatherButton") == 0) featherPressed = true;
		else if (strcmp(control->text.GetString(), "Button") == 0) mantisPressed = true;
		else if (strcmp(control->text.GetString(), "EnemySplitButton") == 0) enemySplitPressed = true;
		else if (strcmp(control->text.GetString(), "MoneyButton") == 0) moneyPressed = true;
		break;

	case PAUSE_MENU:
		if (strcmp(control->text.GetString(), "BackToGameButton") == 0)
		{
			continuePressed = true;
			SetScene(WORLD, world->place);
			continuePressed = false;
			app->render->camera.x = prevCam.x;
			app->render->camera.y = prevCam.y;
			app->audio->TransitionVolumeMusic();
		}
		else if (strcmp(control->text.GetString(), "SaveGameButton") == 0)
		{
			app->SaveGameRequest();
			continueButton->state = GuiControlState::NORMAL;
		}
		else if (strcmp(control->text.GetString(), "OptionsPauseButton") == 0) SetScene(OPTIONS_MENU);
		else if (strcmp(control->text.GetString(), "BackToMenuButton") == 0)
		{
			ListItem<Enemy*>* item = app->entityManager->enemies.start;
			while (item != NULL)
			{
				RELEASE(item->data);
				item = item->next;
			}
			app->entityManager->enemies.Clear();
			SetScene(MAIN_MENU);
			app->audio->TransitionVolumeMusic();
		}
		break;

	case WORLD:
		if (strcmp(control->text.GetString(), "EatSmallMeat") == 0)
		{
			player1->smallMeatCount--;
			player1->health += floor((30 * player1->maxHealth) / 100);
			if (player1->health > player1->maxHealth) player1->health = player1->maxHealth;
		}
		else if (strcmp(control->text.GetString(), "EatLargeMeat") == 0)
		{
			player1->largeMeatCount--;
			player1->health += floor((60 * player1->maxHealth) / 100);
			if (player1->health > player1->maxHealth) player1->health = player1->maxHealth;
		}
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
	smallMeatPressed = false;
	largeMeatPressed = false;
	featherPressed = false;
	mantisPressed = false;
	enemySplitPressed = false;
	moneyPressed = false;
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