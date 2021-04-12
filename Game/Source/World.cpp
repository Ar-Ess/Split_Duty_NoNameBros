#include "App.h"
#include "Render.h"

#include "World.h"
#include "Map.h"
#include "Player.h"

#include "Log.h"

World::World()
{
	map = new Map();
}

void World::Start(const char* mapName)
{
	map->Load(mapName);
	UpdateWorldSpeed();
}

void World::Restart()
{
	map->CleanUp();

	collisions.Clear();

	changeMap.Clear();
}

void World::Update()
{
	WorldMovement();
}

void World::Draw()
{
	map->Draw();
	DrawPlayer();
	DrawEnemy();

	for (int i = 0; i < collisions.Count(); i++) app->render->DrawRectangle(collisions[i], {255, 0, 0, 100});
	for (int i = 0; i < changeMap.Count(); i++) app->render->DrawRectangle(changeMap[i], { 0, 0, 255, 100 });
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
