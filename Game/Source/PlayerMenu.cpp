#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "EntityManager.h"

#include "PlayerMenu.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"

#include "Log.h"

PlayerMenu::PlayerMenu()
{
	
}

void PlayerMenu::Start()
{
	playerLvl = app->scene->player1->lvl;

	playerFacePos = { 30,30 };
	healthBarPos = { 600,40 };
	expBarPos = { 600,80 };

	interfaceTexture = app->tex->Load("Assets/Textures/UI/player-menu.png");
	itemsTexture = app->tex->Load("Assets/Textures/UI/items/items.png");
	faceAnimationTexture = app->tex->Load("Assets/Textures/UI/face-animations.png");

	currPlayerFaceAnim = &idleFaceAnim;

}
void PlayerMenu::Restart()
{
	app->tex->UnLoad(interfaceTexture);
	app->tex->UnLoad(itemsTexture);
	app->tex->UnLoad(faceAnimationTexture);
}

void PlayerMenu::Update()
{
	if (active)
	{
		DrawInterface();
	}
	
}

void PlayerMenu::DrawInterface()
{
	
	app->render->DrawTexture(interfaceTexture, 0, 0, &fullScreen);

	DrawItems();

	DrawFace();

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->expTillNextLevel, BLUE);
}

void PlayerMenu::DrawBar(iPoint pos,int current, int max, SDL_Color color)
{
	int size = 100;
	int thickness = 20;

	int percent = current / max;

	app->render->DrawRectangle({pos.x , pos.y , size * percent ,thickness }, MAGENTA);
	
	if (color.r > 240) //red
	{
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, &healthBar);
	}
	else //blue
	{
		app->render->DrawTexture(interfaceTexture, pos.x, pos.y, &expBar);
	}
}

void PlayerMenu::DrawItems()
{
	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y, &littleBeefRect);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y, &bigBeefRect);

	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + itemOff.y, &featherRect);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + itemOff.x, &mantisRect);

	app->render->DrawTexture(itemsTexture, itemPos.x, itemPos.y + (2*itemOff.x), &coinRect);

	app->render->DrawTexture(itemsTexture, itemPos.x + itemOff.x , itemPos.y + (2 * itemOff.x), &splitRect);

}

void PlayerMenu::DrawFace()
{
	currPlayerFaceAnim->Update(1.0f);

	app->render->DrawTexture(faceAnimationTexture, playerFacePos.x, playerFacePos.y, &currPlayerFaceAnim->GetCurrentFrame());
}

void PlayerMenu::DrawText()
{
}




