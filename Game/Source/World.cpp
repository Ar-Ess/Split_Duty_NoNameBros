#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "Input.h"
#include "DialogueManager.h"

#include "World.h"
#include "Map.h"
#include "Player.h"
#include "NPC.h"

#include "Log.h"

World::World()
{
	map = new Map();
	place = NO_PLACE;
	prevPosition = {0, 0};
	dialogs = new DialogueManager();
}

void World::Start(Places placex)
{
	Player* p = app->scene->player1;
	if (placex == MAIN_VILLAGE)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		place = placex;
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
		place = placex;
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
		place = placex;
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
		place = placex;
		map->Load("graveyard.tmx");

		// IF WE COME FROM CONTINUE BUTTON CLICKED, DONT ENTER HERE
		if (!app->scene->continuePressed)
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
			else
			{
				p->colliderWorld = { prevPosition.x, prevPosition.y, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H };
				p->collisionRect = { prevPosition.x, prevPosition.y + 56, INIT_PLAYER_WORLD_W, INIT_PLAYER_WORLD_H - 56 };

				AlignCameraPosition();

				RectifyCameraPosition(placex);
			}
		}

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf_spritesheet.png");
		birdSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		mantisSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
	}

	walkingSpritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/walking_spritesheet.png");

	LoadNPCs(placex);

	currentPlayerAnimation = &p->walkDownAnim;

	UpdateWorldSpeed();

	p = nullptr;
}

void World::Restart()
{
	map->CleanUp();

	houses.Clear();

	tavern.Clear();

	shop.Clear();

	collisions.Clear();

	location1.Clear();

	location2.Clear();

	location3.Clear();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	if (wolfSpritesheet != nullptr) app->tex->UnLoad(wolfSpritesheet);
	if (birdSpritesheet != nullptr)app->tex->UnLoad(birdSpritesheet);
	if (mantisSpritesheet != nullptr)app->tex->UnLoad(mantisSpritesheet);

	if (walkingSpritesheet != nullptr) app->tex->UnLoad(walkingSpritesheet);
	
	ListItem<NPC*>* item = app->entityManager->NPCs.start;
	while (item != NULL)
	{
		item->data->Delete();
		RELEASE(item->data);
		item = item->next;
	}
	app->entityManager->NPCs.Clear();
}

void World::Update()
{
	if (!app->dialogueManager->onDialog) WorldMovement();

	WorldChange();

	WorldEnemyDetection();

	NPCLogic();
}

void World::Draw()
{
	map->Draw();
	if (debugCollisions) DrawCollisions();
	DrawNPC();
	DrawEnemy();
	DrawPlayer();
}

void World::DrawPlayer()
{
	currentPlayerAnimation->Update(1.0f);
	app->render->DrawTexture(walkingSpritesheet, app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
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
				if (enemy->GetClass() == EnemyClass::SMALL_WOLF)
				{
					app->render->DrawRectangle(enemy->colliderWorld, { 100, 150, 240, 150 });
					app->render->DrawRectangle(enemy->colliderRect, { 150, 150, 140, 200 });
				}
				
			}
			enemy = nullptr;
		}
	}

	app->render->DrawRectangle(contactPlayerZone, { 0, 0, 0, 150 });
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
				app->scene->player1->colliderWorld = { 2380, 825, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 2380, 825 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
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
				ChangeMap(GRASSY_LAND);
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
}

void World::WorldEnemySpawn()
{
	//Player* p = app->scene->player1;
}

void World::WorldEnemyDetection()
{
	if (app->entityManager->enemies.Count() > 0)
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			if (app->entityManager->enemies[i]->active, collisionUtils.CheckCollision(app->scene->player1->collisionRect, app->entityManager->enemies[i]->colliderRect))
			{
				AsignPrevPosition();
				app->scene->SetScene(Scenes::COMBAT, app->entityManager->enemies[i]);
			}
		}
	}
}

bool World::CollisionSolver(iPoint prevPos)
{
	Player* p = app->scene->player1;
	for (int i = 0; i < collisions.Count(); i++)
	{
		if (collisionUtils.CheckCollision(p->collisionRect, collisions[i]))
		{
			//Millorar perqu� no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la ll�gica)

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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ford = true;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) back = true;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) left = true;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) right = true;

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

void World::LoadNPCs(Places placex)
{
	if (placex == MAIN_VILLAGE)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1470, 86}, NPCtype::OLD, placex, 1);
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 30, 1550 }, NPCtype::KNIGHT, placex, 2);
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 200, 1490 }, NPCtype::PRIEST, placex, 3);
	}
	else if (placex == ENEMY_FIELD)
	{
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1580, 500 }, NPCtype::OLD, placex, 4);
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
