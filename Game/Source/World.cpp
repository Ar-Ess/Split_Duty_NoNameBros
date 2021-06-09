#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "GuiManager.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "Input.h"
#include "DialogueManager.h"

#include "World.h"
#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "LilipadPuzzle.h"
#include "StonePuzzle.h"
#include "ButtonsPuzzle.h"
#include "Inventory.h"
#include "LevelUp.h"
#include "Audio.h"
#include "Combat.h"

#include "Log.h"
#include <time.h>

World::World()
{
	map = new Map();
	place = NO_PLACE;
	prevPosition = {0, 0};
	dialogs = new DialogueManager();
}

void World::Start(Places placex)
{
	Places prevPlace = place;
	place = placex;
	Player* p = app->scene->player1;

	if (placex == MAIN_VILLAGE)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("main_village.tmx");
		app->scene->enviroment = Environments::GRASSY_LANDS;

		if (!app->scene->continuePressed)
		{
			if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor1House))
			{
				p->colliderWorld = { 420, 636, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 420, 636 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor2House))
			{
				p->colliderWorld = { 952, 636, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 952, 636 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor3House))
			{
				p->colliderWorld = { 1428, 720, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 1428, 720 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor4House))
			{
				p->colliderWorld = { 2688, 1812, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 2688, 1812 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor5House))
			{
				p->colliderWorld = { 2072, 1812, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 2072, 1812 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor6House))
			{
				p->colliderWorld = { 3108, 1280, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 3108, 1280 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x + 54, prevPosition.y, 1, 1 }, sensorFieldVillage1))
			{
				p->colliderWorld = { 35, 175, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 35, 175 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x + 54, prevPosition.y, 1, 1 }, sensorFieldVillage2))
			{
				p->colliderWorld = { 35, 1380, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 35, 1380 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}

			if (prevPlace == GOLEM_STONES)
			{
				p->colliderWorld = { 3150, 200, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 3150, 200 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == MOSSY_ROCKS_1)
			{
				p->colliderWorld = { 1736, 1932, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 1736, 1932 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == SHOP)
			{
				p->colliderWorld = { 32 * 28, 43 * 28, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 32 * 28, (43 * 28) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);
	}
	else if (placex == HOUSE)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("house.tmx");

		if (!app->scene->continuePressed)
		{
			p->colliderWorld = { 280, 350, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
			p->collisionRect = { 280, 350 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
		}

		app->render->camera.x = 332;
		app->render->camera.y = 100;
	}
	else if (placex == TAVERN)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("tavern.tmx");

		// IF WE COME FROM CONTINUE BUTTON CLICKED, DONT ENTER HERE
		if (!app->scene->continuePressed)
		{
			p->colliderWorld = { 504, 880, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
			p->collisionRect = { 504, 880 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			app->render->camera.y = -375;
		}

		app->render->camera.x = 140;

		RectifyCameraPosition(placex);
	}
	else if (placex == SHOP)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("shop.tmx");

		// IF WE COME FROM CONTINUE BUTTON CLICKED, DONT ENTER HERE
		if (!app->scene->continuePressed)
		{
			p->colliderWorld = { 504, 880, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
			p->collisionRect = { 504, 880 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			app->render->camera.y = -375;
		}

		for (int i = 0; i < 4; i++)
		{
			shopProduct[i].w = 5 * 28;
			shopProduct[i].h = 4 * 28;
		}

		shopProduct[0].x = 9 * 28;
		shopProduct[0].y = 15 * 28;

		shopProduct[1].x = 9 * 28;
		shopProduct[1].y = 24 * 28;
		
		shopProduct[2].x = 25 * 28;
		shopProduct[2].y = 15 * 28;
		
		shopProduct[3].x = 25 * 28;
		shopProduct[3].y = 24 * 28;

		app->render->camera.x = 140;

		questionMark = app->tex->Load("Assets/Textures/UI/question_mark_symbol.png");

		RectifyCameraPosition(placex);
	}
	else if (placex == ENEMY_FIELD)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("graveyard.tmx");

		app->scene->enviroment = Environments::GRASSY_LANDS;

		// IF WE COME FROM CONTINUE BUTTON CLICKED, DONT ENTER HERE
		if (!app->scene->continuePressed)
		{
			if (prevPlace == MAIN_VILLAGE)
			{
				if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensorVillageField1))
				{
					p->colliderWorld = { 1580, 150, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
					p->collisionRect = { 1580, 150 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };

					AlignCameraPosition();
					RectifyCameraPosition(placex);

					app->render->camera.y = -140;

					p->colliderWorld.y += 140;
					p->collisionRect.y += 140;
				}
				else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensorVillageField2))
				{
					p->colliderWorld = { 1580, 150, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
					p->collisionRect = { 1580, 150 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };

					AlignCameraPosition();

					RectifyCameraPosition(placex);

					app->render->camera.y = -1216;

					p->colliderWorld.y += 1356; //1316
					p->collisionRect.y += 1356; //1316
				}
			}
			else if (prevPlace == GRASSY_LAND_2)
			{
				p->colliderWorld = { 812, 50, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 812, 50 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == AUTUM_FALL_1)
			{
				p->colliderWorld = { 28 * 3, 28 * 72, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 28 * 3, (28 * 72) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		leaves = app->tex->Load("Assets/Textures/Environment/leafs.png");
	}
	else if (placex == GRASSY_LAND_1)
	{
		app->audio->SetMusic(SoundTrack::GRASSYLANDS_TRACK);

		map->Load("grassy_lands_1.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == ENEMY_FIELD)
			{
				p->colliderWorld = { 1080, 2640, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 1080, 2640 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == GRASSY_LAND_2)
			{
				p->colliderWorld = { 1784, 64, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 1784, 64 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
	}
	else if (placex == GRASSY_LAND_2)
	{
		app->audio->SetMusic(SoundTrack::GRASSYLANDS_TRACK);

		if (lilipadPuzzle1 == nullptr)
		{
			lilipadPuzzle1 = new LilipadPuzzle(2, { 840 + 14, 1092 + 14, 28, 28 }, { 980 + 14, 1092 + 14, 28, 28 });
			lilipadPuzzle1->lilipad.At(0)->data.SetPosition(644, 924);
			lilipadPuzzle1->lilipad.At(1)->data.SetPosition(252, 616);
		}

		if (stonePuzzle1 == nullptr)
		{
			stonePuzzle1 = new StonePuzzle(1, { 280, 896, 112, 84 });
			stonePuzzle1->stone.At(0)->data.SetPosition(504, 728);
		}

		if (buttonPuzzle1 == nullptr)
		{
			buttonPuzzle1 = new ButtonPuzzle(4, 28, 28, { 392, 308, 56, 56 }, { 616, 308, 56, 56 }, { 840, 308, 56, 56 });
			buttonPuzzle1->button.At(0)->data.SetPosition(196 + 14, 1064 + 14, 1);
			buttonPuzzle1->button.At(1)->data.SetPosition(336 + 14, 1148 + 14, 2);
			buttonPuzzle1->button.At(2)->data.SetPosition(392 + 14, 1372 + 14, 3);
			buttonPuzzle1->button.At(3)->data.SetPosition(224 + 14, 1288 + 14, 4);
		}

		map->Load("grassy_lands_2.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == GRASSY_LAND_1)
			{
				p->colliderWorld = { 616, 1390, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 616, 1390 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == GRASSY_LAND_3)
			{
				p->colliderWorld = { 616, 84, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 616, 84 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
				buttonPuzzle1->resultActive = true;
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);
	}
	else if (placex == GRASSY_LAND_3)
	{
		app->audio->SetMusic(SoundTrack::GRASSYLANDS_TRACK);

		map->Load("grassy_lands_3.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == GRASSY_LAND_2)
			{
				p->colliderWorld = { 1098, 2380, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 1098, 2380 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		stomp = app->tex->Load("Assets/Textures/Environment/stumps.png");
		statItems = app->tex->Load("Assets/Textures/UI/collectible_items.png");
	}
	else if (placex == AUTUM_FALL_1)
	{
		app->scene->enviroment = Environments::AUTUM_FALLS;

		//app->audio->SetMusic(SoundTrack::AUTUM_FALLS_TRACK);

		map->Load("autumn_falls_1.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == ENEMY_FIELD)
			{
				p->colliderWorld = { 136 * 28, 28 * 49, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 136 * 28, (28 * 49) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == AUTUM_FALL_2)
			{
				p->colliderWorld = { 3 * 28, 28 * 72, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 3 * 28, (28 * 72) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
	}
	else if (placex == AUTUM_FALL_2)
	{
		//app->audio->SetMusic(SoundTrack::AUTUM_FALLS_TRACK);

		map->Load("autumn_falls_2.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == AUTUM_FALL_1)
			{
				p->colliderWorld = { 28 * 116, 28 * 82, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 28 * 116, (28 * 82) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		stomp = app->tex->Load("Assets/Textures/Environment/stumps.png");
		statItems = app->tex->Load("Assets/Textures/UI/collectible_items.png");
	}
	else if (placex == MOSSY_ROCKS_1)
	{
		app->scene->enviroment = Environments::MOSSY_LANDS;

		//app->audio->SetMusic(SoundTrack::MOSSY_ROCKS_TRACK);

		map->Load("mossy_rocks_1.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == MAIN_VILLAGE)
			{
				p->colliderWorld = { 28 * 31, 28 * 2, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 28 * 31, (28 * 2) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == MOSSY_ROCKS_2)
			{
				p->colliderWorld = { 57 * 28, 28 * 18, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 57 * 28, (28 * 18) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		stomp = app->tex->Load("Assets/Textures/Environment/stumps.png");
	}
	else if (placex == MOSSY_ROCKS_2)
	{

		//app->audio->SetMusic(SoundTrack::MOSSY_ROCKS_TRACK);

		map->Load("mossy_rocks_2.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == MOSSY_ROCKS_1)
			{
				p->colliderWorld = { 2 * 28, 28 * 25, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 2 * 28, (28 * 25) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		stomp = app->tex->Load("Assets/Textures/Environment/stumps.png");
		statItems = app->tex->Load("Assets/Textures/UI/collectible_items.png");
	}
	else if (placex == GOLEM_STONES)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("golem_sanctuary.tmx");
		app->scene->enviroment = Environments::BOSS_LANDS;

		if (!app->scene->continuePressed)
		{
			if (prevPlace == MAIN_VILLAGE)
			{
				p->colliderWorld = { 25, 800, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 25, 800 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
			else if (prevPlace == CAVE)
			{
				p->colliderWorld = { 46 * 28, 4 * 28, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 46 * 28, (4 * 28) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		if (gameStart) night = app->tex->Load("Assets/Textures/Environment/darkness2.png");

		AlignCameraPosition();

		RectifyCameraPosition(placex);
	}
	else if (placex == CAVE)
	{
		map->Load("cave.tmx");

		if (!app->scene->continuePressed)
		{
			if (prevPlace == GOLEM_STONES)
			{
				p->colliderWorld = { 28 * 22, 28 * 27, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { 28 * 22, (28 * 27) + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
			}
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);
	}

	walkingSpritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/walking_spritesheet.png");
	interactionText = app->tex->Load("Assets/Textures/UI/interact_texture.png");

	LoadNPCs(placex);

	currentPlayerAnimation = &p->walkDownAnim;

	UpdateWorldSpeed();

	if (placex == CAVE) app->render->camera.x -= 4;

	p = nullptr;
}

void World::Restart(Scenes scene)
{
	map->CleanUp();

	houses.Clear();

	tavern.Clear();

	shop.Clear();

	collisions.Clear();

	location1.Clear();

	location2.Clear();

	location3.Clear();

	collisionsOnOff.Clear();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	if (night != nullptr) app->tex->UnLoad(night);

	if (wolfSpritesheet != nullptr) app->tex->UnLoad(wolfSpritesheet);
	if (birdSpritesheet != nullptr)app->tex->UnLoad(birdSpritesheet);
	if (mantisSpritesheet != nullptr)app->tex->UnLoad(mantisSpritesheet);
	if (leaves != nullptr)app->tex->UnLoad(leaves);
	if (stomp != nullptr)app->tex->UnLoad(stomp);
	if (statItems != nullptr)app->tex->UnLoad(statItems);
	if (questionMark != nullptr) app->tex->UnLoad(questionMark);

	if (walkingSpritesheet != nullptr) app->tex->UnLoad(walkingSpritesheet);
	if (interactionText != nullptr) app->tex->UnLoad(interactionText);
	
	if (scene == WORLD)
	{
		ListItem<Enemy*>* item = app->entityManager->enemies.start;
		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		app->entityManager->enemies.Clear();
	}

	ListItem<NPC*>* item1 = app->entityManager->NPCs.start;
	while (item1 != NULL)
	{
		item1->data->Delete();
		RELEASE(item1->data);
		item1 = item1->next;
	}
	app->entityManager->NPCs.Clear();

	if (lilipadPuzzle1 != nullptr && !lilipadPuzzle1->finished)
	{
		lilipadPuzzle1->Restart();
		delete lilipadPuzzle1;
		lilipadPuzzle1 = nullptr;
	}

	if (stonePuzzle1 != nullptr && !stonePuzzle1->finished)
	{
		stonePuzzle1->Restart();
		delete stonePuzzle1;
		stonePuzzle1 = nullptr;
	}

	if (buttonPuzzle1 != nullptr && !buttonPuzzle1->finished)
	{
		buttonPuzzle1->Restart();
		delete buttonPuzzle1;
		buttonPuzzle1 = nullptr;
	}
}

void World::Update()
{
	if (!app->dialogueManager->onDialog && !inventoryOpen) WorldMovement();

	LOG("X:%d, Y:%d", app->scene->player1->colliderWorld.x, app->scene->player1->colliderWorld.y);

	WorldChange();

	if (playerInmuneTime > 0)
	{
		PlayerInmunityLogic();
	}

	EnemyLogic();

	NPCLogic();

	if (place == SHOP) StoreLogic();

	WorldStatGet();

	if (place == GRASSY_LAND_2)
	{
		lilipadPuzzle1->Update();
		stonePuzzle1->Update();
		buttonPuzzle1->Update();
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetControl(Y) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (!app->dialogueManager->onDialog && !gameStart)
		{
			inventoryOpen = !inventoryOpen;
			if (inventoryOpen) inventory->Start();
			else inventory->Restart();
			app->audio->SetFx(Effect::INVENTORY_OPEN_FX);
		}
	}

	if (app->scene->player1->stabStat > 0) WorldStabLogic();

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->scene->player1->stabStat = 25;

	if (place == GOLEM_STONES) GolemCall();

	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN || app->input->GetControl(R3) == KEY_DOWN)
		if (!app->dialogueManager->onDialog && !inventoryOpen) app->dialogueManager->StartDialogue(7);
}

void World::EnemyLogic()
{
	WorldEnemySpawn();

	if (app->entityManager->enemies.Count() > 0)
	{
		WorldEnemyChasing();
		if (!playerInmunity) WorldEnemyDetection();
	}
}

void World::Draw()
{
	map->Draw();

	DrawObstacles();

	if (debugCollisions) DrawCollisions();

	if (place == HOUSE || place == TAVERN)
	{
		DrawNPC();
		DrawEnemy();
		DrawPlayer();
	}

	DrawText();

	DrawFilters();

	if (interactionActive && !app->dialogueManager->onDialog)
	{
		DrawInteraction();
		interactionActive = false;
	}

	return;
}

void World::DrawPlayer()
{
	Player* p = app->scene->player1;
	currentPlayerAnimation->Update(1.0f);
	if (!playerInmunity) app->render->DrawTexture(walkingSpritesheet, p->collisionRect.x, p->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
	else
	{
		if (app->GetFrameCount() % 2 == 0)
		{
			app->render->DrawTexture(walkingSpritesheet, p->collisionRect.x, p->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
		}
	}
}

void World::DrawEnemy()
{
	if (app->entityManager->enemies.Count())
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* enemy = app->entityManager->enemies[i];
			if (enemy->active)
			{
				currentEnemyAnimation = &enemy->idleAnim;
				currentEnemyAnimation->Update(1.0f);
				if (enemy->GetClass() == EnemyClass::SMALL_WOLF)
				{
					if (wolfReverse) app->render->DrawTexture(wolfSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y - 20, SCALE, &currentEnemyAnimation->GetCurrentFrame(), false, 0, SDL_FLIP_HORIZONTAL);
					else app->render->DrawTexture(wolfSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y - 20, SCALE, &currentEnemyAnimation->GetCurrentFrame(), false, 0, SDL_FLIP_NONE);
					app->render->DrawRectangle({ enemy->colliderWorld.x + 5, enemy->colliderWorld.y - 13, 100, 20 }, {0, 0, 0, 175});
					enemy->lvlText->bounds = { enemy->colliderWorld.x + 20 + app->render->camera.x, enemy->colliderWorld.y + app->render->camera.y - 10, 20, 20 };
					enemy->lvlText->Draw();
				}
				if (enemy->GetClass() == EnemyClass::BIRD)
				{

					app->render->DrawTexture(birdSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y-20, SCALE*1.5f, &currentEnemyAnimation->GetCurrentFrame(), false);
					app->render->DrawRectangle({ enemy->colliderWorld.x - 5, enemy->colliderWorld.y - 43, 100, 20 }, { 0, 0, 0, 175 });
					enemy->lvlText->bounds = { enemy->colliderWorld.x + 10 + app->render->camera.x, enemy->colliderWorld.y + app->render->camera.y - 40, 20, 20 };
					enemy->lvlText->Draw();
				}
				if (enemy->GetClass() == EnemyClass::MANTIS)
				{
					app->render->DrawTexture(mantisSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y-26-20, SCALE*1.5f, &currentEnemyAnimation->GetCurrentFrame(), false);
					app->render->DrawRectangle({ enemy->colliderWorld.x - 5, enemy->colliderWorld.y - 58, 100, 20 }, { 0, 0, 0, 175 });
					enemy->lvlText->bounds = { enemy->colliderWorld.x + 10 + app->render->camera.x, enemy->colliderWorld.y + app->render->camera.y - 55, 20, 20 };
					enemy->lvlText->Draw();
				}
			}
			enemy = nullptr;
		}
	}
}

void World::DrawNPC()
{
	for (int i = 0; i < app->entityManager->NPCs.Count(); i++)
	{
		NPC* npc = app->entityManager->NPCs[i];
		if (npc->place == GetPlace())
		{
			npc->Draw();
		}
		npc = nullptr;
	}
}

void World::DrawText()
{
	if (drawRecomendedII)
	{
		app->render->DrawRectangle({ 1250, 210, 130, 50 }, { 0, 0, 0, 175 });
		lvlRecomendedText->Draw();
	}
	else if (drawRecomendedIII)
	{
		app->render->DrawRectangle({ 1480, 290, 130, 50 }, {0, 0, 0, 175});
		lvlRecomendedText->Draw();
	}

	if (place == SHOP)
	{
		shopPriceText->bounds = {(11 * 28) + 10 + app->render->camera.x, (17 * 28) + 5 + app->render->camera.y, 10, 10};
		shopPriceText->SetString("6", BLACK);
		shopPriceText->Draw();

		shopPriceText->bounds = { (11 * 28) + app->render->camera.x, (26 * 28) + 5 + app->render->camera.y, 10, 10 };
		shopPriceText->SetString("12", BLACK);
		shopPriceText->Draw();

		if (app->scene->boss1Beat)
		{
			shopPriceText->bounds = { (27 * 28) + app->render->camera.x, (17 * 28) + 5 + app->render->camera.y, 10, 10 };
			shopPriceText->SetString("10", BLACK);
			shopPriceText->Draw();
		}

		if (app->scene->boss2Beat)
		{
			shopPriceText->bounds = { (27 * 28) + app->render->camera.x, (26 * 28) + 5 + app->render->camera.y, 10, 10 };
			shopPriceText->SetString("14", BLACK);
			shopPriceText->Draw();
		}
	}
}

void World::DrawObstacles()
{
	if (place == ENEMY_FIELD && !app->scene->boss1Beat)
	{
		app->render->DrawTexture(leaves, leavesCollision.x, leavesCollision.y, 3.5f, 3.5f, false, &bush, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
	else if (place == MOSSY_ROCKS_1 && !app->scene->boss2Beat)
	{
		app->render->DrawTexture(stomp, logCollision.x, logCollision.y, 0.7f, 0.7f, false, &fallenWoodRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
}

void World::DrawCollisions()
{
	Player* p = app->scene->player1;

	if (houses.Count() > 0) for (int i = 0; i < houses.Count(); i++) app->render->DrawRectangle(houses[i], { 255, 0, 255, 100 });
	for (int i = 0; i < collisions.Count(); i++) app->render->DrawRectangle(collisions[i], { 255, 0, 0, 100 });
	if (location1.Count() > 0) for (int i = 0; i < location1.Count(); i++) app->render->DrawRectangle(location1[i], { 255, 255, 0, 100 });
	if (location2.Count() > 0) for (int i = 0; i < location2.Count(); i++) app->render->DrawRectangle(location2[i], { 0, 255, 0, 100 });
	if (location3.Count() > 0) for (int i = 0; i < location3.Count(); i++) app->render->DrawRectangle(location3[i], { 0, 0, 255, 100 });
	if (tavern.Count() > 0) for (int i = 0; i < tavern.Count(); i++) app->render->DrawRectangle(tavern[i], { 0, 0, 255, 100 });
	if (shop.Count() > 0) for (int i = 0; i < shop.Count(); i++) app->render->DrawRectangle(shop[i], { 0, 0, 255, 100 });

	app->render->DrawRectangle(app->scene->player1->GetWorldBounds(), { 100, 150, 240, 150 });
	app->render->DrawRectangle(app->scene->player1->GetCollisionBounds(), { 150, 150, 140, 200 });

	for (int i = 0; i < app->entityManager->NPCs.Count() ; i++)
	{
		NPC* npc = app->entityManager->NPCs[i];
		if (npc->place == place)
		{
			app->render->DrawRectangle(npc->collider, { 50, 200, 50, 150 });
			app->render->DrawRectangle(npc->contactCollider, { 100, 250, 100, 150 });
		}
		npc = nullptr;
	}

	if (place == ENEMY_FIELD && !app->scene->boss1Beat)
	{
		app->render->DrawRectangle(leavesCollision, {255, 0, 0, 100});
	}

	if (place == MOSSY_ROCKS_1 && !app->scene->boss2Beat)
	{
		app->render->DrawRectangle(logCollision, { 255, 0, 0, 100 });
	}

	if (place == SHOP)
	{
		for (int i = 0; i < 4; i++) app->render->DrawRectangle(shopProduct[i], {100, 100, 0, 150});
	}

	if (place == ENEMY_FIELD || place == GRASSY_LAND_1)
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* enemy = app->entityManager->enemies[i];
			if (enemy->active)
			{
				app->render->DrawCircle(enemy->dangerRadius.x, enemy->dangerRadius.y, enemy->dangerRadius.radius, { 200, 70, 100, 70 }, false);
				app->render->DrawRectangle(enemy->colliderWorld, { 100, 150, 240, 150 });
				app->render->DrawRectangle(enemy->colliderRect, { 150, 150, 140, 200 });
			}
			enemy = nullptr;
		}
	}

	app->render->DrawRectangle(contactPlayerZone, { 0, 0, 0, 150 });

	if (lilipadPuzzle1 != nullptr) lilipadPuzzle1->DebugDraw();
	if (stonePuzzle1 != nullptr) stonePuzzle1->DebugDraw();
	if (buttonPuzzle1 != nullptr) buttonPuzzle1->DebugDraw();

	if (stabActive)
	{
		int off[4] = { 22, 36, 12, 56 };

		if (stabLeft) app->render->DrawRectangle({ p->colliderWorld.x - STAB_H + off[0], p->colliderWorld.y + off[1], STAB_H, STAB_W }, { 200, 0, 50, 150 });
		else if (stabRight) app->render->DrawRectangle({ p->colliderWorld.x + off[0], p->colliderWorld.y + off[1], STAB_H, STAB_W }, { 200, 0, 50, 150 });
		else if (stabDown) app->render->DrawRectangle({ p->colliderWorld.x + off[2], p->colliderWorld.y + off[3], STAB_W, STAB_H }, { 200, 0, 50, 150 });
		else if (stabUp) app->render->DrawRectangle({ p->colliderWorld.x + off[2], p->colliderWorld.y - STAB_H + off[3], STAB_W, STAB_H }, { 200, 0, 50, 150 });
	}

	if (place == GOLEM_STONES)
	{
		app->render->DrawRectangle(tutorialBossRect, {100, 0, 100, 150});
		app->render->DrawRectangle(secondBossRect, { 100, 0, 100, 150 });
		app->render->DrawRectangle(thirdBossRect, { 100, 0, 100, 150 });
	}

	if (place == GRASSY_LAND_3) app->render->DrawRectangle(velocityStatRect, { 100, 0, 100, 150 });
	else if (place == AUTUM_FALL_2) app->render->DrawRectangle(luckStatRect, { 100, 0, 100, 150 });
	else if (place == MOSSY_ROCKS_2) app->render->DrawRectangle(stabStatRect, { 100, 0, 100, 150 });

	p = nullptr;
}

void World::DrawFilters()
{
	if (gameStart)
	{
		const SDL_Rect a = { 0, 0, 1280, 720 };
		Player* p = app->scene->player1;
		int x = (p->colliderWorld.x + 22 - 640);
		int y = (p->colliderWorld.y + 42 - 360);
		p = nullptr;
		app->render->DrawTexture(night, x, y, 1, false, &a, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
		app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, y + app->render->camera.y }, {0, 0, 0, 255});
	}

	if (place == SHOP)
	{
		const SDL_Rect a = {0, 0, 1280, 720};
		if (!app->scene->boss1Beat) app->render->DrawTexture(questionMark, shopProduct[2].x + 20, shopProduct[2].y, 0.08f, 0.08f, false, &a, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
		if (!app->scene->boss2Beat) app->render->DrawTexture(questionMark, shopProduct[3].x + 20, shopProduct[3].y, 0.08f, 0.08f, false, &a, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
}

void World::DrawStomps()
{
	if (place == GRASSY_LAND_3)
	{
		app->render->DrawTexture(stomp, (38 * 28) + 18, (6 * 28) - 10, 0.35f, 0.3f, false, &stompRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
		if (!velocityTaken) app->render->DrawTexture(statItems, (40 * 28), (5 * 28) - 6, 0.5f, 0.5f, false, &velocityStat, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
	else if (place == AUTUM_FALL_2)
	{
		app->render->DrawTexture(stomp, (24 * 28) + 2, (14 * 28) - 10, 0.35f, 0.3f, false, &stompRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
		if (!luckTaken) app->render->DrawTexture(statItems, (24 * 28) + 40, ((14 * 28) - 10) - 24, 0.5f, 0.5f, false, &luckStat, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
	else if (place == MOSSY_ROCKS_2)
	{
		app->render->DrawTexture(stomp, (71 * 28) - 20, (13 * 28) - 10, 0.35f, 0.3f, false, &stompRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
		if (!luckTaken) app->render->DrawTexture(statItems, (71 * 28) + 18, ((13 * 28) - 10) - 24, 0.5f, 0.5f, false, &stabStat, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, true);
	}
}

void World::DrawInteraction()
{
	const SDL_Rect a = {0, 0, 1280, 720};
	app->render->DrawTexture(interactionText, app->scene->player1->colliderWorld.x + app->scene->player1->colliderWorld.w, app->scene->player1->colliderWorld.y + 5, 0.07f, &a, false);
}

//-------------------------------------------------------------------

void World::WorldMovement()
{
	ChangePlayerState();
	bool move = PlayerMovement();
	CameraMovement(move);
}

void World::WorldChange()
{
	if (place == MAIN_VILLAGE)
	{
		// ENEMY FIELD
		for (int i = 0; i < location3.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location3[i]))
			{
				AsignPrevPosition();
				ChangeMap(ENEMY_FIELD);
				return;
			}
		}

		// GOLEM STONES
		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(GOLEM_STONES);
				return;
			}
		}

		// MOSSY ROCKS
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(MOSSY_ROCKS_1);
				return;
			}
		}

		// HOUSE
		for (int i = 0; i < houses.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, houses[i]))
			{
				AsignPrevPosition();
				ChangeMap(HOUSE);
				return;
			}
		}

		// TAVERN
		for (int i = 0; i < tavern.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, tavern[i]))
			{
				AsignPrevPosition();
				ChangeMap(TAVERN);
				return;
			}
		}

		// SHOP
		for (int i = 0; i < shop.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, shop[i]))
			{
				ChangeMap(SHOP);
				return;
			}
		}
	}
	else if (place == HOUSE || place == SHOP)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(MAIN_VILLAGE);
				return;
			}
		}
	}
	else if (place == TAVERN)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				app->scene->player1->colliderWorld = { 2380, 825, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				app->scene->player1->collisionRect = { 2380, 825 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
				ChangeMap(MAIN_VILLAGE);
				return;
			}
		}
	}
	else if (place == ENEMY_FIELD)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(AUTUM_FALL_1);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(GRASSY_LAND_1);
				return;
			}
		}

		for (int i = 0; i < location3.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location3[i]))
			{
				AsignPrevPosition();
				ChangeMap(MAIN_VILLAGE);
				return;
			}
		}
	}
	else if (place == GRASSY_LAND_1)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(GRASSY_LAND_2);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(ENEMY_FIELD);
				return;
			}
		}
	}
	else if (place == GRASSY_LAND_2)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(GRASSY_LAND_1);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(GRASSY_LAND_3);
				return;
			}
		}
	}
	else if (place == GRASSY_LAND_3)
	{
		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(GRASSY_LAND_2);
				return;
			}
		}
	}
	else if (place == AUTUM_FALL_1)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(ENEMY_FIELD);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(AUTUM_FALL_2);
				return;
			}
		}
	}
	else if (place == AUTUM_FALL_2)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(AUTUM_FALL_1);
				return;
			}
		}
	}
	else if (place == MOSSY_ROCKS_1)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(MAIN_VILLAGE);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(MOSSY_ROCKS_2);
				return;
			}
		}
	}
	else if (place == MOSSY_ROCKS_2)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(MOSSY_ROCKS_1);
				return;
			}
		}
	}
	else if (place == GOLEM_STONES)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(CAVE);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(MAIN_VILLAGE);
				return;
			}
		}
	}
	else if (place == CAVE)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(GOLEM_STONES);
				return;
			}
		}
	}

	return;
}

