#define _CRT_SECURE_NO_WARNINGS

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
	SetText();
}

void LevelUp::Restart()
{
	app->tex->UnLoad(interfaceTexture);
}

void LevelUp::Update()
{
	UpdateButtons();
	UpdateText();
}

void LevelUp::Draw()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	DrawButtons();

	DrawText();

	app->guiManager->DrawCursor();

	
}

void LevelUp::SetButtons()
{
	
}

void LevelUp::UpdateButtons()
{
	upgradeHealthButton->UpdateOnClick(1.0f);
	upgradeAttackButton->UpdateOnClick(1.0f);
	upgradeDefenseButton->UpdateOnClick(1.0f);
	upgradeSpeedButton->UpdateOnClick(1.0f);
	upgradeLuckButton->UpdateOnClick(1.0f);
	upgradeStabButton->UpdateOnClick(1.0f);
}

void LevelUp::SetText()
{
	if (upgradePointsText == nullptr)
	{
		upgradePointsText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		upgradePointsText->bounds = { 1100,200, 300, 100 };
		upgradePointsText->SetTextFont(app->fontTTF->inventoryFont);

		char str[5] = {};
		sprintf(str, "%d", app->scene->player1->featherCount);
		upgradePointsText->SetString(str, BROWN);
	}
}

void LevelUp::UpdateText()
{
	char str[5] = {};
	sprintf(str, "%d", app->scene->player1->featherCount);
	upgradePointsText->SetString(str, BROWN);
}

void LevelUp::DrawText()
{
	upgradePointsText->Draw();
}

void LevelUp::DrawButtons()
{
	upgradeHealthButton->Draw(1, true, false,ButtonType::UPGRADE);
	upgradeDefenseButton->Draw(1, true, false, UPGRADE);
	upgradeAttackButton->Draw(1, true, false, UPGRADE);
	upgradeSpeedButton->Draw(1, true, false, UPGRADE);
	upgradeLuckButton->Draw(1, true, false, UPGRADE);
	upgradeStabButton->Draw(1, true, false, UPGRADE);
}
