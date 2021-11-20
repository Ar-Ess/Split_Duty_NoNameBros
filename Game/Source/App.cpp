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
#include "AssetsManager.h"
#include "QuestManager.h"
#include "Scene.h"
#include "Transition.h"
#include "Player.h"
#include "World.h"
#include "Combat.h"
#include "ParticleSystem.h"
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
	assetsManager = new AssetsManager();
	render = new Render();
	tex = new Textures();
	audio = new AudioManager();
	entityManager = new EntityManager();
	guiManager = new GuiManager();
	fontTTF = new FontTTF();
	scene = new Scene();
	dialogueManager = new DialogueManager();
	transition = new Transition();
	questManager = new QuestManager();
	psystem = new ParticleSystem();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(assetsManager);
	AddModule(tex);
	AddModule(audio);
	AddModule(entityManager);
	AddModule(guiManager);
	AddModule(fontTTF);
	AddModule(scene);
	AddModule(dialogueManager);
	AddModule(questManager);
	AddModule(transition);
	AddModule(psystem);

	// Render last to swap buffer
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

App::~App()
{
	// Release modules
	/*ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}*/
	modules.RClear();
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

bool App::LoadGame(Enemy* e)
{
	bool ret = true;

	pugi::xml_document data;
	pugi::xml_parse_result doc = data.load_file("battle_testing.xml");
	pugi::xml_node playerInfo = data.child("BattleState");
	pugi::xml_node worldInfo;
	pugi::xml_node gameProgress;
	Player* p = app->scene->player1;

	if (doc == NULL)
	{
		LOG("There was an arror trying to load the game, take care of save_game.xml file. %s", doc.description());
		ret = false;
	}
	else
	{
		pugi::xml_node stats = playerInfo.child("Player");
		bool lvlNot = stats.child("UsingLevelAsPlayerSetter").attribute("Bool").as_bool();
		int maxHealth = stats.child("max_health").attribute("Int").as_int();
		int strengthStat = stats.child("strength").attribute("Int").as_int();
		int defenseStat = stats.child("defense").attribute("Int").as_int();
		int velocityStat = stats.child("velocity").attribute("Int").as_int();
		int luckStat = stats.child("luck").attribute("Int").as_int();
		int lvl = stats.child("level").attribute("Int").as_int();

		if (lvlNot) p->SetPlayer(lvl);
		else
			p->SetUp(maxHealth, maxHealth, strengthStat, defenseStat, luckStat, velocityStat, 0, lvl, 0);

		stats = playerInfo.child("Enemy");
		lvlNot = stats.child("UsingLevelAsEnemySetter").attribute("Bool").as_bool();
		maxHealth = stats.child("max_health").attribute("Int").as_int();
		strengthStat = stats.child("strength").attribute("Int").as_int();
		defenseStat = stats.child("defense").attribute("Int").as_int();
		velocityStat = stats.child("velocity").attribute("Int").as_int();
		lvl = stats.child("level").attribute("Int").as_int();

		if (lvlNot) e->SetEnemy(lvl);
		else
			e->SetUp(lvl, 0, maxHealth, strengthStat, defenseStat, velocityStat);

		//pugi::xml_node items = playerInfo.child("Items");
		//p1->smallMeatCount = items.attribute("small_meat").as_int();
		//p1->largeMeatCount = items.attribute("large_meat").as_int();
		//p1->featherCount = items.attribute("feather").as_int();
		//p1->mantisRodCount = items.attribute("mantis_rod").as_int();
		//p1->splitedEnemyCount = items.attribute("splited_enemy").as_int();
		//p1->moneyCount = items.attribute("money").as_int();

		LOG("Loading finished...");
	}

	loadGameRequested = false;

	data.reset();

	return ret;
}

EnemyClass App::LoadEnemyClass()
{
	EnemyClass ret = EnemyClass::SMALL_WOLF;

	pugi::xml_document data;
	pugi::xml_parse_result doc = data.load_file("battle_testing.xml");
	pugi::xml_node playerInfo = data.child("BattleState");

	if (doc == NULL)
	{
		LOG("There was an arror trying to load the game, take care of save_game.xml file. %s", doc.description());
	}
	else
	{

		pugi::xml_node stats = playerInfo.child("Enemy");
		int eClass = stats.child("Class").attribute("Int").as_int();

		if (eClass == 2) ret = EnemyClass::BIRD;
		if (eClass == 3) ret = EnemyClass::MANTIS;

		LOG("Loading finished...");
	}

	loadGameRequested = false;

	data.reset();

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document saveDoc;
	pugi::xml_node root = saveDoc.append_child("BattleState");
	pugi::xml_node saveNode;
	pugi::xml_node playerInfo;
	pugi::xml_node enemyInfo;
	pugi::xml_node gameProgress;
	pugi::xml_node questProgress;
	

	if (root != NULL)
	{
		//PLAYER INFO
		saveNode = root.append_child("Player");
		saveNode.append_child("UsingLevelAsPlayerSetter").append_attribute("Bool").set_value(true);
		saveNode.append_child("level").append_attribute("Int").set_value(10);
		saveNode.append_child("max_health").append_attribute("Int").set_value(10);
		saveNode.append_child("strength").append_attribute("Int").set_value(5);
		saveNode.append_child("defense").append_attribute("Int").set_value(5);
		saveNode.append_child("velocity").append_attribute("Int").set_value(5);
		saveNode.append_child("luck").append_attribute("Int").set_value(5);

		saveNode = root.append_child("Enemy");
		saveNode.append_child("Class").append_attribute("Int").set_value(1);
		saveNode.append_child("UsingLevelAsEnemySetter").append_attribute("Bool").set_value(true);
		saveNode.append_child("level").append_attribute("Int").set_value(10);
		saveNode.append_child("max_health").append_attribute("Int").set_value(10);
		saveNode.append_child("strength").append_attribute("Int").set_value(5);
		saveNode.append_child("defense").append_attribute("Int").set_value(5);
		saveNode.append_child("velocity").append_attribute("Int").set_value(5);
		saveNode.append_child("luck").append_attribute("Int").set_value(5);


		//playerInfo = saveNode.append_child("Items");
		//playerInfo.append_attribute("small_meat").set_value(scene->player1->smallMeatCount);
		//playerInfo.append_attribute("large_meat").set_value(scene->player1->largeMeatCount);
		//playerInfo.append_attribute("feather").set_value(scene->player1->featherCount);
		//playerInfo.append_attribute("mantis_rod").set_value(scene->player1->mantisRodCount);
		//playerInfo.append_attribute("splited_enemy").set_value(scene->player1->splitedEnemyCount);
		//playerInfo.append_attribute("money").set_value(scene->player1->moneyCount);

		saveDoc.save_file("battle_testing.xml");
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