void World::WorldEnemySpawn()
{
	if (place == ENEMY_FIELD)
	{
		while (app->entityManager->enemies.Count() < ENEMY_FIELD_ENEMY_MAX)
		{
			int lvl = (rand() % 11) + 1;
			app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::SMALL_WOLF);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == GRASSY_LAND_1)
	{
		while (app->entityManager->enemies.Count() < GRASSY_1_ENEMY_MAX)
		{
			int randm = rand() % 5;
			int lvl = (rand() % 11) + 12;
			if (randm == 0) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::SMALL_WOLF);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == GRASSY_LAND_3)
	{
		while (app->entityManager->enemies.Count() < GRASSY_3_ENEMY_MAX)
		{
			int randm = rand() % 5;
			int lvl = (rand() % 11) + 23;
			if (randm == 0) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::SMALL_WOLF);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == AUTUM_FALL_1)
	{
		while (app->entityManager->enemies.Count() < AUTUMN_1_ENEMY_MAX)
		{
			int randm = rand() % 9;
			int lvl = (rand() % 11) + 41;
			if (randm == 0) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::SMALL_WOLF);
			else if (randm < 8) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::MANTIS);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == AUTUM_FALL_2)
	{
		while (app->entityManager->enemies.Count() < AUTUMN_2_ENEMY_MAX)
		{
			int randm = rand() % 6;
			int lvl = (rand() % 11) + 52;
			if (randm < 5) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::MANTIS);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == MOSSY_ROCKS_1)
	{
		while (app->entityManager->enemies.Count() < MOSSY_1_ENEMY_MAX)
		{
			int randm = rand() % 7;
			int lvl = (rand() % 11) + 67;
			if (randm == 0) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::MANTIS);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}
	else if (place == MOSSY_ROCKS_2)
	{
		while (app->entityManager->enemies.Count() < MOSSY_2_ENEMY_MAX)
		{
			int randm = rand() % 5;
			int lvl = (rand() % 11) + 78;
			if (randm == 0) app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::BIRD);
			else app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::MANTIS);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1, lvl);
		}
	}

	//ENEMY LEVELING SYSTEM
	//ENEMY FIELD -> 1 to 11
	//GRASSY LANDS 1 -> 12 to 22
	//GRASSY LANDS 2 -> PUZZLE
	//GRASSY LANDS 3 -> 23 to 33
	//BOSS 2 -> 35
	//AUTUM FALL 1 -> 41 to 51
	//AUTUM FALL 2 -> 52 to 62
	//BOSS 3 -> 65
	//MOSSY ROCKS 1 -> 67 to 77
	//MOSSY ROCKS 2 -> 78 to 88
	//FINAL BOSS -> 90
}

