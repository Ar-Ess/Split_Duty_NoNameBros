#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "ButtonsPuzzle.h"
#include "Audio.h"

ButtonPuzzle::ButtonPuzzle() {}

ButtonPuzzle::ButtonPuzzle(int buttonAmount, int width, int height, SDL_Rect bridge1x, SDL_Rect bridge2x, SDL_Rect bridge3x)
{
	for (int i = 0; i < buttonAmount; i++)
	{
		button.Add(Button({ 0, 0, width, height }));
	}

	bridge = app->tex->Load("Assets/Textures/Environment/bridge.png");

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
						app->audio->SetFx(Effect::OK_FX);
					}
					else
					{
						//FX of failure
						app->audio->SetFx(Effect::MEH_FX);
						ResetButtons();
					}
				}
			}
		}

		if (step == (button.Count() + 1))
		{
			//FX of win
			app->audio->SetFx(Effect::BUTTON_SOLVED_FX);
			resultActive = true;
		}
	}
}

void ButtonPuzzle::Draw()
{
	if (resultActive)
	{
		app->render->DrawTexture(bridge, bridge1.x, bridge1.y);
		app->render->DrawTexture(bridge, bridge2.x, bridge2.y);
		app->render->DrawTexture(bridge, bridge3.x, bridge3.y);
	}
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
