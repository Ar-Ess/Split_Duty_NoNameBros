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
	quitQuestButton->Update(1.0f);

	exitInventoryButton->Update(1.0f);

}

void Inventory::UseItems()
{
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



void Inventory::DrawButtons()
{
	quitQuestButton->Draw(1, true, true,ButtonType::QUEST);

	exitInventoryButton->Draw(1,true,true,ButtonType::INVENTORY);
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
		lvlText->SetTextFont(app->fontTTF->inventoryFont);
		lvlText->SetString("Current level ",YELLOW);
	}
	
	if (healthText == nullptr)
	{
		healthText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthText->bounds = { 900, 153, 200, 50 };
		healthText->SetTextFont(app->fontTTF->inventoryFont);
		healthText->SetString("HP: 23 / 50",  YELLOW );
	}
	
	if (expText == nullptr)
	{
		expText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		expText->bounds = { 900, 220, 200, 50 };
		expText->SetTextFont(app->fontTTF->inventoryFont);
		expText->SetString("XP: 34 / 70",YELLOW);
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
		littleBeefText->SetString("12",BROWN);
	}
	if (bigBeefText == nullptr)
	{
		bigBeefText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		bigBeefText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y, 100, 100 };
		bigBeefText->SetTextFont(app->fontTTF->inventoryFont);
		bigBeefText->SetString("12",BROWN);
	}
	if (featherText == nullptr)
	{
		featherText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		featherText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		featherText->SetTextFont(app->fontTTF->inventoryFont);
		featherText->SetString("12",BROWN);
	}
	if (mantisText == nullptr)
	{
		mantisText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		mantisText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y, 100, 100 };
		mantisText->SetTextFont(app->fontTTF->inventoryFont);
		mantisText->SetString("12",BROWN);
	}
	if (coinText == nullptr)
	{
		coinText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		coinText->bounds = { numberItemPos.x, numberItemPos.y + numberItemOff.y*2, 100, 100 };
		coinText->SetTextFont(app->fontTTF->inventoryFont);
		coinText->SetString("12",BROWN);
	}
	if (splitText == nullptr)
	{
		splitText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		splitText->bounds = { numberItemPos.x + numberItemOff.x, numberItemPos.y + numberItemOff.y * 2, 100, 100 };
		splitText->SetTextFont(app->fontTTF->inventoryFont);
		splitText->SetString("12",BROWN);
	}
	//Stats
	if (healthStatText == nullptr)
	{
		healthStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		healthStatText->bounds = {statsPos.x,statsPos.y,100,100};
		healthStatText->SetTextFont(app->fontTTF->inventoryFont);
		healthStatText->SetString("12",BROWN);
	}
	if (strenghtStatText == nullptr)
	{
		strenghtStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		strenghtStatText->bounds = { statsPos.x,statsPos.y + statsOff.y,100,100 };
		strenghtStatText->SetTextFont(app->fontTTF->inventoryFont);
		strenghtStatText->SetString("12", BROWN);
	}
	if (defenseStatText == nullptr)
	{
		defenseStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		defenseStatText->bounds = { statsPos.x,statsPos.y + statsOff.y*2,100,100 };
		defenseStatText->SetTextFont(app->fontTTF->inventoryFont);
		defenseStatText->SetString("12", BROWN);
	}
	if (velocityStatText == nullptr)
	{
		velocityStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		velocityStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 3,100,100 };
		velocityStatText->SetTextFont(app->fontTTF->inventoryFont);
		velocityStatText->SetString("12", BROWN);
	}
	if (luckStatText == nullptr)
	{
		luckStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		luckStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 4,100,100 };
		luckStatText->SetTextFont(app->fontTTF->inventoryFont);
		luckStatText->SetString("12", BROWN);
	}
	if (stabStatText == nullptr)
	{
		stabStatText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		stabStatText->bounds = { statsPos.x,statsPos.y + statsOff.y * 5,100,100 };
		stabStatText->SetTextFont(app->fontTTF->inventoryFont);
		stabStatText->SetString("12", BROWN);
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

	healthStatText->Draw();
	strenghtStatText->Draw();
	defenseStatText->Draw();
	velocityStatText->Draw();
	stabStatText->Draw();
	luckStatText->Draw();

	questText->Draw();
}