void World::WorldEnemyDetection()
{
	Player* p = app->scene->player1;

	for (int i = 0; i < app->entityManager->enemies.Count(); i++)
	{
		if (app->entityManager->enemies[i]->active && collisionUtils.CheckCollision(p->collisionRect, app->entityManager->enemies[i]->colliderRect))
		{
			AsignPrevPosition();
			app->scene->SetScene(Scenes::COMBAT, app->entityManager->enemies[i]);
			if (inventoryOpen)
			{
				inventoryOpen = false;
				inventory->Restart();
			}
			return;
		}

		if (stabActive)
		{
			int off[4] = { 22, 36, 12, 56 };

			if (stabLeft) stabRect = { p->colliderWorld.x - STAB_H + off[0], p->colliderWorld.y + off[1], STAB_H, STAB_W };
			else if (stabRight) stabRect = { p->colliderWorld.x + off[0], p->colliderWorld.y + off[1], STAB_H, STAB_W };
			else if (stabDown) stabRect = { p->colliderWorld.x + off[2], p->colliderWorld.y + off[3], STAB_W, STAB_H };
			else if (stabUp) stabRect = { p->colliderWorld.x + off[2], p->colliderWorld.y - STAB_H + off[3], STAB_W, STAB_H };
		}

		if (app->entityManager->enemies[i]->active && collisionUtils.CheckCollision(stabRect, app->entityManager->enemies[i]->colliderWorld))
		{
			AsignPrevPosition();
			app->scene->SetScene(Scenes::COMBAT, app->entityManager->enemies[i]);

			stabActive = false;
			stabTime = 0;

			stabLeft = false;
			stabRight = false;
			stabDown = false;
			stabUp = false;

			if (inventoryOpen)
			{
				inventoryOpen = false;
				inventory->Restart();
			}
			return;
		}
	}

	p = nullptr;
}

