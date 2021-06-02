#define _CRT_SECURE_NO_WARNINGS

#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "EntityManager.h"

#include "Inventory.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"
#include "Defs.h"

#include "Log.h"
#include <string>
#include <stdio.h>

Inventory::Inventory()
{

}

void Inventory::Start()
{
	playerLvl = app->scene->player1->lvl;
	//healthPercent = app->scene->player1->health / app->scene->player1->maxHealth;

	playerHp = app->scene->player1->health;
	playerExp = app->scene->player1->exp;

	interfaceTexture = app->tex->Load("Assets/Textures/UI/inventory.png");
	faceAnimationTexture = app->tex->Load("Assets/Textures/UI/face_animations.png");
	statsTexture = app->tex->Load("Assets/Textures/UI/stats.png");

	idleFaceAnim.PushBack({ 0, 0, 70, 68 });

	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 70, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.speed = 0.05f;
	blinkFaceAnim.loop = false;

	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.PushBack({ 0, 70, 70, 68 });
	angryFaceAnim.PushBack({ 70, 70, 70, 68 });
	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.speed = 0.05f;
	angryFaceAnim.loop = false;

	currPlayerFaceAnim = &idleFaceAnim;

	//TEXT
	SetText();

	char str[5] = {};
	sprintf(str, "%d", app->scene->player1->lvl);
	lvlText->SetString(str, YELLOW);

	//STATS TEXT
	char str1[5] = {};
	sprintf(str1, "%d", app->scene->player1->healthStat);
	healthStatText->SetString(str1, BROWN);

	char str2[5] = {};
	sprintf(str2, "%d", app->scene->player1->strengthStat);
	strenghtStatText->SetString(str2, BROWN);

	char str3[5] = {};
	sprintf(str3, "%d", app->scene->player1->defenseStat);
	defenseStatText->SetString(str3, BROWN);

	char str4[5] = {};
	sprintf(str4, "%d", app->scene->player1->velocityStat);
	velocityStatText->SetString(str4, BROWN);

	char str5[5] = {};
	sprintf(str5, "%d", app->scene->player1->stabStat);
	stabStatText->SetString(str5, BROWN);

	char str6[5] = {};
	sprintf(str6, "%d", app->scene->player1->luckStat);
	luckStatText->SetString(str6, BROWN);

	//HEALTH 
	char hpText[20] = {};
	sprintf(hpText, "%d / %d", app->scene->player1->health, app->scene->player1->maxHealth);
	healthText->SetString(hpText, YELLOW);

	//XP
	char xpText[20] = {};
	maxExp = int(floor(1000 * pow((float)app->scene->player1->lvl, 1.25f)));

	sprintf(xpText, "%d / %d", app->scene->player1->exp, maxExp);
	expText->SetString(xpText, YELLOW);
		
}

void Inventory::Restart()
{
	app->scene->iterations = 0;

	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(faceAnimationTexture);
	app->tex->UnLoad(statsTexture);

	blinkFaceAnim.Reset();
	angryFaceAnim.Reset();
	idleFaceAnim.Reset();
}

void Inventory::Update()
{
	UpdateText();

	UpdateFace();

	UpdateButtons();
}

