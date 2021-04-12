#include "App.h"
#include "Audio.h"
#include "Render.h"

#include "World.h"
#include "Map.h"
#include "Player.h"

#include "Log.h"

World::World()
{
	map = new Map();
	place = NO_PLACE;
}

void World::Start(Places placex)
{
	if (placex == MAIN_VILLAGE)
	{
		app->audio->SetMusic(SoundTrack::MAINVILLAGE_TRACK);
		map->Load("SplitDuty1.tmx");
		place = placex;
		app->scene->player1->colliderWorld = { 60, 150, 56, 84 };
		app->scene->player1->collisionRect = { 60, 206, 56, 28 };
	}

	UpdateWorldSpeed();
}

void World::Restart()
{
	map->CleanUp();

	houses.Clear();

	collisions.Clear();

	location1.Clear();

	location2.Clear();

	location2.Clear();

	place = NO_PLACE;
}

void World::Update()
{
	WorldMovement();
	WorldChange();
}

void World::Draw()
{
	map->Draw();
	DrawPlayer();
	DrawEnemy();

	for (int i = 0; i < houses.Count(); i++) app->render->DrawRectangle(houses[i], { 255, 0, 255, 100 });
	for (int i = 0; i < collisions.Count(); i++) app->render->DrawRectangle(collisions[i], {255, 0, 0, 100});
	for (int i = 0; i < location1.Count(); i++) app->render->DrawRectangle(location1[i], { 255, 255, 0, 100 });
	for (int i = 0; i < location2.Count(); i++) app->render->DrawRectangle(location2[i], { 0, 255, 0, 100 });
	for (int i = 0; i < location3.Count(); i++) app->render->DrawRectangle(location3[i], { 0, 0, 255, 100 });
}

void World::DrawPlayer()
{
	app->render->DrawRectangle(app->scene->player1->GetWorldBounds(), {100, 150, 240, 150});
	app->render->DrawRectangle(app->scene->player1->GetCollisionBounds(), { 150, 150, 140, 200 });
}

void World::DrawEnemy()
{

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
				ChangeMap(ENEMY_FILD);
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
	//app->scene->SetScene(WORLD, place);
	app->scene->SetScene(Scenes::MAIN_MENU);
}