void World::WorldEnemyChasing()
{
	Player* p = app->scene->player1;
	SDL_Rect conflictZone = {};

	switch (place)
	{
	case ENEMY_FIELD:
		conflictZone = { 196, 168, 1120, 1512 };
		break;

	case GRASSY_LAND_1:
		conflictZone = { 640, 392, 876, 484 };
		break;

	case GRASSY_LAND_3:
		conflictZone = { 784, 784, 644, 672 };
		break;

	case AUTUM_FALL_1:
		conflictZone = { (41 * 28), (85 * 28), (80 * 28), (15 * 28) };
		break;

	case AUTUM_FALL_2:
		conflictZone = { (66 * 28), (17 * 28), (30 * 28), (44 * 28) };
		break;

	case MOSSY_ROCKS_1:
		conflictZone = { (5 * 28), (44 * 28), (51 * 28), (12 * 28) };
		break;

	case MOSSY_ROCKS_2:
		conflictZone = { (33 * 28), (21 * 28), (13 * 28), (16 * 28) };
		break;
	}

	if (collisionUtils.CheckCollision(p->collisionRect, conflictZone) && !playerInmunity)
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* e = app->entityManager->enemies[i];

			if (collisionUtils.CheckCollision(e->dangerRadius, p->collisionRect)) EnemyRunChasing(e, p);
			else EnemyWalkReturn(e, p);

			e = nullptr;
		}
	}
	else
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* e = app->entityManager->enemies[i];

			EnemyWalkReturn(e, p);

			e = nullptr;
		}
	}
	p = nullptr;
}