void Inventory::Draw(int y)
{
	DrawInterface(y);

	DrawFace(y);

	DrawBar({ healthBarPos.x, healthBarPos.y + y }, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar({ expBarPos.x, expBarPos.y + y }, app->scene->player1->exp, maxExp, BLUE);

	//DrawStats();

	DrawButtons(y);

	DrawText(y);

	//app->guiManager->DrawCursor();

}

void Inventory::DrawInterface(int y)
{
	app->render->DrawTexture(interfaceTexture, 0, y, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawBar(iPoint pos, int current, int max, SDL_Color color)
{
	int size = 261;
	int thickness = 20;
	float percent = 1;

	float curr = current;
	float maxim = max;

	if (current > 0 && max > 0) percent = curr / maxim;

	if (color.r > 240) //red
	{
		app->render->DrawRectangle({ pos.x,pos.y,size,thickness }, { 200, 40, 0, 255 }, true, false);
		app->render->DrawRectangle({ pos.x,pos.y,int(floor(size * percent)),thickness }, { 255, 143, 143, 255 }, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &healthBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	}
	else
	{
		app->render->DrawRectangle({ pos.x,pos.y,size,thickness }, { 143, 143, 255, 255 }, true, false);
		app->render->DrawRectangle({ pos.x,pos.y,int(floor(size * percent)),thickness }, { 0, 40, 200, 255 }, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &expBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	}

}

void Inventory::UpdateButtons()
{
	littleBeefButton->Update(1.0f);
	bigBeefButton->Update(1.0f);
	featherButton->Update(1.0f);
	mantisButton->Update(1.0f);
	coinButton->Update(1.0f);
	splitButton->Update(1.0f);

	if (app->scene->player1->smallMeatCount == 0 && littleBeefButton->state != GuiControlState::DISABLED) littleBeefButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->smallMeatCount > 0 && littleBeefButton->state == GuiControlState::DISABLED) littleBeefButton->state = GuiControlState::NORMAL;

	if (app->scene->player1->largeMeatCount == 0 && bigBeefButton->state != GuiControlState::DISABLED) bigBeefButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->largeMeatCount > 0 && bigBeefButton->state == GuiControlState::DISABLED) bigBeefButton->state = GuiControlState::NORMAL;
	
	if (app->scene->player1->featherCount == 0 && featherButton->state != GuiControlState::DISABLED) featherButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->featherCount > 0 && featherButton->state == GuiControlState::DISABLED) featherButton->state = GuiControlState::NORMAL;
	
	if (app->scene->player1->mantisRodCount == 0 && mantisButton->state != GuiControlState::DISABLED) mantisButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->mantisRodCount > 0 && mantisButton->state == GuiControlState::DISABLED) mantisButton->state = GuiControlState::NORMAL;
	
	if (app->scene->player1->moneyCount == 0 && coinButton->state != GuiControlState::DISABLED) coinButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->moneyCount > 0 && coinButton->state == GuiControlState::DISABLED) coinButton->state = GuiControlState::NORMAL;
	
	if (app->scene->player1->splitedEnemyCount == 0 && splitButton->state != GuiControlState::DISABLED) splitButton->state = GuiControlState::DISABLED;
	else if (app->scene->player1->splitedEnemyCount > 0 && splitButton->state == GuiControlState::DISABLED) splitButton->state = GuiControlState::NORMAL;
}

void Inventory::DrawButtons(int y)
{
	littleBeefButton->bounds.y += y;
	bigBeefButton->bounds.y += y;
	featherButton->bounds.y += y;
	mantisButton->bounds.y += y;
	coinButton->bounds.y += y;
	splitButton->bounds.y += y;

	littleBeefButton->Draw(1, true, true, ButtonType::LITTLE_BEEF_B);
	bigBeefButton->Draw(1, true, true, ButtonType::BIG_BEEF_B);
	featherButton->Draw(1, true, true, ButtonType::FEATHER_B);
	mantisButton->Draw(1, true, true, ButtonType::MANTIS_B);
	coinButton->Draw(1, true, true, ButtonType::COIN_B);
	splitButton->Draw(1, true, true, ButtonType::SPLIT_B);

	littleBeefButton->bounds.y -= y;
	bigBeefButton->bounds.y -= y;
	featherButton->bounds.y -= y;
	mantisButton->bounds.y -= y;
	coinButton->bounds.y -= y;
	splitButton->bounds.y -= y;
}

void Inventory::UpdateFace()
{
	counter++;

	if (counter % 60 == 0)
	{
		seconds++;
		//LOG("seconds : %d", seconds);
	}

	if (seconds >= 3)
	{
		blinkFaceAnim.Reset();
		currPlayerFaceAnim = &blinkFaceAnim;
		seconds = 0;
	}
}

