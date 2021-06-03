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

void LevelUp::Start(int exp)
{
	SetText();

	interfaceTexture = app->tex->Load("Assets/Screens/level_scene.png");
	barTexture = app->tex->Load("Assets/Textures/UI/inventory.png");
	interfaceGui = app->tex->Load("Assets/Screens/level_scene_gui.png");

	char str1[24] = {};
	sprintf(str1, "Gained %d exp", exp);
	expGainedText->SetString(str1, WHITE);

	levelUpBool = false;

	//------------------------------------------
	Player* p = app->scene->player1;

	actualExp = p->exp; //Actual player experience
	counter = actualExp; // Counter for exp animation
	actualLevel = p->lvl;
	maxExp = floor(1000 * pow((float)app->scene->player1->lvl, 1.25f)); //Max EXP for actual lvl

	sumUp = ceil(exp / 475);
	if (sumUp <= 0) sumUp = 1;

	int newExp = actualExp + exp; //Sum of the actual experience plus the experience given by the enemy
	
	while (newExp > maxExp)
	{
		if (newExp > maxExp)
		{
			p->lvl++;
			int diffExp = newExp - maxExp;
			newExp = diffExp;
			levelUpBool = true;
			counter = 0;
		}
		else if (newExp == maxExp)
		{
			p->lvl++;
			newExp = 0;
			levelUpBool = true;
			counter = 0;
		}

		maxExp = floor(1000 * pow((float)p->lvl, 1.25f)); //Max EXP for actual lvl
	}

	if (levelUpBool)
	{
		float x = (float)p->lvl;

		p->maxHealth = ceil((x / 2.0f) + 20.0f);
		p->strengthStat = ceil((x / 2.8f) + 6.0f);
		p->defenseStat = ceil((x / 3.0f) + 3.0f);
		if (p->luckStat != 0) p->luckStat = floor((x / 5.0f) + 1.0f);
		if (p->stabStat != 0) p->stabStat = floor(x / 4.0f);
		if (p->velocityStat != 0) p->velocityStat = floor((x / 3.0f) + 5.0f);
	}

	nextLevel = p->lvl;
	p->exp = newExp;

	p = nullptr;

	//TEXTS
	char str[24] = {};
	sprintf(str, "Level Up! %d to %d", actualLevel, nextLevel);
	levelUpText->SetString(str, WHITE);
}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(interfaceGui);
	app->tex->UnLoad(barTexture);
}

void LevelUp::Update()
{
	if (counter >= app->scene->player1->exp) UpdateButtons();
}

void LevelUp::Draw(int x)
{
	LOG("Drawing lvl up scene...");
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(interfaceGui, x, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	if (counter >= app->scene->player1->exp) DrawButtons(x);

    DrawText(x);

	if (counter < app->scene->player1->exp) counter += sumUp;


	DrawBar({530 + x, 400}, counter, maxExp, BLUE);

	//app->guiManager->DrawCursor();
}

void LevelUp::DrawBar(iPoint pos, int current, int max, SDL_Color color)
{
	int size = 261;
	int thickness = 20;
	float percent = 1;

	float curr = current;
	float maxim = max;

	if (current > 0 && max > 0) percent = curr / maxim;

	app->render->DrawRectangle({ pos.x,pos.y,size,thickness }, { 143, 143, 255, 255 }, true, false);
	app->render->DrawRectangle({ pos.x,pos.y,int(floor(size * percent)),thickness }, {0, 40, 200, 255}, true, false);
	app->render->DrawTexture(barTexture, pos.x, pos.y, 1, false, &expBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

}

void LevelUp::UpdateButtons()
{
	skipButton->Update(1.0f);
}

void LevelUp::SetText()
{
	if (levelUpText == nullptr)
	{
		levelUpText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		levelUpText->bounds = { 570, 180, 300, 100 };
		levelUpText->SetTextFont(app->fontTTF->defaultFont);

		int lvl = app->scene->player1->lvl;
		int nextLvl = lvl + 1;

		char str[24] = {};
		sprintf(str, "Level Up! %d to %d", lvl, nextLvl);
		levelUpText->SetString(str, WHITE);
	}

	if (winText == nullptr)
	{
		winText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		winText->bounds = { 600, 180, 300, 100 };
		winText->SetTextFont(app->fontTTF->defaultFont);

		winText->SetString("YOU WON!", WHITE);
	}

	if (expGainedText == nullptr)
	{
		expGainedText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expGainedText->bounds = { 550, 220, 300, 100 };
		expGainedText->SetTextFont(app->fontTTF->inventoryFont);

		char str[24] = {};
		sprintf(str, "Gained %d exp", 0);
		expGainedText->SetString(str, WHITE);
	}

	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { 500, 400, 300, 100 };
		expText->SetTextFont(app->fontTTF->defaultFont);
		expText->SetString("EXP: ");
	}

	if (skipText == nullptr)
	{
		skipText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		skipText->bounds = { 580, 580, 161, 62 };
		skipText->SetTextFont(app->fontTTF->inventoryFont);
		skipText->SetString("RETURN ");
		skipText->CenterAlign();
	}
}

void LevelUp::DrawText(int x)
{
	if (levelUpBool)
	{
		levelUpText->bounds.x += x;
		levelUpText->Draw();
		levelUpText->bounds.x -= x;
	}

	if (!levelUpBool)
	{
		winText->bounds.x += x;
		winText->Draw();
		winText->bounds.x -= x;
	}
	
	expGainedText->bounds.x += x;
	expGainedText->Draw();
	expGainedText->bounds.x -= x;

	if (counter >= app->scene->player1->exp)
	{
		skipText->bounds.x += x;
		skipText->Draw();
		skipText->bounds.x -= x;
	}
}

void LevelUp::DrawButtons(int x)
{
	skipButton->bounds.x += x;

	skipButton->Draw();

	skipButton->bounds.x -= x;
}