void World::WorldStabLogic()
{
	Player* p = app->scene->player1;

	if (app->input->GetControl(X) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		stabActive = true;

		if (currentPlayerAnimation == &p->walkDownAnim)
		{
			stabDown = true;
			stabRect = { p->colliderWorld.x, p->colliderWorld.y, STAB_W, STAB_H };
		}
		else if (currentPlayerAnimation == &p->walkUpAnim)
		{
			stabUp = true;
			stabRect = { p->colliderWorld.x, p->colliderWorld.y - STAB_H, STAB_W, STAB_H };
		}
		else if (currentPlayerAnimation == &p->walkLeftAnim)
		{
			stabLeft = true;
			stabRect = { p->colliderWorld.x - STAB_H, p->colliderWorld.y, STAB_H, STAB_W };
		}
		else if (currentPlayerAnimation == &p->walkRightAnim)
		{
			stabRight = true;
			stabRect = { p->colliderWorld.x, p->colliderWorld.y, STAB_H, STAB_W };
		}
	}

	if (stabActive)
	{
		if (stabTime < STAB_TIME) stabTime++;
		else
		{
			stabTime = 0;
			stabActive = false;

			stabLeft = false;
			stabRight = false;
			stabUp = false;
			stabDown = false;
		}
	}
}

void World::EnemyStatsGeneration(Enemy* e, Player* p, int lvl)
{
	float x = lvl;

	int eHealth = 0;
	int eStrength = 0;
	int eDefense = 0;
	int eVelocity = 0;

	//LVL EXP OPTION 1
	/*int variation = (200 * (x / 2));
	int eExp = (x * 150) + (rand() % variation);*/

	//LVL EXP OPTION 2
	int variation = (100 * (x / 2));
	int maxExp = int(floor(1000 * pow((float)app->scene->player1->lvl, 1.25f)));
	maxExp = (maxExp / 5) - 100;
	int eExp = maxExp + (rand() % variation);

	SDL_Rect combatCollider = { NULL };
	SDL_Rect worldCollider = { NULL };

	switch(e->GetClass())
	{
	case EnemyClass::SMALL_WOLF:
		eHealth = floor((x / 2.0f) + 18);
		eStrength = floor((x / 3.0f) + 5);
		eDefense = floor((x / 5.0f) + 1);
		eVelocity = floor((x / 3.0f) + 5);
		combatCollider = { SMALLWOLF_C_X, SMALLWOLF_C_Y, SMALLWOLF_C_W, SMALLWOLF_C_H };
		worldCollider = {0, 0, SMALLWOLF_W_W, SMALLWOLF_W_H};
		break;

	case EnemyClass::BIRD:
		eHealth = floor((x / 4.0f) + 17);
		eStrength = floor((x / 2.0f) + 5);
		eDefense = floor((x / 3.0f) + 2);
		eVelocity = floor((x / 3.0f) + 5);
		combatCollider = { BIRD_C_X, BIRD_C_Y, BIRD_C_W, BIRD_C_H };
		worldCollider = { 0, 0, BIRD_W_W, BIRD_W_H };
		break;

	case EnemyClass::MANTIS:
		eHealth = floor((x / 3.0f) + 19);
		eStrength = floor((x / 3.5f) + 5);
		eDefense = floor((x / 2.5f) + 2);
		eVelocity = floor((x / 3.0f) + 5);
		combatCollider = { MANTIS_C_X, MANTIS_C_Y, MANTIS_C_W, MANTIS_C_H };
		worldCollider = { 0, 0, MANTIS_W_W , MANTIS_W_H };
		break;
	}

	switch (place)
	{
	case ENEMY_FIELD:
		worldCollider.x = 196 + (rand() % 1121);
		worldCollider.y = 168 + (rand() % 1513);
		break;

	case GRASSY_LAND_1:
		worldCollider.x = 644 + (rand() % 896);
		worldCollider.y = 392 + (rand() % 504);
		break;

	case GRASSY_LAND_3:
		worldCollider.x = 784 + (rand() % 644);
		worldCollider.y = 784 + (rand() % 672);
		break;

	case AUTUM_FALL_1:
		worldCollider.x = (41 * 28) + (rand() % (80 * 28));
		worldCollider.y = (85 * 28) + (rand() % (15 * 28));
		break;

	case AUTUM_FALL_2:
		worldCollider.x = (66 * 28) + (rand() % (30 * 28));
		worldCollider.y = (17 * 28) + (rand() % (44 * 28));
		break;

	case MOSSY_ROCKS_1:
		worldCollider.x = (5 * 28) + (rand() % (51 * 28));
		worldCollider.y = (44 * 28) + (rand() % (12 * 28));
		break;

	case MOSSY_ROCKS_2:
		worldCollider.x = (33 * 28) + (rand() % (13 * 28));
		worldCollider.y = (21 * 28) + (rand() % (16 * 28));
		break;
	}

	e->SetUp(combatCollider, worldCollider, lvl, eExp, eHealth, eStrength, eDefense, eVelocity);
}

void World::EnemyRunChasing(Enemy* e, Player* p)
{
	int xOffset = p->collisionRect.x - e->colliderRect.x;
	int yOffset = p->collisionRect.y - e->colliderRect.y;

	int velX = xOffset / ENEMY_RUN_SPEED;
	int velY = yOffset / ENEMY_RUN_SPEED;

	if (velX > ENEMY_RUN_SPEED)
	{
		velX = ENEMY_RUN_SPEED;
		wolfReverse = true;
	}
	if (velY > ENEMY_RUN_SPEED) velY = ENEMY_RUN_SPEED;

	if (velX < -ENEMY_RUN_SPEED)
	{
		velX = -ENEMY_RUN_SPEED;
		wolfReverse = false;
	}
	if (velY < -ENEMY_RUN_SPEED) velY = -ENEMY_RUN_SPEED;

	e->colliderRect.x += velX;
	e->colliderRect.y += velY;

	e->colliderWorld.x += velX;
	e->colliderWorld.y += velY;

	e->dangerRadius.x += velX;
	e->dangerRadius.y += velY;
}