void Inventory::DrawFace(int y)
{
	currPlayerFaceAnim->Update(1.0f);

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y + y, 1.4f, false, &currPlayerFaceAnim->GetCurrentFrame(), 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::SetText()
{
	//player
	if (lvlText == nullptr)
	{
		lvlText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		lvlText->bounds = { 1000, 200, 200, 50 };
		lvlText->SetTextFont(app->fontTTF->inventoryFont);
		

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->lvl);
		lvlText->SetString(str, YELLOW);
	}

	if (healthText == nullptr)
	{
		healthText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthText->bounds = { healthBarPos.x, healthBarPos.y+33, 200, 50 };
		healthText->SetTextFont(app->fontTTF->defaultFont);

	}

	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { expBarPos.x, expBarPos.y+33, 200, 50 };
		expText->SetTextFont(app->fontTTF->defaultFont);

	}

	//ITEMS
	if (littleBeefText == nullptr)
	{
		littleBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		littleBeefText->bounds = { numberItemPos.x, numberItemPos.y, 50, 50 };
		littleBeefText->SetTextFont(app->fontTTF->inventoryFont);
		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->smallMeatCount);
		littleBeefText->SetString(str, BROWN);
		littleBeefText->CenterAlign();
	}
	if (bigBeefText == nullptr)
	{
		bigBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		bigBeefText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y, 50,50 };
		bigBeefText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->largeMeatCount);
		bigBeefText->SetString(str, BROWN);
		bigBeefText->CenterAlign();
	}
	if (featherText == nullptr)
	{
		featherText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		featherText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y, 50,50 };
		featherText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->featherCount);
		featherText->SetString(str, BROWN);
		featherText->CenterAlign();
	}
	if (mantisText == nullptr)
	{
		mantisText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		mantisText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y, 50,50 };
		mantisText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->mantisRodCount);
		mantisText->SetString(str, BROWN);
		mantisText->CenterAlign();
	}
	if (coinText == nullptr)
	{
		coinText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		coinText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y * 2, 50,50 };
		coinText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->moneyCount);
		coinText->SetString(str, BROWN);
		coinText->CenterAlign();
	}
	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y * 2, 50,50 };
		splitText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->splitedEnemyCount);
		splitText->SetString(str, BROWN);
		splitText->CenterAlign();
	}

	//Stats
	if (healthStatText == nullptr)
	{
		healthStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthStatText->bounds = { statsPos.x,statsPos.y,50,50 };
		healthStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->healthStat);
		healthStatText->SetString(str, BROWN);
		healthStatText->CenterAlign();
	}
	if (strenghtStatText == nullptr)
	{
		strenghtStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strenghtStatText->bounds = { statsPos.x,statsPos.y + statsOff.y,50,50 };
		strenghtStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->strengthStat);
		strenghtStatText->SetString(str, BROWN);
		strenghtStatText->CenterAlign();
	}
	if (defenseStatText == nullptr)
	{
		defenseStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		defenseStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 2,50,50 };
		defenseStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->defenseStat);
		defenseStatText->SetString(str, BROWN);
		defenseStatText->CenterAlign();
	}
	if (velocityStatText == nullptr)
	{
		velocityStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		velocityStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 3,50,50 };
		velocityStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->velocityStat);
		velocityStatText->SetString(str, BROWN);
		velocityStatText->CenterAlign();
	}
	if (luckStatText == nullptr)
	{
		luckStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		luckStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 4,50,50 };
		luckStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->luckStat);
		luckStatText->SetString(str, BROWN);
		luckStatText->CenterAlign();
	}
	if (stabStatText == nullptr)
	{
		stabStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		stabStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,50,50 };
		stabStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->stabStat);
		stabStatText->SetString(str, BROWN);
		stabStatText->CenterAlign();
	}
	if (app->questManager->currentQuest != nullptr)
	{
		if (currQuestTitleText == nullptr)
		{
			currQuestTitleText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
			currQuestTitleText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,50,50 };
			currQuestTitleText->SetTextFont(app->fontTTF->inventoryFont);

			char str[5] = {};
			sprintf(str, "%d", app->questManager->currentQuest->textTitle);
			currQuestTitleText->SetString(str, BROWN);
			currQuestTitleText->CenterAlign();
		}
		if (currQuestText == nullptr)
		{
			currQuestText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
			currQuestText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,50,50 };
			currQuestText->SetTextFont(app->fontTTF->inventoryFont);

			char str[5] = {};
			sprintf(str, "%d", app->questManager->currentQuest->textTitle);
			currQuestText->SetString(str, BROWN);
			currQuestText->CenterAlign();
		}
	}

	//quest
	
}

