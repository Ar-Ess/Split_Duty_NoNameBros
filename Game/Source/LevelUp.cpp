#define _CRT_SECURE_NO_WARNINGS

#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "EntityManager.h"

#include "LevelUp.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"

#include "Log.h"

LevelUp::LevelUp()
{	
}

void LevelUp::Start(short int exp)
{
	SetText();

	interfaceTexture = app->tex->Load("Assets/Textures/UI/level-up.png");

	int lvl = app->scene->player1->lvl;
	int prevLvl = 0;
	if (lvl != 0) int prevLvl = lvl - 1;

	char str[24] = {};
	sprintf(str, "Level Up! %d to %d", prevLvl, lvl);
	upgradeText->SetString(str, BROWN);

	levelUpBool = false;

	plusExp = exp; //Experience given by the enemy
	actualExp = app->scene->player1->exp; //Actual player experience

	int newExp = actualExp + plusExp; //Sum of the actual experience plus the experience given by the enemy

	maxExp = floor(pow((float)app->scene->player1->lvl, 1.25f));
	
	if (newExp > maxExp)
	{
		app->scene->player1->lvl++;
		int diffExp = newExp - maxExp;
		newExp = diffExp;
		levelUpBool = true;
	}
	else if (newExp == maxExp)
	{
		app->scene->player1->lvl++;
		newExp = 0;
		levelUpBool = true;
	}

	if (levelUpBool)
	{
		Player* p = app->scene->player1;

		p->health += ((p->lvl - 1) / 2) + 10

		p = nullptr;
	}
}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(barTexture);
}

void LevelUp::Update()
{
	UpdateButtons();
}

void LevelUp::Draw()
{
	LOG("Drawing lvl up scene...");
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	DrawButtons();

	DrawText();

	DrawBar({600, 400}, app->scene->player1->exp, maxExp, BLUE);

	//app->guiManager->DrawCursor();
}

void LevelUp::DrawBar(iPoint pos, int current, int max, SDL_Color color)
{
	int size = 261;
	int thickness = 20;
	int percent = 1;

	if (current > 0 && max > 0) percent = current / max;

	app->render->DrawRectangle({ pos.x,pos.y,size * percent,thickness }, BLUE, true, false);
	app->render->DrawTexture(barTexture, pos.x, pos.y, 1, false, &expBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

}

void LevelUp::UpdateButtons()
{
	skipButton->Update(1.0f);
}

void LevelUp::SetText()
{
	if (upgradeText == nullptr)
	{
		upgradeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		upgradeText->bounds = { 600, 200, 300, 100 };
		upgradeText->SetTextFont(app->fontTTF->inventoryFont);

		int lvl = app->scene->player1->lvl;
		int prevLvl = 0;
		if (lvl != 0) int prevLvl = lvl - 1;

		char str[24] = {};
		sprintf(str, "Level Up! %d to %d", prevLvl, lvl);
		upgradeText->SetString(str, BROWN);
	}
}

void LevelUp::DrawText()
{
	if (levelUpBool) upgradeText->Draw();
}

void LevelUp::DrawButtons()
{
	skipButton->Draw(1, false);
}