void World::EnemyWalkReturn(Enemy* e, Player* p)
{
	int xOffset = e->originalPosition.x - e->colliderRect.x;
	int yOffset = e->originalPosition.y - e->colliderRect.y;

	int velX = xOffset / ENEMY_WALK_SPEED;
	int velY = yOffset / ENEMY_RUN_SPEED;

	if (velX > ENEMY_WALK_SPEED)
	{
		velX = ENEMY_WALK_SPEED;
		wolfReverse = true;
	}
	if (velY > ENEMY_WALK_SPEED) velY = ENEMY_WALK_SPEED;

	if (velX < -ENEMY_WALK_SPEED)
	{
		velX = -ENEMY_WALK_SPEED;
		wolfReverse = false;
	}
	if (velY < -ENEMY_WALK_SPEED) velY = -ENEMY_WALK_SPEED;

	e->colliderRect.x += velX;
	e->colliderRect.y += velY;

	e->colliderWorld.x += velX;
	e->colliderWorld.y += velY;

	e->dangerRadius.x += velX;
	e->dangerRadius.y += velY;
}

bool World::CollisionSolver(iPoint prevPos)
{
	Player* p = app->scene->player1;
	for (int i = 0; i < collisions.Count(); i++)
	{
		if (collisionUtils.CheckCollision(p->collisionRect, collisions[i]))
		{
			//Millorar perquè no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

			if (prevPos.y - p->collisionRect.y < 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
			}
			if (prevPos.x - p->collisionRect.x < 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
			}
			if (prevPos.y - p->collisionRect.y > 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
			}
			if (prevPos.x - p->collisionRect.x > 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
			}

			p = nullptr;
			return false; //Player Can Not Move
		}
	}

	if (place == GRASSY_LAND_2)
	{
		for (int i = 0; i < lilipadPuzzle1->riverTracks.Count(); i++)
		{
			if (lilipadPuzzle1->riverTracks[i].active && collisionUtils.CheckCollision(p->collisionRect, lilipadPuzzle1->riverTracks[i].rect))
			{
				//Millorar perquè no es quedi parat sense tocar la valla
				//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

				if (prevPos.y - p->collisionRect.y < 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
				}
				if (prevPos.x - p->collisionRect.x < 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
				}
				if (prevPos.y - p->collisionRect.y > 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
				}
				if (prevPos.x - p->collisionRect.x > 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
				}

				p = nullptr;
				return false; //Player Can Not Move
			}
		}

		for (int i = 0; i < stonePuzzle1->stone.Count(); i++)
		{
			if (!stonePuzzle1->stoneInWater && collisionUtils.CheckCollision(p->collisionRect, stonePuzzle1->stone[i].rect))
			{
				//Millorar perquè no es quedi parat sense tocar la valla
				//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

				if (prevPos.y - p->collisionRect.y < 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
				}
				if (prevPos.x - p->collisionRect.x < 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
				}
				if (prevPos.y - p->collisionRect.y > 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
				}
				if (prevPos.x - p->collisionRect.x > 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
				}

				p = nullptr;
				return false; //Player Can Not Move
			}
		}

		if (!stonePuzzle1->stoneInWater && collisionUtils.CheckCollision(p->collisionRect, stonePuzzle1->path))
		{
			//Millorar perquè no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

			if (prevPos.y - p->collisionRect.y < 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
			}
			if (prevPos.x - p->collisionRect.x < 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
			}
			if (prevPos.y - p->collisionRect.y > 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
			}
			if (prevPos.x - p->collisionRect.x > 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
			}

			p = nullptr;
			return false; //Player Can Not Move
		}

		if (!buttonPuzzle1->resultActive)
		{
			if (collisionUtils.CheckCollision(p->collisionRect, buttonPuzzle1->bridge1) || collisionUtils.CheckCollision(p->collisionRect, buttonPuzzle1->bridge2) || collisionUtils.CheckCollision(p->collisionRect, buttonPuzzle1->bridge3))
			{
				if (prevPos.y - p->collisionRect.y < 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
				}
				if (prevPos.x - p->collisionRect.x < 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
				}
				if (prevPos.y - p->collisionRect.y > 0)
				{
					p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
				}
				if (prevPos.x - p->collisionRect.x > 0)
				{
					p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
				}

				p = nullptr;
				return false; //Player Can Not Move
			}
		}
	}

	if (place == ENEMY_FIELD)
	{
		if (!app->scene->boss1Beat && collisionUtils.CheckCollision(p->collisionRect, leavesCollision))
		{
			//Millorar perquè no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

			if (prevPos.y - p->collisionRect.y < 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
			}
			if (prevPos.x - p->collisionRect.x < 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
			}
			if (prevPos.y - p->collisionRect.y > 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
			}
			if (prevPos.x - p->collisionRect.x > 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
			}

			p = nullptr;
			return false; //Player Can Not Move
		}
	}

	if (place == MOSSY_ROCKS_1)
	{
		if (!app->scene->boss2Beat && collisionUtils.CheckCollision(p->collisionRect, logCollision))
		{
			//Millorar perquè no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

			if (prevPos.y - p->collisionRect.y < 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING UP TO DOWN
			}
			if (prevPos.x - p->collisionRect.x < 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING LEFT TO RIGHT
			}
			if (prevPos.y - p->collisionRect.y > 0)
			{
				p->collisionRect.y = prevPos.y; //COLLIDING DOWN TO UP
			}
			if (prevPos.x - p->collisionRect.x > 0)
			{
				p->collisionRect.x = prevPos.x; //COLLIDING RIGHT TO LEFT
			}

			p = nullptr;
			return false; //Player Can Not Move
		}
	}

	p = nullptr;
	return true; //Player Can Move
}

void World::ChangePlayerState()
{
	playerState = PlayerState::IDLE;
	bool ford = false;
	bool back = false;
	bool left = false;
	bool right = false;
	short unsigned int directionsActive = 0;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetControl(UP_PAD) == KEY_DOWN || app->input->GetControl(UP_PAD) == KEY_REPEAT) ford = true;
	
	if (!gameStart)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetControl(DOWN_PAD) == KEY_DOWN || app->input->GetControl(DOWN_PAD) == KEY_REPEAT) back = true;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetControl(LEFT_PAD) == KEY_DOWN || app->input->GetControl(LEFT_PAD) == KEY_REPEAT) left = true;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetControl(RIGHT_PAD) == KEY_DOWN || app->input->GetControl(RIGHT_PAD) == KEY_REPEAT) right = true;
	}

	if (ford && !back && !left && !right) playerState = PlayerState::UP;
	if (!ford && back && !left && !right) playerState = PlayerState::DOWN;
	if (!ford && !back && left && !right) playerState = PlayerState::LEFT;
	if (!ford && !back && !left && right) playerState = PlayerState::RIGHT;
	if (ford && !back && left && !right) playerState = PlayerState::UPLEFT;
	if (ford && !back && !left && right) playerState = PlayerState::UPRIGHT;
	if (!ford && back && left && !right) playerState = PlayerState::DOWNLEFT;
	if (!ford && back && !left && right) playerState = PlayerState::DOWNRIGHT;

}

