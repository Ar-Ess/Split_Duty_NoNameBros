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
}

void World::Start(Places placex)
{
	if (placex == MAIN_VILLAGE)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		Places temp = place;
		place = placex;
		map->Load("main_village.tmx");

		if (!app->scene->continuePressed)
		{
			if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor1House))
			{
				app->scene->player1->colliderWorld = { 420, 636, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 420, 636 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor2House))
			{
				app->scene->player1->colliderWorld = { 952, 636, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 952, 636 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor3House))
			{
				app->scene->player1->colliderWorld = { 1428, 720, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 1428, 720 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor4House))
			{
				app->scene->player1->colliderWorld = { 2688, 1812, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 2688, 1812 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor5House))
			{
				app->scene->player1->colliderWorld = { 2072, 1812, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 2072, 1812 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensor6House))
			{
				app->scene->player1->colliderWorld = { 3108, 1280, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 3108, 1280 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x + 54, prevPosition.y, 1, 1 }, sensorFieldVillage1))
			{
				app->scene->player1->colliderWorld = { 35, 175, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 35, 175 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x + 54, prevPosition.y, 1, 1 }, sensorFieldVillage2))
			{
				app->scene->player1->colliderWorld = { 35, 1380, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 35, 1380 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
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
			app->scene->player1->colliderWorld = { 280, 350, PLAYER_WORLD_WIDTH, 84 };
			app->scene->player1->collisionRect = { 280, 350 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
		}

		app->render->camera.x = 332;
		app->render->camera.y = 100; //Full = 22 width -> 616 | Half = 11 width -> 308 | Half screen width = 640
	}
	else if (placex == TAVERN)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		place = placex;
		map->Load("tavern.tmx");

		if (!app->scene->continuePressed)
		{
			app->scene->player1->colliderWorld = { 504, 880, PLAYER_WORLD_WIDTH, 84 };
			app->scene->player1->collisionRect = { 504, 880 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };
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

		if (!app->scene->continuePressed)
		{
			if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensorVillageField1))
			{
				app->scene->player1->colliderWorld = { 1580, 150, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 1580, 150 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };

				app->render->camera.x = (app->scene->player1->colliderWorld.x - REL_CAMX_PLAYERX) * -1;
				app->render->camera.y = (app->scene->player1->colliderWorld.y - REL_CAMY_PLAYERY) * -1;

				RectifyCameraPosition(placex);

				app->render->camera.y = -140;

				app->scene->player1->colliderWorld.y += 140;
				app->scene->player1->collisionRect.y += 140;
			}
			else if (collisionUtils.CheckCollision({ prevPosition.x, prevPosition.y, 1, 1 }, sensorVillageField2))
			{
				app->scene->player1->colliderWorld = { 1580, 150, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { 1580, 150 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };

				AlignCameraPosition();

				RectifyCameraPosition(placex);

				app->render->camera.y = -1216;

				app->scene->player1->colliderWorld.y += 1356; //1316
				app->scene->player1->collisionRect.y += 1356; //1316
			}
			else
			{
				app->scene->player1->colliderWorld = { prevPosition.x, prevPosition.y, PLAYER_WORLD_WIDTH, 84 };
				app->scene->player1->collisionRect = { prevPosition.x, prevPosition.y + 56, PLAYER_WORLD_WIDTH, 84 - 56 };

				AlignCameraPosition();

				RectifyCameraPosition(placex);
			}
		}

		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf-spritesheet.png");
		
	}

	walkingSpritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/walking-spritesheet.png");

	LoadNPCs(placex);

	currentPlayerAnimation = &app->scene->player1->walkDownAnim;

	UpdateWorldSpeed();
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

	if (place == ENEMY_FIELD) app->tex->UnLoad(wolfSpritesheet);

	app->tex->UnLoad(walkingSpritesheet);

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
	if (place == ENEMY_FIELD) WorldEnemyDetection();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) drawCollisions = !drawCollisions;

	NPCLogic();
}

void World::Draw()
{
	map->Draw();
	if (drawCollisions) DrawCollisions();
	app->render->DrawRectangle(temp, { 0, 0, 0, 150 });
	DrawPlayer();
	//DrawNPC();
	DrawEnemy();
	app->dialogueManager->Update(1.0f);
}

void World::DrawPlayer()
{
	currentPlayerAnimation->Update(1.0f);
	app->render->DrawTexture(walkingSpritesheet, app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y - 56, &currentPlayerAnimation->GetCurrentFrame());
}

void World::DrawEnemy()
{
	if (place == ENEMY_FIELD)
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* enemy = app->entityManager->enemies[i];
			if (enemy->active)
			{
				if (enemy->GetClass() == EnemyClass::SMALL_WOLF)
				{
					app->render->DrawTexture(wolfSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y, SCALE, &wolfRect, false);
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
					/*app->render->DrawRectangle(enemy->colliderWorld, { 100, 150, 240, 150 });
					app->render->DrawRectangle(enemy->colliderRect, { 150, 150, 140, 200 });*/
				}
				app->render->DrawRectangle(enemy->colliderWorld, { 100, 150, 240, 150 });
				app->render->DrawRectangle(enemy->colliderRect, { 150, 150, 140, 200 });
			}
			enemy = nullptr;
		}
	}
}

