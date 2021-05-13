#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "ButtonsPuzzle.h"

ButtonPuzzle::ButtonPuzzle() {}

ButtonPuzzle::ButtonPuzzle(int buttonAmount, uint width, uint height)
{
	for (int i = 0; i < buttonAmount; i++)
	{
		button.Add(Button({ 0, 0, width, height }));
	}
}

ButtonPuzzle::~ButtonPuzzle() {}

void ButtonPuzzle::Restart()
{

}

void ButtonPuzzle::Update()
{
	if ()
}

void ButtonPuzzle::Draw()
{

}

void ButtonPuzzle::DebugDraw()
{
	for (int i = 0; i < button.Count(); i++) app->render->DrawRectangle(button[i].rect, {0, 230, 170, 150})
}