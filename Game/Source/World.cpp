#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "EntityManager.h"
#include "Enemy.h"

#include "World.h"
#include "Map.h"
#include "Player.h"

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
		place = placex;
		map->Load("SplitDuty1.tmx");

		//if (collisionUtils.CheckCollision({prevPosition.x, prevPosition.y}))

		app->scene->player1->colliderWorld = { 60, 150, 56, 84 };
		app->scene->player1->collisionRect = { 60, 150 + 56, 56, 84 - 56};

		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}
	else if (placex == HOUSE)
	{
		place = placex;
		map->Load("house.tmx");
		app->scene->player1->colliderWorld = { 280, 350, 56, 84 };
		app->scene->player1->collisionRect = { 280, 350 + 56, 56, 84 - 56 };

		app->render->camera.x = 332;
		app->render->camera.y = 100; //Full = 22 width -> 616 | Half = 11 width -> 308 | Half screen width = 640
	}
	else if (placex == TAVERN)
	{
		place = placex;
		map->Load("tavern.tmx");
		app->scene->player1->colliderWorld = { 60, 150, 56, 84 };
		app->scene->player1->collisionRect = { 60, 150 + 56, 56, 84 - 56 };
	}
	else if (placex == ENEMY_FIELD)
	{
		place = placex;
		map->Load("graveyard.tmx");
		app->scene->player1->colliderWorld = { 60, 150, 56, 84 };
		app->scene->player1->collisionRect = { 60, 150 + 56, 56, 84 - 56 };
		wolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/wolf-spritesheet.png");
	}

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

	//place = NO_PLACE;
}

void World::Update()
{
	WorldMovement();
	WorldChange();
	if (place == ENEMY_FIELD) WorldEnemyDetection();
}

void World::Draw()
{
	map->Draw();
	DrawPlayer();
	DrawEnemy();
	DrawCollisions();
}

void World::DrawPlayer()
{
	app->render->DrawRectangle(app->scene->player1->GetWorldBounds(), {100, 150, 240, 150});
	app->render->DrawRectangle(app->scene->player1->GetCollisionBounds(), { 150, 150, 140, 200 });
}

void World::DrawEnemy()
{
	if (place == ENEMY_FIELD)
	{
		for (int i = 0; i < app->entityManager->enemies.Count(); i++)
		{
			Enemy* enemy = app->entityManager->enemies[i];
			app->render->DrawRectangle(enemy->colliderWorld, { 100, 150, 240, 150 });
			app->render->DrawRectangle(enemy->colliderRect, { 150, 150, 140, 200 });

			if (enemy->GetClass() == EnemyClass::SMALL_WOLF)
			{
				app->render->DrawTexture(wolfSpritesheet, enemy->colliderWorld.x, enemy->colliderWorld.y, SCALE, &wolfRect, false);
			}

			enemy = nullptr;
		}
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
}

//-------------------------------------------------------------------

void World::WorldMovement()
{
	bool move = PlayerMovement();
	if (place != HOUSE) CameraMovement(move);
}

void World::WorldChange()
{
	if (place == MAIN_VILLAGE)
	{
		for (int i = 0; i < location3.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location3[i]))
			{
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
				ChangeMap(HOUSE);
				return;
			}
		}

		for (int i = 0; i < tavern.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, tavern[i]))
			{
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
	else if (place == HOUSE || place == TAVERN || place == SHOP)
	{
		for (int i = 0; i < location1.Count(); i++)
		{
			if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, location1[i]))
			{
				ChangeMap(MAIN_VILLAGE);
				prevPosition = {app->scene->player1->colliderWorld.x, app->scene->player1->colliderWorld.y};
				prevCam = {app->render->camera.x, app->render->camera.y};
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
		if (collisionUtils.CheckCollision(app->scene->player1->collisionRect, app->entityManager->enemies[i]->colliderRect))
		{
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

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->scene->player1->collisionRect.y -= worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->scene->player1->collisionRect.y += worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->scene->player1->collisionRect.x -= worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->scene->player1->collisionRect.x += worldSpeed;

	bool move = CollisionSolver(previousPosition);

	app->scene->player1->colliderWorld.x = app->scene->player1->collisionRect.x;
	app->scene->player1->colliderWorld.y = app->scene->player1->collisionRect.y - 56;

	return move;
}

void World::CameraMovement(bool move)
{
	if (move)
	{
		if (app->scene->player1->colliderWorld.y > 317 && app->scene->player1->colliderWorld.y < 1662) if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->render->camera.y += worldSpeed;
		if (app->scene->player1->colliderWorld.y > 318 && app->scene->player1->colliderWorld.y < 1663) if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->render->camera.y -= worldSpeed;
		if (app->scene->player1->colliderWorld.x > 611 && app->scene->player1->colliderWorld.x < 2832) if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->render->camera.x += worldSpeed;
		if (app->scene->player1->colliderWorld.x > 612 && app->scene->player1->colliderWorld.x < 2833) if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->render->camera.x -= worldSpeed;
	}

	RectifyCameraPosition();
}

void World::RectifyCameraPosition()
{
	if (app->scene->player1->colliderWorld.y < 318) app->render->camera.y = 0;
	if (app->scene->player1->colliderWorld.y > 1670) app->render->camera.y = 720 - 2072;

	if (app->scene->player1->colliderWorld.x < 612) app->render->camera.x = 0;
	if (app->scene->player1->colliderWorld.x > 2832) app->render->camera.x = 1280 - 3500;
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
