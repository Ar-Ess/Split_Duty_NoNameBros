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
}

void World::Update()
{
	WorldMovement();

	LOG("PlayerY: %d", app->scene->player1->colliderWorld.y);
}

void World::Draw()
{
	map->Draw();
	DrawPlayer();
	DrawEnemy();
}

void World::DrawPlayer()
{
	app->render->DrawRectangle(app->scene->player1->GetWorldBounds(), {100, 150, 240, 150});
}

void World::DrawEnemy()
{

}

//-------------------------------------------------------------------

void World::WorldMovement()
{
	PlayerMovement();
	CameraMovement();
}

void World::PlayerMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->scene->player1->colliderWorld.y -= worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->scene->player1->colliderWorld.y += worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->scene->player1->colliderWorld.x -= worldSpeed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->scene->player1->colliderWorld.x += worldSpeed;
}

void World::CameraMovement()
{
	//1670
	if (app->scene->player1->colliderWorld.y > 317 && app->scene->player1->colliderWorld.y < 1662) if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->render->camera.y += worldSpeed;
	if (app->scene->player1->colliderWorld.y > 318 && app->scene->player1->colliderWorld.y < 1663) if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->render->camera.y -= worldSpeed;
	if (app->scene->player1->colliderWorld.x > 611 && app->scene->player1->colliderWorld.x < 2832) if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->render->camera.x += worldSpeed;
	if (app->scene->player1->colliderWorld.x > 612 && app->scene->player1->colliderWorld.x < 2833) if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->render->camera.x -= worldSpeed;

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
