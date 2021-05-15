#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "StonePuzzle.h"

StonePuzzle::StonePuzzle() {}

StonePuzzle::StonePuzzle(int stoneAmount, SDL_Rect pathx)
{
	for (int i = 0; i < stoneAmount; i++)
	{
		stone.Add(Stone({ 0, 0, 42, 42 }, "Assets/Textures/Environment/rock.png", "Assets/Textures/Environment/rockunderwater.png"));

	}

	path = pathx;

	stoneInWater = false;
}

StonePuzzle::~StonePuzzle() {}

void StonePuzzle::Restart()
{
	for (int i = 0; i < stone.Count(); i++)
	{
		app->tex->UnLoad(stone[i].texture);
		app->tex->UnLoad(stone[i].underTexture);
	}
	stone.Clear();
}

void StonePuzzle::Update()
{
	Player* p = app->scene->player1;

	if (!stoneInWater)
	{
		for (int i = 0; i < stone.Count(); i++)
		{
			SDL_Rect ampStone = { stone[i].rect.x - 8, stone[i].rect.y - 8, stone[i].rect.w + 16, stone[i].rect.h + 16 };

			if (collisionUtils.CheckCollision(ampStone, p->collisionRect))
			{
				/*if (app->scene->world->GetPlayerState() == PlayerState::LEFT) stone[i].rect.x -= 3;
				else if (app->scene->world->GetPlayerState() == PlayerState::RIGHT) stone[i].rect.x += 3;
				else if (app->scene->world->GetPlayerState() == PlayerState::UP) stone[i].rect.y -= 3;
				else if (app->scene->world->GetPlayerState() == PlayerState::DOWN) stone[i].rect.y += 3;*/

				if (!left && !right && !up && !down)
				{
					if (app->scene->world->GetPlayerState() == PlayerState::LEFT) left = true;
					else if (app->scene->world->GetPlayerState() == PlayerState::RIGHT) right = true;
					else if (app->scene->world->GetPlayerState() == PlayerState::UP) up = true;
					else if (app->scene->world->GetPlayerState() == PlayerState::DOWN) down = true;
				}
			}
			else
			{
				left = false;
				right = false;
				up = false;
				down = false;
			}

			if (left && !right && !up && !down) stone[i].rect.x -= 3;
			else if (!left && right && !up && !down) stone[i].rect.x += 3;
			else if (!left && !right && up && !down) stone[i].rect.y -= 3;
			else if (!left && !right && !up && down) stone[i].rect.y += 3;
		}

		for (int i = 0; i < stone.Count(); i++)
		{
			if (collisionUtils.CheckCollision(path, stone[i].rect))
			{
				stoneInWater = true;
			}
		}
	}
}

void StonePuzzle::Draw()
{
	if (!stoneInWater) for (int i = 0; i < stone.Count(); i++) app->render->DrawTexture(stone[i].texture, stone[i].rect.x, stone[i].rect.y);
	else for (int i = 0; i < stone.Count(); i++) app->render->DrawTexture(stone[i].underTexture, path.x + 40, path.y + 40);
}

void StonePuzzle::DebugDraw()
{
	if (!stoneInWater)
	{
		for (int i = 0; i < stone.Count(); i++) app->render->DrawRectangle(stone[i].rect, { 255, 40, 40, 150 });
		app->render->DrawRectangle(path, { 255, 0, 160, 150 });
	}
}

Stone::Stone() {}

Stone::Stone(SDL_Rect rectx, const char* path, const char* path1)
{
	rect = rectx;
	texture = app->tex->Load(path);
	underTexture = app->tex->Load(path1);
}

Stone::~Stone() {}

