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
}

void LilipadPuzzle::Update()
{
	SwitchLogic();
}

void LilipadPuzzle::Draw()
{
	for (int i = 0; i < lilipad.Count(); i++) app->render->DrawRectangle(lilipad[i].rect, {255, 255, 40, 255});
}

void LilipadPuzzle::DebugDraw()
{
	app->render->DrawRectangle(leftSwitch, {200, 80, 100, 150});
	app->render->DrawRectangle(rightSwitch, { 200, 80, 100, 150 });
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

Lilipad LilipadPuzzle::GetLiliFromId(uint id)
{
	for (int i = 0; i < lilipad.Count(); i++)
	{
		if (lilipad[i].id == id) return lilipad.At(i)->data;
	}
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
