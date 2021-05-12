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


	//texts
	SetText();


}

void Inventory::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(faceAnimationTexture);
	app->tex->UnLoad(statsTexture);

}

void Inventory::Update()
{
	quitQuestButton->Update(1.0f);

	exitInventoryButton->Update(1.0f);

	bigBeefButton->UpdateOnClick(1.0f);
	littlebeefButton->UpdateOnClick(1.0f);
	featherButton->UpdateOnClick(1.0f);
	mantisButton->UpdateOnClick(1.0f);
	coinButton->UpdateOnClick(1.0f);
	splitButton->UpdateOnClick(1.0f);
}

void Inventory::UseItems()
{
}

void Inventory::Draw()
{
	DrawInterface();

	DrawFace();

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->exp, BLUE);

	//DrawStats();

	UpdateButtons();

	DrawButtons();

	UpdateText();

	DrawText();

	app->guiManager->DrawCursor();

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

void Inventory::SetButtons()
{
	if (app->scene->player1->playerInventory->littleBeef == 0) littlebeefButton->state = GuiControlState::DISABLED;
	else littlebeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->bigBeef == 0) bigBeefButton->state = GuiControlState::DISABLED;
	else bigBeefButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->feather == 0) featherButton->state = GuiControlState::DISABLED;
	else featherButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->mantisLeg == 0) mantisButton->state = GuiControlState::DISABLED;
	else mantisButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->coins == 0) coinButton->state = GuiControlState::DISABLED;
	else coinButton->state = GuiControlState::NORMAL;
	if (app->scene->player1->playerInventory->split == 0) splitButton->state = GuiControlState::DISABLED;
	else splitButton->state = GuiControlState::NORMAL;

}

void Inventory::UpdateButtons()
{
	if (app->scene->player1->playerInventory->littleBeef == 0) littlebeefButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->bigBeef == 0) bigBeefButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->feather == 0) featherButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->mantisLeg == 0) mantisButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->coins == 0) coinButton->state = GuiControlState::DISABLED;

	if (app->scene->player1->playerInventory->split == 0) splitButton->state = GuiControlState::DISABLED;

}

void Inventory::SetInventoryValues()
{
}





void Inventory::DrawButtons()
{
	quitQuestButton->Draw(1, true, true, ButtonType::QUEST);

	exitInventoryButton->Draw(1, true, true, ButtonType::INVENTORY);

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
		lvlText->bounds = { 541, 43, 200, 50 };
		lvlText->SetTextFont(app->fontTTF->inventoryFont);
		lvlText->SetString("Current level ", YELLOW);
	}

	if (healthText == nullptr)
	{
		healthText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthText->bounds = { 900, 153, 200, 50 };
		healthText->SetTextFont(app->fontTTF->inventoryFont);
		
		/*char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->health, 2);
		
		littleBeefText->SetString(str, BROWN);*/
	}

	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { 900, 220, 200, 50 };
		expText->SetTextFont(app->fontTTF->inventoryFont);
		expText->SetString("XP: 34 / 70", YELLOW);
	}

	if (upgradeText == nullptr)
	{
		upgradeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		upgradeText->bounds = { 900,100, 100, 100 };
		upgradeText->SetTextFont(app->fontTTF->inventoryFont);
		upgradeText->SetString("14");
	}

	//ITEMS
	if (littleBeefText == nullptr)
	{
		littleBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		littleBeefText->bounds = { numberItemPos.x, numberItemPos.y, 100, 100 };
		littleBeefText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->littleBeef, 2);
		littleBeefText->SetString(str, BROWN);
	}
	if (bigBeefText == nullptr)
	{
		bigBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		bigBeefText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y, 100, 100 };
		bigBeefText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->bigBeef, 2);
		bigBeefText->SetString(str, BROWN);
	}
	if (featherText == nullptr)
	{
		featherText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		featherText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		featherText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->feather, 2);
		featherText->SetString(str, BROWN);
	}
	if (mantisText == nullptr)
	{
		mantisText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		mantisText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		mantisText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->mantisLeg, 2);
		mantisText->SetString(str, BROWN);
	}
	if (coinText == nullptr)
	{
		coinText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		coinText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		coinText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->coins, 2);
		coinText->SetString(str, BROWN);
	}
	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		splitText->SetTextFont(app->fontTTF->inventoryFont);

		char str[2] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->split, 2);
		splitText->SetString(str, BROWN);
	}

	//Stats
	if (healthStatText == nullptr)
	{
		healthStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthStatText->bounds = { statsPos.x,statsPos.y,100,100 };
		healthStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->healthStat, 2);
		healthStatText->SetString("12", BROWN);
	}
	if (strenghtStatText == nullptr)
	{
		strenghtStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strenghtStatText->bounds = { statsPos.x,statsPos.y + statsOff.y,100,100 };
		strenghtStatText->SetTextFont(app->fontTTF->inventoryFont);
		
		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->strengthStat, 2);
		strenghtStatText->SetString(str, BROWN);
	}
	if (defenseStatText == nullptr)
	{
		defenseStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		defenseStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 2,100,100 };
		defenseStatText->SetTextFont(app->fontTTF->inventoryFont);

		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->defenseStat, 2);
		defenseStatText->SetString(str, BROWN);
	}
	if (velocityStatText == nullptr)
	{
		velocityStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		velocityStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 3,100,100 };
		velocityStatText->SetTextFont(app->fontTTF->inventoryFont);
		
		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->velocityStat, 2);
		velocityStatText->SetString(str, BROWN);
	}
	if (luckStatText == nullptr)
	{
		luckStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		luckStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 4,100,100 };
		luckStatText->SetTextFont(app->fontTTF->inventoryFont);
		
		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->luckStat, 2);
		luckStatText->SetString(str, BROWN);
	}
	if (stabStatText == nullptr)
	{
		stabStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		stabStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,100,100 };
		stabStatText->SetTextFont(app->fontTTF->inventoryFont);
		
		char str[12] = { "" };
		app->fontTTF->IntToDynamicString(str, app->scene->player1->stabStat, 2);
		stabStatText->SetString(str, BROWN);
	}

	//quest
	if (questText == nullptr)
	{
		questText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		questText->bounds = { 60,600, 100, 100 };
		questText->SetTextFont(app->fontTTF->defaultFont);
		questText->SetString("Dale ostias a un lobo");
	}
}

void Inventory::UpdateText()
{

	char str[2] = { "" };

	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->littleBeef, 2);
	littleBeefText->SetString(str, BROWN);

	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->bigBeef, 2);
	bigBeefText->SetString(str, BROWN);


	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->feather, 2);
	featherText->SetString(str, BROWN);

	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->mantisLeg, 2);
	mantisText->SetString(str, BROWN);


	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->coins, 2);
	coinText->SetString(str, BROWN);


	app->fontTTF->IntToDynamicString(str, app->scene->player1->playerInventory->split, 2);
	splitText->SetString(str, BROWN);

}

void Inventory::DrawText()
{
	lvlText->Draw();
	healthText->Draw();
	expText->Draw();
	upgradeText->Draw();

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

	questText->Draw();
}