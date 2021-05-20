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
	else if (placex == ENEMY_FIELD)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("graveyard.tmx");

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

				AlignCameraPosition();

				RectifyCameraPosition(placex);
			}
		}

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
	}
	else if (placex == GRASSY_LAND_2)
	{
		app->audio->SetMusic(SoundTrack::GRASSYLANDS_TRACK);

		if (lilipadPuzzle1 == nullptr)
		{
			lilipadPuzzle1 = new LilipadPuzzle(2, {840 + 14, 1092 + 14, 28, 28}, {980 + 14, 1092 + 14, 28, 28});
			lilipadPuzzle1->lilipad.At(0)->data.SetPosition(644, 924);
			lilipadPuzzle1->lilipad.At(1)->data.SetPosition(252, 616);
		}

		if (stonePuzzle1 == nullptr)
		{
			stonePuzzle1 = new StonePuzzle(1, {280, 896, 112, 84});
			stonePuzzle1->stone.At(0)->data.SetPosition(504, 728);
		}

		if (buttonPuzzle1 == nullptr)
		{
			buttonPuzzle1 = new ButtonPuzzle(4, 28, 28, {392, 308, 56, 56}, { 616, 308, 56, 56 }, { 840, 308, 56, 56 });
			buttonPuzzle1->button.At(0)->data.SetPosition(196 + 14, 1064 + 14, 1);
			buttonPuzzle1->button.At(1)->data.SetPosition(336 + 14, 1148 + 14, 2);
			buttonPuzzle1->button.At(2)->data.SetPosition(392 + 14, 1372 + 14, 3);
			buttonPuzzle1->button.At(3)->data.SetPosition(224 + 14, 1288 + 14, 4);
		}

		map->Load("grassy_lands_2.tmx");

		if (!app->scene->continuePressed)
		{
			p->colliderWorld = { 616, 1390, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
			p->collisionRect = { 616, 1390 + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };
		}

		AlignCameraPosition();

		RectifyCameraPosition(placex);
	}

	walkingSpritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/walking_spritesheet.png");

	LoadNPCs(placex);

	currentPlayerAnimation = &p->walkDownAnim;

	UpdateWorldSpeed();

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

	if (wolfSpritesheet != nullptr) app->tex->UnLoad(wolfSpritesheet);
	if (birdSpritesheet != nullptr)app->tex->UnLoad(birdSpritesheet);
	if (mantisSpritesheet != nullptr)app->tex->UnLoad(mantisSpritesheet);

	if (walkingSpritesheet != nullptr) app->tex->UnLoad(walkingSpritesheet);
	
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

	WorldChange();

	if (playerInmuneTime > 0)
	{
		PlayerInmunityLogic();
	}

	EnemyLogic();

	NPCLogic();

	if (place == GRASSY_LAND_2)
	{
		lilipadPuzzle1->Update();
		stonePuzzle1->Update();
		buttonPuzzle1->Update();
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->GetControl(Y) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (!app->dialogueManager->onDialog)
		{
			inventoryOpen = !inventoryOpen;
			if (inventoryOpen) inventory->Start();
			else inventory->Restart();
			app->audio->SetFx(Effect::INVENTORY_OPEN_FX);
		}
	}
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

	if (debugCollisions)
	{
		DrawCollisions();
	}
	if (place != MAIN_VILLAGE && place != GRASSY_LAND_2)
	{
		DrawNPC();
		DrawEnemy();
		DrawPlayer();
	}
	return;
}

void World::DrawPlayer()
{
	currentPlayerAnimation->Update(1.0f);
	if (!playerInmunity) app->render->DrawTexture(walkingSpritesheet, app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
	else
	{
		if (app->GetFrameCount() % 2 == 0)
		{
			app->render->DrawTexture(walkingSpritesheet, app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
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
				if (enemy->GetClass() == EnemyClass::SMALL_WOLF)
				{
					app->render->DrawTexture(wolfSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y-20, SCALE, &wolfRect, false);
				}
				if (enemy->GetClass() == EnemyClass::BIRD)
				{
					app->render->DrawTexture(birdSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y-20, SCALE*1.5f, &birdRect, false);

				}
				if (enemy->GetClass() == EnemyClass::MANTIS)
				{
					app->render->DrawTexture(mantisSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y-26-20, SCALE*1.5f, &mantisRect, false);
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

void World::DrawCollisions()
{
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

	if (place == ENEMY_FIELD)
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
		for (int i = 0; i < location3.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location3[i]))
			{
				AsignPrevPosition();
				ChangeMap(ENEMY_FIELD);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(MOSSY_ROCKS);
				return;
			}
		}

		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(GOLEM_STONES);
				return;
			}
		}

		for (int i = 0; i < houses.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, houses[i]))
			{
				AsignPrevPosition();
				ChangeMap(HOUSE);
				return;
			}
		}

		for (int i = 0; i < tavern.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, tavern[i]))
			{
				AsignPrevPosition();
				ChangeMap(TAVERN);
				return;
			}
		}

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
				ChangeMap(AUTUM_FALL);
				return;
			}
		}

		for (int i = 0; i < location2.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location2[i]))
			{
				ChangeMap(GRASSY_LAND_2);
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
	else if (place == GRASSY_LAND_2)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(ENEMY_FIELD);
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
			app->entityManager->CreateEntity(EntityType::ENEMY, EnemyClass::SMALL_WOLF);
			EnemyStatsGeneration(app->entityManager->enemies.end->data, app->scene->player1);
		}
	}
}

