#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "ButtonsPuzzle.h"

ButtonPuzzle::ButtonPuzzle() {}

ButtonPuzzle::ButtonPuzzle(int buttonAmount, int width, int height, SDL_Rect bridge1x, SDL_Rect bridge2x, SDL_Rect bridge3x)
{
	for (int i = 0; i < buttonAmount; i++)
	{
		button.Add(Button({ 0, 0, width, height }));
	}

	step = 1;

	resultActive = false;

	bridge1 = bridge1x;
	bridge2 = bridge2x;
	bridge3 = bridge3x;
}

ButtonPuzzle::~ButtonPuzzle() {}

void ButtonPuzzle::Restart()
{
	button.Clear();
	app->tex->UnLoad(bridge);
}

void ButtonPuzzle::Update()
{
	if (!resultActive)
	{
		for (int i = 0; i < button.Count(); i++)
		{
			if (!button[i].active && collisionUtils.CheckCollision(button[i].rect, app->scene->player1->collisionRect))
			{
				if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetControl(A) == KEY_DOWN)
				{
					if (button[i].order == step)
					{
						step++;
						button[i].active = true;
					}
					else
					{
						//FX of failure
						ResetButtons();
					}
				}
			}
		}

		if (step == (button.Count() + 1))
		{
			//FX of win
			resultActive = true;
		}
	}
}

void ButtonPuzzle::Draw()
{

}

void ButtonPuzzle::DebugDraw()
{
	for (int i = 0; i < button.Count(); i++) app->render->DrawRectangle(button[i].rect, { 0, 230, 170, 150 });

	if (!resultActive)
	{
		app->render->DrawRectangle(bridge1, { 230, 23, 170, 150 });
		app->render->DrawRectangle(bridge2, { 230, 23, 170, 150 });
		app->render->DrawRectangle(bridge3, { 230, 23, 170, 150 });
	}
}

void ButtonPuzzle::ResetButtons()
{
	for (int i = 0; i < button.Count(); i++)
	{
		button[i].active = false;
	}
	step = 1;
}

Button::Button()
{

}

Button::Button(SDL_Rect rectx)
{
	rect = rectx;
}

Button::~Button()
{
}
