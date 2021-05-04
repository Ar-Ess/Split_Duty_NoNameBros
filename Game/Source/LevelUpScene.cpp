#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "EntityManager.h"

#include "LevelUpScene.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"

#include "Log.h"

LevelUpScene::LevelUpScene()
{	
	interfaceTexture = app->tex->Load("Assets/Textures/UI/level-up.png");
}

void LevelUpScene::Start()
{
	SetButtons();
}

void LevelUpScene::Restart()
{
	app->tex->UnLoad(interfaceTexture);
}

void LevelUpScene::Update()
{
	upgradeHealthButton->Update(1.0f);
	upgradeAttackButton->Update(1.0f);
	upgradeDefenseButton->Update(1.0f);
	upgradeSpeedButton->Update(1.0f);
	upgradeLuckButton->Update(1.0f);
	upgradeStabButton->Update(1.0f);
}

void LevelUpScene::Draw()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	DrawButtons();

	app->guiManager->DrawCursor();
}

void LevelUpScene::SetButtons()
{
	
	app->scene->SetLevelUp(1);
}

void LevelUpScene::DrawButtons()
{


	SetButtons();

	/*void Combat::UpdateButtons()
	{
		Scene* s = app->scene;
		if (combatState != SECOND_PLAYER_TURN)
		{
			s->moveButton->Update(0.0f);
			s->itemButton->Update(0.0f);
		}

		s->attackButton->Update(0.0f);
		s->escapeButton->Update(0.0f);
		s->splitButton->Update(0.0f);

		if (combatState != PLAYER_TURN)
		{
			s->secondAttackButton->Update(0.0f);
			s->protectButton->Update(0.0f);
			s->buffButton->Update(0.0f);
		}
		s = nullptr;
	}*/
	upgradeHealthButton->Draw(1, true, false);
	upgradeDefenseButton->Draw(1, true, false);
	upgradeAttackButton->Draw(1, true, false);
	upgradeSpeedButton->Draw(1, true, false);
	upgradeLuckButton->Draw(1, true, false);
	upgradeStabButton->Draw(1, true, false);
}