//-------------------------------------------------------------------

void World::WorldMovement()
{
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

void World::WorldEnemyDetection()
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

bool World::CollisionSolver(iPoint prevPos)
{
	for (int i = 0; i < collisions.Count(); i++)
	{
		if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, collisions[i]))
		{
			//Millorar perquè no es quedi parat sense tocar la valla
			//Com fer-ho? if (prev.y - actual.y < 0) Esta fent collision des d'adalt (continual la llògica)

			app->scene->player1->collisionRect.x = prevPos.x;
			app->scene->player1->collisionRect.y = prevPos.y;
			return false; //Player Can Not Move
		}
	}

	return true; //Player Can Move
}

bool World::PlayerMovement()
{
	iPoint previousPosition = { app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y };

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		currentPlayerAnimation = &app->scene->player1->walkUpAnim;
		app->scene->player1->walkUpAnim.loop = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		app->scene->player1->collisionRect.y -= worldSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		app->scene->player1->ResetWalkingAnimation();
		app->scene->player1->walkUpAnim.loop = false;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		currentPlayerAnimation = &app->scene->player1->walkDownAnim;
		app->scene->player1->walkDownAnim.loop = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		app->scene->player1->collisionRect.y += worldSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
	{
		app->scene->player1->ResetWalkingAnimation();
		app->scene->player1->walkDownAnim.loop = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		app->scene->player1->walkLeftAnim.loop = true;
		currentPlayerAnimation = &app->scene->player1->walkLeftAnim;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		app->scene->player1->collisionRect.x -= worldSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		app->scene->player1->ResetWalkingAnimation();
		app->scene->player1->walkLeftAnim.loop = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		app->scene->player1->walkRightAnim.loop = true;
		currentPlayerAnimation = &app->scene->player1->walkRightAnim;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		app->scene->player1->collisionRect.x += worldSpeed;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		app->scene->player1->ResetWalkingAnimation();
		app->scene->player1->walkRightAnim.loop = false;
	}

	if (!app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && !app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && !app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		app->scene->player1->ResetWalkingAnimation();
		app->scene->player1->walkRightAnim.loop = app->scene->player1->walkLeftAnim.loop = app->scene->player1->walkUpAnim.loop = app->scene->player1->walkDownAnim.loop = false;
	}


	
	bool move = CollisionSolver(previousPosition);

	app->scene->player1->colliderWorld.x = app->scene->player1->collisionRect.x;
	app->scene->player1->colliderWorld.y = app->scene->player1->collisionRect.y - 56;

	return move;
}

void World::CameraMovement(bool move)
{
	if (move)
	{
		if (place == MAIN_VILLAGE || place == ENEMY_FIELD)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->render->camera.y += worldSpeed;
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->render->camera.y -= worldSpeed;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->render->camera.x += worldSpeed;
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->render->camera.x -= worldSpeed;
		}
		else if (place == TAVERN)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->render->camera.y += worldSpeed;
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->render->camera.y -= worldSpeed;
		}

		RectifyCameraPosition(place);
	}

}

void World::AlignCameraPosition()
{
	app->render->camera.x = (app->scene->player1->colliderWorld.x - REL_CAMX_PLAYERX) * -1;
	app->render->camera.y = (app->scene->player1->colliderWorld.y - REL_CAMY_PLAYERY) * -1;
}

void World::NPCLogic()
{
	SDL_Rect ampPlayerCollider = app->scene->player1->collisionRect;
	ampPlayerCollider.x -= 15;
	ampPlayerCollider.y -= 25;
	ampPlayerCollider.w += 30;
	ampPlayerCollider.h += 50;
	temp = ampPlayerCollider;

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
		app->entityManager->NPCs.end->data->SetUp({ 1470, 86}, NPCtype::CITICIZEN, placex, 1);
		app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->NPCs.end->data->SetUp({ 1470, 86 }, NPCtype::CITICIZEN, placex, 1);
		/*app->entityManager->CreateEntity(EntityType::NPC);
		app->entityManager->CreateEntity(EntityType::NPC);*/
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
		if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = -28;
		if (app->scene->player1->colliderWorld.y > 639) app->render->camera.y = -375;
	}
	else if (placex == ENEMY_FIELD)
	{
		if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = 0;
		if (app->scene->player1->colliderWorld.y > 1838) app->render->camera.y = 720 - 2240;
		if (app->scene->player1->colliderWorld.x < 612) app->render->camera.x = 0;
		if (app->scene->player1->colliderWorld.x > 1068) app->render->camera.x = 1280 - 1680;
	}
}

void World::UpdateWorldSpeed()
{
	worldSpeed = app->scene->player1->playerSpeed;
}

void World::ChangeMap(Places place)
{
	if (place == HOUSE || place == TAVERN || place == SHOP || place == MAIN_VILLAGE || place == ENEMY_FIELD) app->scene->SetScene(WORLD, place);
	else
	{
		app->scene->SetScene(Scenes::MAIN_MENU);
	}

	//QUAN ELS TINGUEM TOTS NO TINDRÀ SENTIT L'IF, SIMPLEMENT app->scene->SetScene(WORLD, place); i ja
}

void World::AsignPrevPosition()
{
	prevPosition = { app->scene->player1->collisionRect.x, app->scene->player1->collisionRect.y };
}
