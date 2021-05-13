#include "App.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "FontTTF.h"
#include "DialogueManager.h"
#include "Scene.h"
#include "Transition.h"
#include "Player.h"
#include "World.h"
#include "Combat.h"
#include <time.h>

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new AudioManager();
	entityManager = new EntityManager();
	guiManager = new GuiManager();
	fontTTF = new FontTTF();
	scene = new Scene();
	dialogueManager = new DialogueManager();
	transition = new Transition();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(entityManager);
	AddModule(guiManager);
	AddModule(fontTTF);
	AddModule(scene);
	AddModule(dialogueManager);
	AddModule(transition);

	// Render last to swap buffer
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		int cap = configApp.attribute("framerate_cap").as_int(-1);
		if (cap > 0) cappedMs = 1000 / cap;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	
	PERF_PEEK(ptimer);

	configFile.reset();

	return ret;
}

bool App::Start()
{
	PERF_START(ptimer);
	
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();

	srand(time(NULL));

	return ret;
}

pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

void App::PrepareUpdate()
{
    frameCount++;
    lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
    
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
			  averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

	if (frameDelay > lastFrameMs)
	{
		SDL_Delay(frameDelay - lastFrameMs);
	}
}

bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.GetString();
}

const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadGameRequest()
{
	loadGameRequested = true;
}

