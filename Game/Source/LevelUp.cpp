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
#include "Combat.h"

#include "Log.h"

LevelUp::LevelUp()
{	
}

void LevelUp::Start(int expGained)
{
	SetText();

	interfaceTexture = app->tex->Load("Assets/Screens/level_scene.png");
	barTexture = app->tex->Load("Assets/Textures/UI/inventory.png");
	interfaceGui = app->tex->Load("Assets/Screens/level_scene_gui.png");

	char str1[24] = {};
	sprintf(str1, "Gained %d exp", expGained);
	expGainedText->SetString(str1, WHITE);

	levelUpBool = false;

	//------------------------------------------
	Player* p = app->scene->player1;
	/*
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
	*/

	startExp = p->exp;
	startLvl = p->lvl;
	endExp = p->exp;
	endLvl = p->lvl;
	counter = startExp;

	bool end = false;

	while (!end)
	{
		maxExp = floor(1000 * pow((float)endLvl, 1.25f));

		if (expGained >= maxExp)
		{
			endLvl++;
			expGained -= maxExp;
			endExp = expGained;
			levelUpBool = true;
			counter = 0;
		}
		else
		{
			endExp = expGained;
			end = true;
		}
	}

	if (endExp == 0) endExp = 1;
	sumUp = (0.5f * endExp) / 100;

	if (endLvl > 100)
	{
		endLvl = 100;
		endExp = 0;
		maxLevel = true;
	}

	UpgradeStats(endLvl);
	if (app->scene->combatScene->GetSecondPlayerExistance()) Upgrade2Stats(endLvl);

	p->exp = endExp;
	p->lvl = endLvl;
	p = nullptr;

	//TEXTS
	char str[24] = {};
	sprintf(str, "Level Up! %d to %d", startLvl, endLvl);
	levelUpText->SetString(str, WHITE);
}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(interfaceGui);
	app->tex->UnLoad(barTexture);

	maxExp = 0;
	startLvl = 0;
	endLvl = 0;
	startExp = 0;
	endExp = 0;
	levelUpBool = false;
	maxLevel = false;
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

	if (!maxLevel) if (counter < app->scene->player1->exp) counter += sumUp;


	DrawBar({530 + x, 400}, counter, maxExp, BLUE);
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

void LevelUp::UpgradeStats(int x)
{
	Player* p = app->scene->player1;

	int mHealth= ceil((x / 2.0f) + 20.0f);
	p->health = (mHealth * p->health) / p->maxHealth;
	p->maxHealth = mHealth;

	p->strengthStat = ceil((x / 2.8f) + 6.0f);

	p->defenseStat = ceil((x / 3.0f) + 3.0f);

	if (p->luckStat != 0) p->luckStat = ceil((x / 5.0f));

	if (p->stabStat != 0) p->stabStat = floor(x / 4.0f);

	if (p->velocityStat != 0) p->velocityStat = floor((x / 3.0f) + 5.0f);

	p = nullptr;
}

void LevelUp::Upgrade2Stats(int x)
{
	Player* p = app->scene->player2;

	int mHealth = ceil(pow((float)x, 0.75f) + 5);
	p->health = (mHealth * p->health) / p->maxHealth;
	p->maxHealth = mHealth;

	p->strengthStat = ceil((x / 3.5f) + 3.0f);

	p->defenseStat = ceil(pow((float)x, (9.0f / 14.0f)) + 2.0f);
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

	if (maxLvlText == nullptr)
	{
		maxLvlText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		maxLvlText->bounds = { 600, 180, 300, 100 };
		maxLvlText->SetTextFont(app->fontTTF->defaultFont);

		maxLvlText->SetString("MAX LEVEL REACHED", WHITE);
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
	if (!maxLevel)
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
	}
	else
	{
		maxLvlText->bounds.x += x;
		maxLvlText->Draw();
		maxLvlText->bounds.x -= x;
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
