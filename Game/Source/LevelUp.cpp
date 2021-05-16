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

	interfaceTexture = app->tex->Load("Assets/Screens/level_scene.png");
	barTexture = app->tex->Load("Assets/Textures/UI/inventory.png");

	int lvl = app->scene->player1->lvl;
	int nextLvl = lvl + 1;

	char str[24] = {};
	sprintf(str, "Level Up! %d to %d", lvl, nextLvl);
	upgradeText->SetString(str, BROWN);

	char str1[24] = {};
	sprintf(str1, "Gained %d exp", exp);
	expGainedText->SetString(str1, WHITE);

	levelUpBool = false;

	counter = 0;

	plusExp = exp; //Experience given by the enemy
	actualExp = app->scene->player1->exp; //Actual player experience

	int newExp = actualExp + plusExp; //Sum of the actual experience plus the experience given by the enemy

	maxExp = floor(1000 * pow((float)app->scene->player1->lvl, 1.25f));
	
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

	Player* p = app->scene->player1;
	if (levelUpBool)
	{
		Player* p = app->scene->player1;
		int x = p->lvl - 1;

		p->health = floor((x / 2) + 20);
		p->strengthStat = floor((x / 3) + 6);
		p->defenseStat = floor((x / 3) + 3);

	}

	p->exp = newExp;

	p = nullptr;
}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(barTexture);
}

void LevelUp::Update()
{
	if (counter >= app->scene->player1->exp) UpdateButtons();
}

void LevelUp::Draw()
{
	LOG("Drawing lvl up scene...");
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	if (counter >= app->scene->player1->exp) DrawButtons();

    DrawText();

	if (counter < app->scene->player1->exp) counter ++;

	DrawBar({530, 400}, counter, maxExp, BLUE);

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
	if (upgradeText == nullptr)
	{
		upgradeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		upgradeText->bounds = { 600, 220, 300, 100 };
		upgradeText->SetTextFont(app->fontTTF->inventoryFont);

		int lvl = app->scene->player1->lvl;
		int nextLvl = lvl + 1;

		char str[24] = {};
		sprintf(str, "Level Up! %d to %d", lvl, nextLvl);
		upgradeText->SetString(str, WHITE);
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
		expText->SetTextFont(app->fontTTF->inventoryFont);
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

void LevelUp::DrawText()
{
	if (levelUpBool) upgradeText->Draw();

	winText->Draw();
	
	expGainedText->Draw();

	if (counter >= app->scene->player1->exp) skipText->Draw();
}

void LevelUp::DrawButtons()
{
	skipButton->Draw();
}