void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_document data;
	pugi::xml_parse_result doc = data.load_file("save_game.xml");
	pugi::xml_node playerInfo;
	pugi::xml_node worldInfo;
	Player* p1 = app->scene->player1;
	Player* p2 = app->scene->player2;
	World* w = app->scene->world;

	if (doc == NULL)
	{
		LOG("There was an arror trying to load the game, take care of save_game.xml file. %s", doc.description());
		ret = false;
	}
	else
	{
		playerInfo = data.child("saveState").child("Player_Information");
		pugi::xml_node pos = playerInfo.child("Position");
		p1->colliderWorld.x = pos.attribute("x_world").as_int();
		p1->colliderWorld.y = pos.attribute("y_world").as_int();
		p1->colliderWorld.w = pos.attribute("w_world").as_int();
		p1->colliderWorld.h = pos.attribute("h_world").as_int();
		p1->collisionRect.x = pos.attribute("x_coll").as_int();
		p1->collisionRect.y = pos.attribute("y_coll").as_int();
		p1->collisionRect.w = pos.attribute("w_coll").as_int();
		p1->collisionRect.h = pos.attribute("h_coll").as_int();
		p1->playerSpeed = pos.attribute("player_speed").as_int();

		pugi::xml_node stats = playerInfo.child("Player1_Stats");
		p1->health = stats.attribute("health").as_int();
		p1->maxHealth = stats.attribute("max_health").as_int();
		p1->strengthStat = stats.attribute("strength").as_int();
		p1->defenseStat = stats.attribute("defense").as_int();
		p1->velocityStat = stats.attribute("velocity").as_int();
		p1->luckStat = stats.attribute("luck").as_int();
		p1->stabStat = stats.attribute("stab").as_int();
		p1->lvl = stats.attribute("level").as_int();
		p1->exp = stats.attribute("experience").as_int();

		stats = playerInfo.child("Player2_Stats");
		app->scene->combatScene->secondPlayer = stats.attribute("exists").as_bool();
		p2->health = stats.attribute("health").as_int();
		p2->maxHealth = stats.attribute("max_health").as_int();
		p2->strengthStat = stats.attribute("strength").as_int();
		p2->defenseStat = stats.attribute("defense").as_int();
		p2->lvl = stats.attribute("level").as_int();
		p2->exp = stats.attribute("experience").as_int();

		pugi::xml_node items = playerInfo.child("Items");
		p1->smallMeatCount = stats.attribute("small_meat").as_int();
		p1->largeMeatCount = stats.attribute("large_meat").as_int();
		p1->featherCount = stats.attribute("feather").as_int();
		p1->mantisRodCount = stats.attribute("mantis_rod").as_int();
		p1->splitedEnemyCount = stats.attribute("splited_enemy").as_int();
		p1->moneyCount = stats.attribute("money").as_int();

		worldInfo = data.child("saveState").child("World_Information");
		pugi::xml_node world = worldInfo.child("Position");
		w->SetPrevPosition(world.attribute("previous_x").as_int(), world.attribute("previous_y").as_int());
		app->render->camera.x = world.attribute("cam_x").as_int();
		app->render->camera.y = world.attribute("cam_y").as_int();

		world = worldInfo.child("Place");
		w->SetPlace(world.attribute("place").as_int());

		LOG("Loading finished...");
	}

	loadGameRequested = false;

	app->scene->continuePressed = true;
	app->scene->SetScene(WORLD, w->GetPlace());
	app->scene->continuePressed = false;

	app->scene->world->AlignCameraPosition();

	data.reset();

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document saveDoc;
	pugi::xml_node root = saveDoc.append_child("saveState");
	pugi::xml_node saveNode;
	pugi::xml_node playerInfo;
	pugi::xml_node worldInfo;

	if (root != NULL)
	{
		//PLAYER INFO
		saveNode = root.append_child("Player_Information");
		playerInfo = saveNode.append_child("Position");
		playerInfo.append_attribute("x_world").set_value(scene->player1->colliderWorld.x);
		playerInfo.append_attribute("y_world").set_value(scene->player1->colliderWorld.y);
		playerInfo.append_attribute("w_world").set_value(scene->player1->colliderWorld.w);
		playerInfo.append_attribute("h_world").set_value(scene->player1->colliderWorld.h);
		playerInfo.append_attribute("x_coll").set_value(scene->player1->collisionRect.x);
		playerInfo.append_attribute("y_coll").set_value(scene->player1->collisionRect.y);
		playerInfo.append_attribute("w_coll").set_value(scene->player1->collisionRect.w);
		playerInfo.append_attribute("h_coll").set_value(scene->player1->collisionRect.h);
		playerInfo.append_attribute("player_speed").set_value(scene->player1->playerSpeed);
		playerInfo = saveNode.append_child("Player1_Stats");
		playerInfo.append_attribute("health").set_value(scene->player1->health);
		playerInfo.append_attribute("max_health").set_value(scene->player1->maxHealth);
		playerInfo.append_attribute("strength").set_value(scene->player1->strengthStat);
		playerInfo.append_attribute("defense").set_value(scene->player1->defenseStat);
		playerInfo.append_attribute("velocity").set_value(scene->player1->velocityStat);
		playerInfo.append_attribute("luck").set_value(scene->player1->luckStat);
		playerInfo.append_attribute("stab").set_value(scene->player1->stabStat);
		playerInfo.append_attribute("level").set_value(scene->player1->lvl);
		playerInfo.append_attribute("experience").set_value(scene->player1->exp);
		playerInfo = saveNode.append_child("Player2_Stats");
		playerInfo.append_attribute("exists").set_value(scene->combatScene->secondPlayer);
		playerInfo.append_attribute("health").set_value(scene->player2->health);
		playerInfo.append_attribute("max_health").set_value(scene->player2->maxHealth);
		playerInfo.append_attribute("strength").set_value(scene->player2->strengthStat);
		playerInfo.append_attribute("defense").set_value(scene->player2->defenseStat);
		playerInfo.append_attribute("level").set_value(scene->player2->lvl);
		playerInfo.append_attribute("experience").set_value(scene->player2->exp);
		playerInfo = saveNode.append_child("Items");
		playerInfo.append_attribute("small_meat").set_value(scene->player1->smallMeatCount);
		playerInfo.append_attribute("large_meat").set_value(scene->player1->largeMeatCount);
		playerInfo.append_attribute("feather").set_value(scene->player1->featherCount);
		playerInfo.append_attribute("mantis_rod").set_value(scene->player1->mantisRodCount);
		playerInfo.append_attribute("splited_enemy").set_value(scene->player1->splitedEnemyCount);
		playerInfo.append_attribute("money").set_value(scene->player1->moneyCount);

		//WORLD INFO
		saveNode = root.append_child("World_Information");
		worldInfo = saveNode.append_child("Position");
		worldInfo.append_attribute("cam_x").set_value(render->camera.x);
		worldInfo.append_attribute("cam_y").set_value(render->camera.y);
		worldInfo.append_attribute("previous_x").set_value(scene->world->GetPreviousPosition().x);
		worldInfo.append_attribute("previous_y").set_value(scene->world->GetPreviousPosition().y);
		worldInfo.append_attribute("world_speed").set_value(scene->world->GetWorldSpeed());
		worldInfo = saveNode.append_child("Place");
		worldInfo.append_attribute("place").set_value(scene->world->GetPlace());

		saveDoc.save_file("save_game.xml");
		LOG("Game saved correctly");
	}
	else
	{
		LOG("Error on append child of the save_game.xml file");
		ret = false;
	}

	saveDoc.reset();

	saveGameRequested = false;

	return ret;
}
