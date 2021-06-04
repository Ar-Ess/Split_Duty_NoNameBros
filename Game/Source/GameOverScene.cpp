#include "GameOverScene.h"
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

#include "Textures.h"
#include "Render.h"

#include "Log.h"

GameOverScene::GameOverScene()
{
}

void GameOverScene::Start()
{
	background = app->tex->Load("Assets/Screens/end_screen.png");

	if (backToMenuText == nullptr)
	{
		backToMenuText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		backToMenuText->bounds = backToMenuButton->bounds;
		backToMenuText->SetTextFont(app->fontTTF->defaultFont);
		backToMenuText->SetString("MENU");
		backToMenuText->CenterAlign();
	}

	if (continueText == nullptr)
	{
		continueText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
		continueText->bounds = continueButton->bounds;
		continueText->SetTextFont(app->fontTTF->defaultFont);
		continueText->SetString("CONTINUE");
		continueText->CenterAlign();
	}
}

void GameOverScene::Restart()
{
	app->tex->UnLoad(background);
}

void GameOverScene::Update()
{
	backToMenuButton->Update(1.0f);

	continueButton->Update(1.0f);
}

void GameOverScene::Draw(int y)
{
	app->render->DrawTexture(background, 0, y);

	backToMenuButton->bounds.y += y;
	continueButton->bounds.y += y;
	backToMenuText->bounds.y += y;
	continueText->bounds.y += y;

	backToMenuButton->Draw();
	continueButton->Draw();
	backToMenuText->Draw();
	continueText->Draw();

	backToMenuButton->bounds.y -= y;
	continueButton->bounds.y -= y;
	backToMenuText->bounds.y -= y;
	continueText->bounds.y -= y;
}