bool World::PlayerMovement()
{
	Player* p = app->scene->player1;
	iPoint previousPosition = { p->collisionRect.x, p->collisionRect.y };

	switch (playerState)
	{
	case(PlayerState::IDLE):
	
		p->walkDownAnim.loop = false;
		p->walkDownAnim.Reset();
		p->walkUpAnim.loop = false;
		p->walkUpAnim.Reset();
		p->walkLeftAnim.loop = false;
		p->walkLeftAnim.Reset();
		p->walkRightAnim.loop = false;
		p->walkRightAnim.Reset();
		break;

	case(PlayerState::UP):
		currentPlayerAnimation = &p->walkUpAnim;
		p->walkUpAnim.loop = true;
		if (!gameStart) p->collisionRect.y -= worldSpeed;
		else p->collisionRect.y -= 1;
		break;

	case(PlayerState::DOWN):
		currentPlayerAnimation = &p->walkDownAnim;
		p->walkDownAnim.loop = true;
		p->collisionRect.y += worldSpeed;
		break;

	case(PlayerState::LEFT):
		currentPlayerAnimation = &p->walkLeftAnim;
		p->walkLeftAnim.loop = true;
		p->collisionRect.x -= worldSpeed;
		break;

	case(PlayerState::RIGHT):
		currentPlayerAnimation = &p->walkRightAnim;
		p->walkRightAnim.loop = true;
		p->collisionRect.x += worldSpeed;
		break;

	case(PlayerState::UPLEFT):
		currentPlayerAnimation = &p->walkUpAnim;
		p->walkUpAnim.loop = true;
		p->collisionRect.y -= worldSpeed;
		p->collisionRect.x -= worldSpeed;
		break;

	case(PlayerState::UPRIGHT):
		currentPlayerAnimation = &p->walkUpAnim;
		p->walkDownAnim.loop = true;
		p->collisionRect.y -= worldSpeed;
		p->collisionRect.x += worldSpeed;
		break;

	case(PlayerState::DOWNLEFT):
		currentPlayerAnimation = &p->walkDownAnim;
		p->walkLeftAnim.loop = true;
		p->collisionRect.y += worldSpeed;
		p->collisionRect.x -= worldSpeed;
		break;

	case(PlayerState::DOWNRIGHT):
		currentPlayerAnimation = &p->walkDownAnim;
		p->walkRightAnim.loop = true;
		p->collisionRect.y += worldSpeed;
		p->collisionRect.x += worldSpeed;
		break;
	}
	
	bool move = true;

	if (!godMode) move = CollisionSolver(previousPosition);

	p->colliderWorld.x = p->collisionRect.x;
	p->colliderWorld.y = p->collisionRect.y - 56;

	p = nullptr;

	return move;
}

void World::CameraMovement(bool move)
{
	if (move)
	{
		switch (playerState)
		{
		case(PlayerState::IDLE):
			break;

		case(PlayerState::UP):
			if (place != HOUSE)
			{
				if (!gameStart) app->render->camera.y += worldSpeed;
				else app->render->camera.y += 1;
			}
			break;

		case(PlayerState::DOWN):
			if (place != HOUSE) app->render->camera.y -= worldSpeed;
			break;

		case(PlayerState::LEFT):
			if (place != TAVERN && place != HOUSE && place != CAVE && place != SHOP) app->render->camera.x += worldSpeed;
			break;

		case(PlayerState::RIGHT):
			if (place != TAVERN && place != HOUSE && place != CAVE && place != SHOP) app->render->camera.x -= worldSpeed;
			break;

		case(PlayerState::UPLEFT):
			if (place != HOUSE)
			{
				app->render->camera.y += worldSpeed;
				if (place != TAVERN && place != CAVE && place != SHOP) app->render->camera.x += worldSpeed;
			}
			break;

		case(PlayerState::UPRIGHT):
			if (place != HOUSE)
			{
				app->render->camera.y += worldSpeed;
				if (place != TAVERN && place != CAVE && place != SHOP) app->render->camera.x -= worldSpeed;
			}
			break;

		case(PlayerState::DOWNLEFT):
			if (place != HOUSE)
			{
				app->render->camera.y -= worldSpeed;
				if (place != TAVERN && place != CAVE && place != SHOP) app->render->camera.x += worldSpeed;
			}
			break;

		case(PlayerState::DOWNRIGHT):
			if (place != HOUSE)
			{
				app->render->camera.y -= worldSpeed;
				if (place != TAVERN && place != CAVE && place != SHOP) app->render->camera.x -= worldSpeed;
			}
			break;
		}

		RectifyCameraPosition(place);
	}

}

void World::AlignCameraPosition()
{
	Player* p = app->scene->player1;
	app->render->camera.x = (p->colliderWorld.x - REL_CAMX_PLAYERX) * -1;
	app->render->camera.y = (p->colliderWorld.y - REL_CAMY_PLAYERY) * -1;
	p = nullptr;
}

void World::NPCLogic()
{
	SDL_Rect ampPlayerCollider = app->scene->player1->collisionRect;
	ampPlayerCollider.x -= 15;
	ampPlayerCollider.y -= 25;
	ampPlayerCollider.w += 30;
	ampPlayerCollider.h += 50;
	contactPlayerZone = ampPlayerCollider;

	for (int i = 0; i < app->entityManager->NPCs.Count(); i++)
	{
		app->entityManager->NPCs[i]->Update(ampPlayerCollider);
	}
}

void World::StoreLogic()
{
	Scene* s = app->scene;
	for (int i = 0; i < 4; i++)
	{
		if (collisionUtils.CheckCollision(s->player1->collisionRect, shopProduct[i]))
		{
			interactionActive = true;
			if (!s->boss1Beat && !s->boss2Beat)
			{
				if (i == 2 || i == 3)
				{
					app->audio->SetFx(Effect::MEH_FX);
					return;
				}
			}
			else if (s->boss1Beat && !s->boss2Beat)
			{
				if (i == 3)
				{
					app->audio->SetFx(Effect::MEH_FX);
					return;
				}
			}

			if (app->input->GetControl(A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				switch (i)
				{
				case 0:
					if (s->player1->moneyCount >= 6)
					{
						s->player1->moneyCount -= 6;
						s->player1->smallMeatCount++;
						//app->audio->SetFx(BUY_ITEM);
					}
					else app->audio->SetFx(Effect::MEH_FX);
					break;
				case 1:
					if (s->player1->moneyCount >= 12)
					{
						s->player1->moneyCount -= 12;
						s->player1->largeMeatCount++;
						//app->audio->SetFx(BUY_ITEM);
					}
					else app->audio->SetFx(Effect::MEH_FX);
					break;
				case 2:
					if (s->player1->moneyCount >= 10)
					{
						s->player1->moneyCount -= 10;
						s->player1->featherCount++;
						//app->audio->SetFx(BUY_ITEM);
					}
					else app->audio->SetFx(Effect::MEH_FX);
					break;
				case 3:
					if (s->player1->moneyCount >= 14)
					{
						s->player1->moneyCount -= 14;
						s->player1->mantisRodCount++;
						//app->audio->SetFx(BUY_ITEM);
					}
					else app->audio->SetFx(Effect::MEH_FX);
					break;
				}
			}
		}
	}
}

void World::PlayerInmunityLogic()
{
	if (playerInmuneTime > 1)
	{
		playerInmuneTime--;
		playerInmunity = true;
	}
	else
	{
		playerInmunity = false;
	}
}

void World::LoadNPCs(Places placex)
{
	if (placex == MAIN_VILLAGE)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1470, 86}, NPCtype::OLD, placex, 1);
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1600, 86 }, NPCtype::OLD, placex, 3);
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 30, 1550 }, NPCtype::KNIGHT_M, placex, 2);
	}
	else if (placex == TAVERN)
	{
		int dialog = 0;
		if (app->scene->bossTBeat && !app->scene->boss1Beat && !app->scene->boss2Beat) dialog = 8;
		else if (app->scene->bossTBeat && app->scene->boss1Beat && !app->scene->boss2Beat) dialog = 9;
		else if (app->scene->bossTBeat && app->scene->boss1Beat && app->scene->boss2Beat) dialog = 10;
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 150, 150 }, NPCtype::BARMAN, placex, dialog);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 24*28 - 10, 29*28 - 20 }, NPCtype::OLD, placex, 11, 3);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 7*28 - 10, 30*28 - 20 }, NPCtype::KNIGHT_M, placex, 12, 2);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 3*28 - 15 , 30*28 - 20}, NPCtype::KNIGHT_F, placex, 13, 3);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 33*28 - 10, 17*28 }, NPCtype::ADVENTURER, placex, 14, 3);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 24*28, 9*28 }, NPCtype::ADVENTURER, placex, 15, 4);

		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 13*28 - 15, 18*28 - 15 }, NPCtype::CITIZEN_1, placex, 16, 4);
	}
	else if (placex == ENEMY_FIELD)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1580, 500 }, NPCtype::OLD, placex, 4);
	}
	else if (placex == GRASSY_LAND_2)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 924, 392 }, NPCtype::NO_NPC, placex, 5);
	}
	else if (placex == CAVE)
	{
		if (!app->scene->boss2Beat)
		{
			app->entityManager->CreateEntity(EntityType::NPC);
			app->entityManager->NPCs.end->data->SetUp({ 22 * 28, -1 * 28}, NPCtype::FINAL_BOSS, placex, 23);
		}
	}
	else if (placex == AUTUM_FALL_1)
	{
		if (!app->scene->combatScene->secondPlayer)
		{
			app->entityManager->CreateEntity(EntityType::NPC);
			app->entityManager->NPCs.end->data->SetUp({ 69 * 28, 20 * 28 }, NPCtype::CITIZEN_1, placex, 24);
		}
		else
		{
			app->entityManager->CreateEntity(EntityType::NPC);
			app->entityManager->NPCs.end->data->SetUp({ 69 * 28, 20 * 28 }, NPCtype::CITIZEN_1, placex, 25);
		}
	}
	else if (placex == SHOP)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 924, 392 }, NPCtype::SHOP_WOMAN, placex, 26);
	}
}

