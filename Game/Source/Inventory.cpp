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

	playerFacePos = { 30,30 };
	healthBarPos = { 600,40 };
	expBarPos = { 600,80 };

	playerHp = app->scene->player1->health;
	playerExp = app->scene->player1->exp;

	interfaceTexture = app->tex->Load("Assets/Textures/UI/inventory.png");
	itemsTexture = app->tex->Load("Assets/Textures/UI/items/items.png");
	faceAnimationTexture = app->tex->Load("Assets/Textures/UI/face-animations.png");
	statsTexture = app->tex->Load("Assets/Textures/UI/stats.png");

	idleFaceAnim.PushBack({ 0, 0, 70, 68 });
	idleFaceAnim.PushBack({ 70, 0, 70, 68 });

	currPlayerFaceAnim = &idleFaceAnim;

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
	
}

void Inventory::Draw()
{
	DrawInterface();

	DrawItems();

	DrawFace();

	
	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->exp, BLUE);

	DrawStats();

	DrawText();
}

void Inventory::DrawInterface()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawBar(iPoint pos,int current, int max, SDL_Color color)
{
	int size = 100;
	int thickness = 20;
	int percent = 1;

	if ((current && max) > 0)
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
	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y,1,false, &littleBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x ,1,false, itemPos.y, &bigBeefRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + itemOff.y,1,false, &featherRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + itemOff.x,1,false, &mantisRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + (2*itemOff.x),1,false, &coinRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + (2 * itemOff.x),1,false, &splitRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

}

void Inventory::DrawStats()
{
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y,1,false, &healthStatRect);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + statsOff.y, 1,false,&strenghtStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 2),1,false, &defenseStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 3),1,false, &velocityStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 4),1,false, &luckStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
	app->render->DrawTexture(statsTexture, statsPos.x, statsPos.y + (statsOff.y * 5),1,false, &stabStatRect, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
}

void Inventory::DrawFace()
{
	currPlayerFaceAnim->Update(1.0f);

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y, &currPlayerFaceAnim->GetCurrentFrame());

	
}

void Inventory::DrawText()
{
}