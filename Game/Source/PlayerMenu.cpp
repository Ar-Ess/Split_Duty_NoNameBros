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
}
void PlayerMenu::Restart()
{
	app->tex->UnLoad(interfaceTexture);
}

void PlayerMenu::Update()
{
}

void PlayerMenu::DrawInterface()
{

	DrawBar(healthBarPos, app->scene->player1->health, app->scene->player1->maxHealth, RED);
	DrawBar(expBarPos, app->scene->player1->exp, app->scene->player1->expTillNextLevel, BLUE);
}

void PlayerMenu::DrawBar(iPoint pos,int current, int max, SDL_Color color)
{
	int size = 100;
	int thickness = 20;
	
	app->render->DrawRectangle({pos.x , pos.y , size,thickness }, MAGENTA);
	
}

void PlayerMenu::DrawItems()
{
}

void PlayerMenu::DrawText()
{
}