void World::GolemCall()
{
	Player* p = app->scene->player1;
	SDL_Rect ampPlayerCollider = app->scene->player1->collisionRect;
	ampPlayerCollider.x -= 15;
	ampPlayerCollider.y -= 25;
	ampPlayerCollider.w += 30;
	ampPlayerCollider.h += 50;

	if (collisionUtils.CheckCollision(tutorialBossRect, ampPlayerCollider))
	{
		if (tutorialDialogueOnce)
		{
			app->dialogueManager->StartDialogue(18);
			tutorialDialogueOnce = false;
			p->walkUpAnim.loop = false;
			p->walkUpAnim.Reset();
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetControl(A) == KEY_DOWN)
		{
			if (!app->dialogueManager->onDialog && !inventoryOpen)
			{
				tutorialBossActivation = true;
				AsignPrevPosition();
			}
		}
	}
	else if (collisionUtils.CheckCollision(secondBossRect, ampPlayerCollider))
	{
		if (!app->scene->boss2Beat)
		{
			drawRecomendedII = true;
			lvlRecomendedText->bounds = { 460 - app->render->camera.x, 220 + app->render->camera.y, 100, 100 };
			lvlRecomendedText->SetString("RECOMENDED\n  LVL. 35");
			interactionActive = true;
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetControl(A) == KEY_DOWN)
		{
			if (!app->dialogueManager->onDialog && !inventoryOpen)
			{
				app->SaveGameRequest();
				secondBossActivation = true;
				AsignPrevPosition();
			}
		}
	}
	else if (collisionUtils.CheckCollision(thirdBossRect, ampPlayerCollider))
	{
		if (!app->scene->boss3Beat)
		{
			drawRecomendedIII = true;
			lvlRecomendedText->bounds = { 690 - app->render->camera.x, 300 + app->render->camera.y, 30, 40 };
			lvlRecomendedText->SetString("RECOMENDED\n  LVL. 65");
			interactionActive = true;
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetControl(A) == KEY_DOWN)
		{
			if (!app->dialogueManager->onDialog && !inventoryOpen)
			{
				app->SaveGameRequest();
				thirdBossActivation = true;
				AsignPrevPosition();
			}
		}
	}
	else
	{
		drawRecomendedII = false;
		drawRecomendedIII = false;
	}
}

void World::RectifyCameraPosition(Places placex)
{
	if (placex == MAIN_VILLAGE)
	{
		if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1698) app->render->camera.y = 720 - 2100;
		if (app->scene->player1->colliderWorld.x < 612) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 2832) app->render->camera.x = 1280 - 3500;
	}
	else if (placex == TAVERN || placex == SHOP)
	{
		if (app->scene->player1->colliderWorld.y < 300) app->render->camera.y = -28;
		if (app->scene->player1->colliderWorld.y > 639) app->render->camera.y = -375;
	}
	else if (placex == ENEMY_FIELD)
	{
		if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1838) app->render->camera.y = 720 - 2240;
		if (app->scene->player1->colliderWorld.x < 668) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 1068) app->render->camera.x = 1280 - 1680;
	}
	else if (placex == GRASSY_LAND_1)
	{
		if (app->scene->player1->colliderWorld.y < 310) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 2398) app->render->camera.y = 720 - 2800;
	}
	else if (placex == GRASSY_LAND_2)
	{
		if (app->scene->player1->colliderWorld.y < 310) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1110) app->render->camera.y = 720 - 1512;
		if (app->scene->player1->colliderWorld.x < 668) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 675) app->render->camera.x = 1280 - 1288; //1680
	}
	else if (placex == GRASSY_LAND_3)
	{
		if (app->scene->player1->colliderWorld.y < 310) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 2118) app->render->camera.y = 720 - 2520;
	}
	else if (placex == AUTUM_FALL_1)
	{
		if (app->scene->player1->colliderWorld.x < 608) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 3294) app->render->camera.x = 1280 - 3920; //616
	}
	else if (placex == AUTUM_FALL_2)
	{
		if (app->scene->player1->colliderWorld.x < 608) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 2744) app->render->camera.x = 1280 - 3360; //616
	}
	else if (placex == MOSSY_ROCKS_1)
	{
		if (app->scene->player1->colliderWorld.y < 310) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1276) app->render->camera.y = 720 - 1680;
		if (app->scene->player1->colliderWorld.x < 608) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 1006) app->render->camera.x = 1280 - 1680; //616
	}
	else if (placex == MOSSY_ROCKS_2)
	{
		if (app->scene->player1->colliderWorld.x < 608) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 1624) app->render->camera.x = 1280 - 2240; //616
	}
	else if (placex == GOLEM_STONES)
	{
		if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 598) app->render->camera.y = 720 - 1000;
		if (app->scene->player1->colliderWorld.x < 668) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 1064) app->render->camera.x = 1280 - 1680; //616
	}
	else if (placex == CAVE)
	{
		if (app->scene->player1->colliderWorld.y > 522) app->render->camera.y = 720 - 924;

	}

}

void World::UpdateWorldSpeed()
{
	worldSpeed = app->scene->player1->playerSpeed;
}

void World::ChangeMap(Places place)
{
	app->scene->SetScene(WORLD, place);
}

void World::AsignPrevPosition()
{
	prevPosition = { app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y };
}

void World::Teleport(iPoint pos)
{
	Player* p = app->scene->player1;
	p->colliderWorld = { pos.x, pos.y, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
	p->collisionRect = { pos.x, pos.y + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
	AlignCameraPosition();
	RectifyCameraPosition(place);
}

void World::WorldStatGet()
{
	Player* p = app->scene->player1;

	if (place == GRASSY_LAND_3)
	{
		if (!velocityTaken && collisionUtils.CheckCollision(app->scene->player1->collisionRect, velocityStatRect))
		{
			app->scene->player1->velocityStat = 1;
			app->scene->levelUpScene->UpgradeStats(app->scene->player1->lvl);
			velocityTaken = true;
			app->dialogueManager->StartDialogue(19);
			app->scene->player1->playerSpeed = 7;
			UpdateWorldSpeed();
			p->walkUpAnim.loop = false;
			p->walkUpAnim.Reset();
			p->walkDownAnim.loop = false;
			p->walkDownAnim.Reset();
			p->walkLeftAnim.loop = false;
			p->walkLeftAnim.Reset();
			p->walkRightAnim.loop = false;
			p->walkRightAnim.Reset();
		}
	}
	else if (place == AUTUM_FALL_2)
	{
		if (!luckTaken && collisionUtils.CheckCollision(app->scene->player1->collisionRect, luckStatRect))
		{
			app->scene->player1->luckStat = 1;
			app->scene->levelUpScene->UpgradeStats(app->scene->player1->lvl);
			luckTaken = true;
			app->dialogueManager->StartDialogue(20);
			p->walkUpAnim.loop = false;
			p->walkUpAnim.Reset();
			p->walkDownAnim.loop = false;
			p->walkDownAnim.Reset();
			p->walkLeftAnim.loop = false;
			p->walkLeftAnim.Reset();
			p->walkRightAnim.loop = false;
			p->walkRightAnim.Reset();
		}
	}
	else if (place == MOSSY_ROCKS_2)
	{
		if (!stabTaken && collisionUtils.CheckCollision(app->scene->player1->collisionRect, stabStatRect))
		{
			app->scene->player1->stabStat = 1;
			app->scene->levelUpScene->UpgradeStats(app->scene->player1->lvl);
			stabTaken = true;
			app->dialogueManager->StartDialogue(21);
			p->walkUpAnim.loop = false;
			p->walkUpAnim.Reset();
			p->walkDownAnim.loop = false;
			p->walkDownAnim.Reset();
			p->walkLeftAnim.loop = false;
			p->walkLeftAnim.Reset();
			p->walkRightAnim.loop = false;
			p->walkRightAnim.Reset();
		}
	}
}
