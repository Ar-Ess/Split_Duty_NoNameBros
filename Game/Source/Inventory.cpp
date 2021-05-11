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

#include "Log.h"

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
	itemsTexture = app->tex->Load("Assets/Textures/UI/items/items.png");
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
	app->tex->UnLoad(itemsTexture);
	app->tex->UnLoad(faceAnimationTexture);
	app->tex->UnLoad(statsTexture);
}

void Inventory::Update()
{
	quitButton->Update(1.0f);
}

void Inventory::Draw()
{
	DrawInterface();

	DrawItems();

	DrawFace();

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->exp, BLUE);

	//DrawStats();

	DrawButtons();

	DrawText();

	app->guiManager->DrawCursor();
	
}

void Inventory::DrawInterface()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawBar(iPoint pos,int current, int max, SDL_Color color)
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

void Inventory::DrawItems()
{
	if(app->scene->player1->playerInventory->littleBeef >=1) app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y,1,false, &littleBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	else app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y, 1, false, &mLittleBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	
	if (app->scene->player1->playerInventory->bigBeef >= 1)app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x ,1,false, itemPos.y, &bigBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	else app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x, 1, false, itemPos.y, &mBigBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	
	if (app->scene->player1->playerInventory->feather >= 1)	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + itemOff.y,1,false, &featherRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	else app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + itemOff.y, 1, false, &mFeatherRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	if (app->scene->player1->playerInventory->mantisLeg >= 1)app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + itemOff.y,1,false, &mantisRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	else app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x, itemPos.y + itemOff.y, 1, false, &mMantisRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	if (app->scene->player1->playerInventory->coins >= 1)app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + (2*itemOff.x),1,false, &coinRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	else app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + (2 * itemOff.y), 1, false, &mCoinRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	
	if (app->scene->player1->playerInventory->split >= 1)app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + (2 * itemOff.x),1,false, &splitRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x, itemPos.y + (2 * itemOff.y), 1, false, &mSplitRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawStats()
{
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y,1,false, &healthStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + statsOff.y, 1,false,&strenghtStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 2),1,false, &defenseStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 3),1,false, &velocityStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 4),1,false, &luckStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 5),1,false, &stabStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawButtons()
{
	quitButton->Draw(1, false, true);
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

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y,1.4f,false, &currPlayerFaceAnim->GetCurrentFrame(), 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	
}

void Inventory::SetText()
{
	//player
	if (lvlText == nullptr)
	{
		lvlText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		lvlText->bounds = { 541, 43, 200, 50 };
		lvlText->SetTextFont(app->fontTTF->defaultFont);
		lvlText->SetString("Current level ");
	}
	
	if (healthText == nullptr)
	{
		healthText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthText->bounds = { 900, 153, 200, 50 };
		healthText->SetTextFont(app->fontTTF->defaultFont);
		healthText->SetString("HP: 23 / 50");
	}
	
	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { 900, 220, 200, 50 };
		expText->SetTextFont(app->fontTTF->defaultFont);
		expText->SetString("XP: 34 / 70");
	}

	if (upgradeText == nullptr)
	{
		upgradeText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		upgradeText->bounds = { 900,100, 100, 100 };
		upgradeText->SetTextFont(app->fontTTF->defaultFont);
		upgradeText->SetString("14");
	}
	
	//ITEMS
	if (littleBeefText == nullptr)
	{
		littleBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		littleBeefText->bounds = { numberItemPos.x, numberItemPos.y, 100, 100 };
		littleBeefText->SetTextFont(app->fontTTF->defaultFont);
		littleBeefText->SetString("12");
	}
	if (bigBeefText == nullptr)
	{
		bigBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		bigBeefText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y, 100, 100 };
		bigBeefText->SetTextFont(app->fontTTF->defaultFont);
		bigBeefText->SetString("12");
	}
	if (featherText == nullptr)
	{
		featherText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		featherText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		featherText->SetTextFont(app->fontTTF->defaultFont);
		featherText->SetString("12");
	}
	if (mantisText == nullptr)
	{
		mantisText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		mantisText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		mantisText->SetTextFont(app->fontTTF->defaultFont);
		mantisText->SetString("12");
	}
	if (coinText == nullptr)
	{
		coinText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		coinText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y*2, 100, 100 };
		coinText->SetTextFont(app->fontTTF->defaultFont);
		coinText->SetString("12");
	}
	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		splitText->SetTextFont(app->fontTTF->defaultFont);
		splitText->SetString("12");
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

	questText->Draw();
}