void Inventory::UpdateText()
{
	//ITEMS
	char str[5] = {};
	sprintf(str, "%d", app->scene->player1->smallMeatCount);
	littleBeefText->SetString(str, BROWN);

	char str1[5] = {};
	sprintf(str1, "%d", app->scene->player1->largeMeatCount);
	bigBeefText->SetString(str1, BROWN);

	char str2[5] = {};
	sprintf(str2, "%d", app->scene->player1->featherCount);
	featherText->SetString(str2, BROWN);

	char str3[5] = {};
	sprintf(str3, "%d", app->scene->player1->mantisRodCount);
	mantisText->SetString(str3, BROWN);

	char str4[5] = {};
	sprintf(str4, "%d", app->scene->player1->moneyCount);
	coinText->SetString(str4, BROWN);

	char str5[5] = {};
	sprintf(str5, "%d", app->scene->player1->splitedEnemyCount);
	splitText->SetString(str5, BROWN);

	//STAT

}

void Inventory::DrawText(int y)
{
	healthText->bounds.y += y;
	expText->bounds.y += y;
	lvlText->bounds.y += y;

	littleBeefText->bounds.y += y;
	bigBeefText->bounds.y += y;
	featherText->bounds.y += y;
	mantisText->bounds.y += y;
	coinText->bounds.y += y;
	splitText->bounds.y += y;

	healthStatText->bounds.y += y;
	strenghtStatText->bounds.y += y;
	defenseStatText->bounds.y += y;
	velocityStatText->bounds.y += y;
	stabStatText->bounds.y += y;
	luckStatText->bounds.y += y;

	healthText->Draw();
	expText->Draw();
	lvlText->Draw();

	littleBeefText->Draw();
	bigBeefText->Draw();
	featherText->Draw();
	mantisText->Draw();
	coinText->Draw();
	splitText->Draw();

	healthStatText->Draw();
	strenghtStatText->Draw();
	defenseStatText->Draw();
	velocityStatText->Draw();
	stabStatText->Draw();
	luckStatText->Draw();

	healthText->bounds.y -= y;
	expText->bounds.y -= y;
	lvlText->bounds.y -= y;

	littleBeefText->bounds.y -= y;
	bigBeefText->bounds.y -= y;
	featherText->bounds.y -= y;
	mantisText->bounds.y -= y;
	coinText->bounds.y -= y;
	splitText->bounds.y -= y;

	healthStatText->bounds.y -= y;
	strenghtStatText->bounds.y -= y;
	defenseStatText->bounds.y -= y;
	velocityStatText->bounds.y -= y;
	stabStatText->bounds.y -= y;
	luckStatText->bounds.y -= y;
}

void Inventory::UpdateHealthText()
{
	char hpText[20] = {};
	sprintf(hpText, "%d / %d", app->scene->player1->health, app->scene->player1->maxHealth);
	healthText->SetString(hpText, YELLOW);
}
