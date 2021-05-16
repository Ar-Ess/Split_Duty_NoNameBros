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
	faceAnimationTexture = app->tex->Load("Assets/Textures/UI/face-animations.png");
	statsTexture = app->tex->Load("Assets/Textures/UI/stats.png");

	idleFaceAnim.PushBack({ 0, 0, 70, 68 });

	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 70, 0, 70, 68 });
	blinkFaceAnim.PushBack({ 0, 0, 70, 68 });
	blinkFaceAnim.speed = 0.1f;
	blinkFaceAnim.loop = false;

	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.PushBack({ 0, 70, 70, 68 });
	angryFaceAnim.PushBack({ 70, 70, 70, 68 });
	angryFaceAnim.PushBack({ 0, 0, 70, 68 });
	angryFaceAnim.speed = 0.1f;
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
	int maxExp = int(floor(1000 * pow((float)app->scene->player1->lvl, 1.25f)));

	sprintf(xpText, "%d / %d", app->scene->player1->exp, maxExp);
	expText->SetString(xpText, YELLOW);
		
}

void Inventory::Restart()
{
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

	UpdateButtons();
}

void Inventory::Draw()
{
	DrawInterface();

	DrawFace();

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->exp, BLUE);

	//DrawStats();

	DrawButtons();

	DrawText();

	//app->guiManager->DrawCursor();

}

void Inventory::DrawInterface()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawBar(iPoint pos, int current, int max, SDL_Color color)
{
	int size = 261;
	int thickness = 20;
	int percent = 1;

	if (current > 0 && max > 0)
	{
		percent = current / max;
	}

	if (color.r > 240) //red
	{
		app->render->DrawRectangle({ pos.x,pos.y,size * percent,thickness }, RED, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &healthBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	}
	else //blue
	{
		app->render->DrawRectangle({ pos.x,pos.y,size * percent,thickness }, BLUE, true, false);
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, 1, false, &expBarRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	}
}

void Inventory::UpdateButtons()
{
	littlebeefButton->Update(1.0f);
	bigBeefButton->Update(1.0f);
	featherButton->Update(1.0f);
	mantisButton->Update(1.0f);
	coinButton->Update(1.0f);
	splitButton->Update(1.0f);

	if (app->scene->player1->smallMeatCount == 0) littlebeefButton->state = GuiControlState::DISABLED;
	else littlebeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->largeMeatCount == 0) bigBeefButton->state = GuiControlState::DISABLED;
	else bigBeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->featherCount == 0) featherButton->state = GuiControlState::DISABLED;
	else featherButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->mantisRodCount == 0) mantisButton->state = GuiControlState::DISABLED;
	else mantisButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->moneyCount == 0) coinButton->state = GuiControlState::DISABLED;
	else coinButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->splitedEnemyCount == 0) splitButton->state = GuiControlState::DISABLED;
	else splitButton->state = GuiControlState::NORMAL;
}

void Inventory::DrawButtons()
{
	

	littlebeefButton->Draw(1, true, true, ButtonType::LITTLE_BEEF_B);
	bigBeefButton->Draw(1, true, true, ButtonType::BIG_BEEF_B);
	featherButton->Draw(1, true, true, ButtonType::FEATHER_B);
	mantisButton->Draw(1, true, true, ButtonType::MANTIS_B);
	coinButton->Draw(1, true, true, ButtonType::COIN_B);
	splitButton->Draw(1, true, true, ButtonType::SPLIT_B);
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

void Inventory::DrawFace()
{
	UpdateFace();

	currPlayerFaceAnim->Update(1.0f);

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y, 1.4f, false, &currPlayerFaceAnim->GetCurrentFrame(), 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);


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

void Inventory::DrawText()
{
	
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

	
}