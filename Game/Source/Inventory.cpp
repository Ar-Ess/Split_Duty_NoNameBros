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
	hiddenStats = app->tex->Load("Assets/Textures/UI/stats.png");

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
	Player* p = app->scene->player1;

	SetText();

	char str[5] = {};
	sprintf(str, "%d", p->lvl);
	lvlText->SetString(str, YELLOW);

	//STATS TEXT
	char str1[5] = {};
	sprintf(str1, "%d", p->maxHealth);
	healthStatText->SetString(str1, BROWN);
	if (p->maxHealth > 9) healthStatText->bounds.x -= 8;

	char str2[5] = {};
	sprintf(str2, "%d", p->strengthStat);
	strenghtStatText->SetString(str2, BROWN);
	if (p->strengthStat > 9) strenghtStatText->bounds.x -= 8;

	char str3[5] = {};
	sprintf(str3, "%d", p->defenseStat);
	defenseStatText->SetString(str3, BROWN);
	if (p->defenseStat > 9) defenseStatText->bounds.x -= 8;

	char str4[5] = {};
	sprintf(str4, "%d", p->velocityStat);
	velocityStatText->SetString(str4, BROWN);
	if (p->velocityStat > 9) velocityStatText->bounds.x -= 8;

	char str5[5] = {};
	sprintf(str5, "%d", p->stabStat);
	stabStatText->SetString(str5, BROWN);
	if (p->stabStat > 9) stabStatText->bounds.x -= 8;

	char str6[5] = {};
	sprintf(str6, "%d", p->luckStat);
	luckStatText->SetString(str6, BROWN);
	if (p->luckStat > 9) luckStatText->bounds.x -= 8;

	//HEALTH 
	char hpText[20] = {};
	sprintf(hpText, "%d / %d", p->health, p->maxHealth);
	healthText->SetString(hpText, YELLOW);

	//XP
	char xpText[20] = {};
	maxExp = int(floor(1000 * pow((float)p->lvl, 1.25f)));
	sprintf(xpText, "%d / %d", p->exp, maxExp);
	expText->SetString(xpText, YELLOW);

	p = nullptr;
		
}

void Inventory::Restart()
{
	app->scene->iterations = 0;

	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(faceAnimationTexture);
	app->tex->UnLoad(hiddenStats);

	blinkFaceAnim.Reset();
	angryFaceAnim.Reset();
	idleFaceAnim.Reset();

	Player* p = app->scene->player1;
	if (p->luckStat > 9) if (luckStatText != nullptr) luckStatText->bounds.x += 8;
	if (p->stabStat > 9) if (stabStatText != nullptr) stabStatText->bounds.x += 8;
	if (p->velocityStat > 9) if (velocityStatText != nullptr) velocityStatText->bounds.x += 8;
	if (p->defenseStat > 9) if (defenseStatText != nullptr) defenseStatText->bounds.x += 8;
	if (p->maxHealth > 9) if (healthStatText != nullptr) healthStatText->bounds.x += 8;
	if (p->strengthStat > 9) if (strenghtStatText != nullptr) strenghtStatText->bounds.x += 8;
	p = nullptr;
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

	DrawStats(y);

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

	//active quest
	if (currQuestText == nullptr)
	{
		currQuestText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		currQuestText->bounds = { 75, 600,50,50 };
		currQuestText->SetTextFont(app->fontTTF->inventoryFont);

		currQuestText->SetString("NO QUESTS ACTIVE");
	}	
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
	//QUEST
	char str6[50] = {};
	if (app->questManager->currentQuest != nullptr)
	{
		KillQuest* q = nullptr;
		GatherQuest* q1 = nullptr;
		FindQuest* q2 = nullptr;
		switch (app->questManager->currentQuest->type)
		{
		case QuestType::KILL:
			q = (KillQuest*)app->questManager->currentQuest;
			sprintf(str6, "%s: %d / %d", q->title->text.GetString(), q->count, q->goal);
			currQuestText->SetString(str6, WHITE);
			break;
		case QuestType::GATHER:
			q1 = (GatherQuest*)app->questManager->currentQuest;
			switch (q1->iType)
			{
			case LITTLE_BEEF_I:
				sprintf(str6, "%s: %d OF LITTLE BEEF", q1->title->text.GetString(), q1->goal);
				break;
			case BIF_BEEF_I:
				sprintf(str6, "%s: %d OF BIG BEEF", q1->title->text.GetString(), q1->goal);
				break;
			case FEATHER_I:
				sprintf(str6, "%s: %d OF FEATHER", q1->title->text.GetString(), q1->goal);
				break;
			case MANTIS_I:
				sprintf(str6, "%s: %d OF MANTIS LEG", q1->title->text.GetString(), q1->goal);
				break;
			}
			currQuestText->SetString(str6, WHITE);
			break;
		case QuestType::FIND:
			q2 = (FindQuest*)app->questManager->currentQuest;
			sprintf(str6, "%s", q2->title->text.GetString());
			currQuestText->SetString(str6, WHITE);
			break;
		}
	}
	else			
		currQuestText->SetString("NO MISSION ACTIVE", WHITE);
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
	currQuestText->bounds.y += y;

	healthText->Draw();
	expText->Draw();
	lvlText->Draw();

	littleBeefText->Draw();
	bigBeefText->Draw();
	featherText->Draw();
	mantisText->Draw();
	coinText->Draw();
	splitText->Draw();

	currQuestText->Draw();

	healthStatText->Draw();
	strenghtStatText->Draw();
	defenseStatText->Draw();
	if (app->scene->player1->velocityStat > 0) velocityStatText->Draw();
	if (app->scene->player1->stabStat > 0) stabStatText->Draw();
	if (app->scene->player1->luckStat > 0) luckStatText->Draw();

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
	currQuestText->bounds.y -= y;

}

void Inventory::DrawStats(int y)
{
	if (app->scene->player1->velocityStat > 0) app->render->DrawTexture(hiddenStats, 846, 455 + y, 1, false, &velocity, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	if (app->scene->player1->luckStat > 0) app->render->DrawTexture(hiddenStats, 846, 523 + y, 1, false, &luck, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	if (app->scene->player1->stabStat > 0) app->render->DrawTexture(hiddenStats, 846, 591 + y, 1, false, &stab, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::UpdateHealthText()
{
	char hpText[20] = {};
	sprintf(hpText, "%d / %d", app->scene->player1->health, app->scene->player1->maxHealth);
	healthText->SetString(hpText, YELLOW);
}
