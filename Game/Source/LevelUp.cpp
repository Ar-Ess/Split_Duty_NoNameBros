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
	interfaceTexture = app->tex->Load("Assets/Textures/UI/level-up.png");
}

void LevelUp::Start()
{

}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
}

void LevelUp::Update()
{
	upgradeHealthButton->UpdateOnClick(1.0f);
	upgradeAttackButton->UpdateOnClick(1.0f);
	upgradeDefenseButton->UpdateOnClick(1.0f);
	upgradeSpeedButton->UpdateOnClick(1.0f);
	upgradeLuckButton->UpdateOnClick(1.0f);
	upgradeStabButton->UpdateOnClick(1.0f);
}

void LevelUp::Draw()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	DrawButtons();

	app->guiManager->DrawCursor();
}

void LevelUp::DrawButtons()
{
	upgradeHealthButton->Draw(1, true, false);
	upgradeDefenseButton->Draw(1, true, false);
	upgradeAttackButton->Draw(1, true, false);
	upgradeSpeedButton->Draw(1, true, false);
	upgradeLuckButton->Draw(1, true, false);
	upgradeStabButton->Draw(1, true, false);
}
