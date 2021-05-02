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
}

void LevelUp::Draw()
{
	app->render->DrawTexture(interfaceTexture, 0, 0, 1, false, 0, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);

	DrawButtons();
}

void LevelUp::UpdateButtons()
{
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
}

void LevelUp::DrawButtons()
{
	UpdateButtons();

	upgradeHealthButton->Draw(1, true, false);
	upgradeDefenseButton->Draw(1, true, false);
	upgradeAttackButton->Draw(1, true, false);
	upgradeSpeedButton->Draw(1, true, false);
	upgradeLuckButton->Draw(1, true, false);
	upgradeStabButton->Draw(1, true, false);
}