void World::WorldEnemyDetection()
{
	for (int i = 0; i < app->entityManager->enemies.Count(); i++)
	{
		if (app->entityManager->enemies[i]->active && collisionUtils.CheckCollision(app->scene->player1->collisionRect, app->entityManager->enemies[i]->colliderRect))
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
	}
}

void World::WorldEnemyChasing()
{
	Player* p = app->scene->player1;
	if (collisionUtils.CheckCollision(p->collisionRect, { 196, 168, 1120, 1512 }) && !playerInmunity)
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

void World::EnemyStatsGeneration(Enemy* e, Player* p)
{
	int eHealth = p->maxHealth;
	int eStrength = p->strengthStat;
	int eDefense = p->defenseStat - 2;
	int eVelocity = 10;
	int eLevel = p->lvl + 1;
	int eExp = 2100 + (rand() % 200);

	SDL_Rect combatCollider = { NULL };
	SDL_Rect worldCollider = { NULL };

	switch(e->GetClass())
	{
	case EnemyClass::SMALL_WOLF:
		combatCollider = { SMALLWOLF_C_X, SMALLWOLF_C_Y, SMALLWOLF_C_W, SMALLWOLF_C_H };
		worldCollider = {0, 0, SMALLWOLF_W_W, SMALLWOLF_W_H};
		break;

	case EnemyClass::BIRD:
		combatCollider = { BIRD_C_X, BIRD_C_Y, BIRD_C_W, BIRD_C_H };
		worldCollider = { 0, 0, BIRD_W_W, BIRD_W_H };
		break;

	case EnemyClass::MANTIS:
		combatCollider = { MANTIS_C_X, MANTIS_C_Y, MANTIS_C_W, MANTIS_C_H };
		worldCollider = { 0, 0, MANTIS_W_W , MANTIS_W_H };
		break;
	}

	worldCollider.x = 196 + (rand() % 1121);
	worldCollider.y = 168 + (rand() % 1513);

	e->SetUp(combatCollider, worldCollider, eLevel, eExp, eHealth, eStrength, eDefense, eVelocity);
}

void World::EnemyRunChasing(Enemy* e, Player* p)
{
	int xOffset = p->collisionRect.x - e->colliderRect.x;
	int yOffset = p->collisionRect.y - e->colliderRect.y;

	int velX = xOffset / ENEMY_RUN_SPEED;
	int velY = yOffset / ENEMY_RUN_SPEED;

	if (velX > ENEMY_RUN_SPEED) velX = ENEMY_RUN_SPEED;
	if (velY > ENEMY_RUN_SPEED) velY = ENEMY_RUN_SPEED;

	if (velX < -ENEMY_RUN_SPEED) velX = -ENEMY_RUN_SPEED;
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

	if (velX > ENEMY_WALK_SPEED) velX = ENEMY_WALK_SPEED;
	if (velY > ENEMY_WALK_SPEED) velY = ENEMY_WALK_SPEED;

	if (velX < -ENEMY_WALK_SPEED) velX = -ENEMY_WALK_SPEED;
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
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetControl(DOWN_PAD) == KEY_DOWN || app->input->GetControl(DOWN_PAD) == KEY_REPEAT) back = true;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetControl(LEFT_PAD) == KEY_DOWN || app->input->GetControl(LEFT_PAD) == KEY_REPEAT) left = true;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetControl(RIGHT_PAD) == KEY_DOWN || app->input->GetControl(RIGHT_PAD) == KEY_REPEAT) right = true;

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
		p->collisionRect.y -= worldSpeed;
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
			if (place != HOUSE) app->render->camera.y += worldSpeed;
			break;

		case(PlayerState::DOWN):
			if (place != HOUSE) app->render->camera.y -= worldSpeed;
			break;

		case(PlayerState::LEFT):
			if (place != TAVERN && place != HOUSE) app->render->camera.x += worldSpeed;
			break;

		case(PlayerState::RIGHT):
			if (place != TAVERN && place != HOUSE) app->render->camera.x -= worldSpeed;
			break;

		case(PlayerState::UPLEFT):
			if (place != HOUSE)
			{
				app->render->camera.y += worldSpeed;
				if (place != TAVERN) app->render->camera.x += worldSpeed;
			}
			break;

		case(PlayerState::UPRIGHT):
			if (place != HOUSE)
			{
				app->render->camera.y += worldSpeed;
				if (place != TAVERN) app->render->camera.x -= worldSpeed;
			}
			break;

		case(PlayerState::DOWNLEFT):
			if (place != HOUSE)
			{
				app->render->camera.y -= worldSpeed;
				if (place != TAVERN) app->render->camera.x += worldSpeed;
			}
			break;

		case(PlayerState::DOWNRIGHT):
			if (place != HOUSE)
			{
				app->render->camera.y -= worldSpeed;
				if (place != TAVERN) app->render->camera.x -= worldSpeed;
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
		app->entityManager->NPCs.end->data->SetUp({ 30, 1550 }, NPCtype::KNIGHT, placex, 2);
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
	else if (placex == TAVERN)
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
	else if (placex == GRASSY_LAND_2)
	{
		if (app->scene->player1->colliderWorld.y < 310) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1110) app->render->camera.y = 720 - 1512;
		if (app->scene->player1->colliderWorld.x < 668) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 675) app->render->camera.x = 1280 - 1288; //1680
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
