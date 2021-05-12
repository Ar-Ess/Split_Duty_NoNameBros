#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "LilipadPuzzle.h"

LilipadPuzzle::LilipadPuzzle()
{

}

LilipadPuzzle::LilipadPuzzle(int lilipadAmount, SDL_Rect leftSwitchRect, SDL_Rect rightSwitchRect)
{
	for (int i = 0; i < lilipadAmount; i++)
	{
		lilipad.Add(Lilipad({ 0, 0, 56, 56 }, nullptr, (uint)i));
	}

	leftSwitch = leftSwitchRect;
	rightSwitch = rightSwitchRect;
}

LilipadPuzzle::~LilipadPuzzle()
{
}

void LilipadPuzzle::Start()
{
}

void LilipadPuzzle::Restart()
{
	for (int i = 0; i < lilipad.Count(); i++) app->tex->UnLoad(lilipad[i].texture);
	lilipad.Clear();

	riverTracks.Clear();
}

void LilipadPuzzle::Update()
{
	SwitchLogic();
	RiverTrackLogic();
}

void LilipadPuzzle::Draw()
{
	for (int i = 0; i < lilipad.Count(); i++) app->render->DrawRectangle(lilipad[i].rect, { 40, 255, 40, 150 });
}

void LilipadPuzzle::DebugDraw()
{
	app->render->DrawRectangle(leftSwitch, {200, 80, 100, 150});
	app->render->DrawRectangle(rightSwitch, { 200, 80, 100, 150 });

	for (int i = 0; i < riverTracks.Count(); i++) if (riverTracks[i].active) app->render->DrawRectangle(riverTracks[i].rect, {0, 100, 100, 150});

	for (int i = 0; i < lilipad.Count(); i++) app->render->DrawRectangle(lilipad[i].rect, {255, 255, 40, 150});
}

void LilipadPuzzle::SwitchLogic()
{
	bool left = collisionUtils.CheckCollision(leftSwitch, app->scene->player1->collisionRect);
	bool right = collisionUtils.CheckCollision(rightSwitch, app->scene->player1->collisionRect);

	if (!switchPressed && left)
	{
		switchPressed = true;
		for (int i = 0; i < lilipad.Count(); i++)
		{
			if (lilipad[i].moves <= 0)
			{
				return;
			}
		}
		for (int i = 0; i < lilipad.Count(); i++) lilipad[i].MoveLeft();
		return;
	}

	if (!switchPressed && right)
	{
		switchPressed = true;
		for (int i = 0; i < lilipad.Count(); i++)
		{
			if (lilipad[i].moves >= MAX_MOVES)
			{
				return;
			}
		}
		for (int i = 0; i < lilipad.Count(); i++) lilipad[i].MoveRight();
		return;
	}

	if (!left && !right) switchPressed = false;
}

void LilipadPuzzle::RiverTrackLogic()
{
	for (int a = 0; a < riverTracks.Count(); a++)
	{
		for (int i = 0; i < lilipad.Count(); i++)
		{
			SDL_Rect ampLilipad = {};

			if (i == 0 && lilipad[i].moves == 15) ampLilipad = { lilipad[i].rect.x, lilipad[i].rect.y - 28, lilipad[i].rect.w, lilipad[i].rect.h + 28 };
			else if (i == 1 && lilipad[i].moves == 4) ampLilipad = { lilipad[i].rect.x, lilipad[i].rect.y - 28, lilipad[i].rect.w, lilipad[i].rect.h + 28 };
			else ampLilipad = lilipad[i].rect;

			if (collisionUtils.CheckCollision(riverTracks[a].rect, ampLilipad))
			{
				riverTracks[a].active = false;
				break;
			}
			else riverTracks[a].active = true;
		}
	}
}

Lilipad LilipadPuzzle::GetLiliFromId(uint id)
{
	for (int i = 0; i < lilipad.Count(); i++)
	{
		if (lilipad[i].id == id) return lilipad.At(i)->data;
	}
	return {};
}

Lilipad::Lilipad()
{

}

Lilipad::Lilipad(SDL_Rect rectx, SDL_Texture* texx, uint idx)
{
	rect = rectx;
	texture = texx;
	id = idx;
}

Lilipad::~Lilipad()
{
}

void Lilipad::MoveLeft()
{
	rect.x -= rect.w;
	moves--;
}

void Lilipad::MoveRight()
{
	rect.x += rect.w;
	moves++;
}
