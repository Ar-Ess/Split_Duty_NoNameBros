#include "App.h"
#include "Textures.h"

#include "GuiManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Combat.h"
#include "OptionsMenu.h"
#include "LevelUp.h"
#include "GameOverScene.h"
#include "World.h"
#include "Inventory.h"

#include "Log.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiString.h"
#include "FontTTF.h"

GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON: 
		control = new GuiButton(buttonId, { 0, 0, 0, 0 }, "0"); 
		buttonId++;
		break;
	case GuiControlType::CHECKBOX: control = new GuiCheckBox(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::SLIDER: control = new GuiSlider(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::TEXT: control = new GuiString(); break;
	default: break;
	}

	if (control != nullptr)
	{
		controls.Add(control);
		switch (type)
		{
		case GuiControlType::BUTTON: buttons.Add(control); break;
		default: break;
		}
	}

	

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int id = controls.Find(entity);
	controls.Del(controls.At(id));
}

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

bool GuiManager::Start()
{
	secondsCounter = 0;
	frameCounter = 0;

	buttonSpriteSheet = nullptr;
	checkBoxSpriteSheet = nullptr;
	sliderSpriteSheet = nullptr;

	cursorTexture = app->tex->Load("Assets/Textures/UI/grab_hand.png");
	GuiTexture = app->tex->Load("Assets/Textures/UI/GUI.png");
	faceAnimationsTexture = app->tex->Load("Assets/Textures/UI/face_animations.png");
	itemsTexture = app->tex->Load("Assets/Textures/UI/Items/Items.png");
	

	idleCursorAnim.PushBack({ 0,0,30,30});

	//clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 64,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.loop = false;
	clickCursorAnim.speed = 0.5f;

	currentCursorAnim = &idleCursorAnim;

	//Buttons
	

	//face

	return true;
}

bool GuiManager::Update(float dt)
{
	frameCounter++;
	if (frameCounter % 25 == 0)
		secondsCounter++;

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	Scene* s = app->scene;
	if (s->GetCurrScene() != Scenes::LOGO_SCENE) SelectButtonsLogic();

	return true;
}

void GuiManager::CenterCursor()
{
	
}

void GuiManager::DrawCursor()
{
	currentCursorAnim->Update(1.0f);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	app->render->DrawTexture(cursorTexture, mouseX, mouseY, &currentCursorAnim->GetCurrentFrame());

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if (currentCursorAnim != &clickCursorAnim)
		{
			currentCursorAnim = &clickCursorAnim;
			
		}
	}

	if (currentCursorAnim->HasFinished() && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP )
	{
		currentCursorAnim = &idleCursorAnim;
		clickCursorAnim.Reset();
		
		
	}
}

void GuiManager::DrawPlayerLifeBar(int life,int maxLife,int x,int y)
{
	int size = 4;
	int thickness = 20;
	maxLifeBar = { x,y,maxLife * size,thickness };

	if (!app->scene->player1->godMode)
	{
		
		app->render->DrawRectangle(maxLifeBar, MAGENTA);
		if (life > 0)
		{
			lifeBar = { x,y,life * size,thickness };

			//if life is critical blinks
			if (life <= maxLife / 3)
			{
				BlinkLifeBar(life, RED, SOFT_RED);
			}
			else
				app->render->DrawRectangle(lifeBar, RED);
		}
		
	}
	else if (app->scene->player1->godMode)
	{
		LOG("god mode life bar");
		app->render->DrawRectangle(maxLifeBar, BLUE);
		//BlinkLifeBar(maxLife, BLUE, CYAN);
	}
}

void GuiManager::DrawEnemyLifeBar(int life, int maxLife, int x, int y)
{
	int size = 4;
	int thickness = 20;
	int offset = maxLife * size;
	app->render->DrawRectangle({x - offset,y,maxLife*size,thickness }, MAGENTA);
	lifeBar = { x-life*4,y,life * size,thickness };
	if (life > 0)
	{
		//if life is critical blinks
		if (life <= maxLife / 3)
		{
			BlinkLifeBar(life, RED, SOFT_RED);
		}
		else
			app->render->DrawRectangle(lifeBar, RED);
	}
}

void GuiManager::BlinkLifeBar(int life, SDL_Color color1, SDL_Color color2)
{
	
	if(secondsCounter % 2 ==0)
		app->render->DrawRectangle(lifeBar, color1);
	else
		app->render->DrawRectangle(lifeBar, color2);
}

void GuiManager::DrawCombatInterface(Enemy* enemy)
{
	const SDL_Rect guiRect = { 0,0,1280,720 };
	app->render->DrawTexture(GuiTexture, 0, 0, &guiRect);

	//const SDL_Rect faceRect = { 0,0,70,69 };
	//app->render->DrawTexture(faceAnimationsTexture, 71, 27, &faceRect);

	app->guiManager->DrawPlayerLifeBar(app->scene->player1->health, app->scene->player1->maxHealth, 182, 30);

	if (app->scene->combatScene->GetSecondPlayerExistance()) app->guiManager->DrawPlayerLifeBar(app->scene->player2->health, app->scene->player2->maxHealth, 182, 80);

	DrawEnemyLifeBar(enemy->health, enemy->maxHealth, 1080, 30);
}

void GuiManager::DrawCombatInterface(Boss* boss)
{
	const SDL_Rect guiRect = { 0,0,1280,720 };
	app->render->DrawTexture(GuiTexture, 0, 0, &guiRect);

	//const SDL_Rect faceRect = { 0,0,70,69 };
	//app->render->DrawTexture(faceAnimationsTexture, 71, 27, &faceRect);

	app->guiManager->DrawPlayerLifeBar(app->scene->player1->health, app->scene->player1->maxHealth, 182, 30);

	if (app->scene->combatScene->GetSecondPlayerExistance()) app->guiManager->DrawPlayerLifeBar(app->scene->player2->health, app->scene->player2->maxHealth, 182, 80);

	DrawEnemyLifeBar(boss->health, boss->maxHealth, 1080, 30);
}

void GuiManager::DisableAllButtons()
{
	Scene* s = app->scene;

	switch (s->GetCurrScene())
	{
	case MAIN_MENU:
		s->newGameButton->buttonFocus = false;
		s->continueButton->buttonFocus = false;
		s->optionsButton->buttonFocus = false;
		s->exitButton->buttonFocus = false;
		break;

	case PAUSE_MENU:
		s->backToGameButton->buttonFocus = false;
		s->saveGameButton->buttonFocus = false;
		s->optionsPauseButton->buttonFocus = false;
		s->backToMenuButton->buttonFocus = false;
		break;

	case OPTIONS_MENU:
		s->optionsMenu->fullScreenCheckBox->checkBoxFocus = false;
		s->optionsMenu->dFullScreenCheckBox->checkBoxFocus = false;
		s->optionsMenu->vSyncCheckBox->checkBoxFocus = false;
		s->optionsMenu->musicVolumeSlider->sliderFocus = false;
		s->optionsMenu->fxVolumeSlider->sliderFocus = false;
		s->optionsMenu->returnMenuButton->buttonFocus = false;
		break;

	case LEVEL_UP:
		s->levelUpScene->skipButton->buttonFocus = false;
		break;

	case END_SCREEN:
		s->endScene->backToMenuButton->buttonFocus = false;
		break;

	case COMBAT:
		s->attackButton->buttonFocus = false;
		s->moveButton->buttonFocus = false;
		s->itemButton->buttonFocus = false;
		s->splitButton->buttonFocus = false;
		s->escapeButton->buttonFocus = false;
		s->secondAttackButton->buttonFocus = false;
		s->protectButton->buttonFocus = false;
		s->buffButton->buttonFocus = false;
		s->combatScene->smallMeatButton->buttonFocus = false;
		s->combatScene->largeMeatButton->buttonFocus = false;
		s->combatScene->featherButton->buttonFocus = false;
		s->combatScene->mantisButton->buttonFocus = false;
		s->combatScene->enemySplitButton->buttonFocus = false;
		s->combatScene->moneyButton->buttonFocus = false;
		break;

	case WORLD:
		s->world->inventory->littleBeefButton->buttonFocus = false;
		s->world->inventory->bigBeefButton->buttonFocus = false;
		s->world->inventory->featherButton->buttonFocus = false;
		s->world->inventory->mantisButton->buttonFocus = false;
		s->world->inventory->coinButton->buttonFocus = false;
		s->world->inventory->splitButton->buttonFocus = false;
		break;
	}

	s = nullptr;
}

void GuiManager::SelectButtonsLogic()
{
	Scene* s = app->scene;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->GetControl(L3) == KEY_DOWN)
	{
		idSelection++;
		DisableAllButtons();
	}

	switch (s->GetCurrScene())
	{
	case MAIN_MENU:
		if (idSelection == 0) s->newGameButton->buttonFocus = true;
		else if (idSelection == 1) s->continueButton->buttonFocus = true;
		else if (idSelection == 2) s->optionsButton->buttonFocus = true;
		else if (idSelection == 3) s->exitButton->buttonFocus = true;
		else if (idSelection == 4) idSelection = -1;
		break;

	case PAUSE_MENU:
		if (idSelection == 0) s->backToGameButton->buttonFocus = true;
		else if (idSelection == 1) s->saveGameButton->buttonFocus = true;
		else if (idSelection == 2) s->optionsPauseButton->buttonFocus = true;
		else if (idSelection == 3) s->backToMenuButton->buttonFocus = true;
		else if (idSelection == 4) idSelection = -1;
		break;

	case OPTIONS_MENU:
		if (idSelection == 0) s->optionsMenu->dFullScreenCheckBox->checkBoxFocus = true;
		else if (idSelection == 1) s->optionsMenu->fullScreenCheckBox->checkBoxFocus = true;
		else if (idSelection == 2) s->optionsMenu->vSyncCheckBox->checkBoxFocus = true;
		else if (idSelection == 3) s->optionsMenu->musicVolumeSlider->sliderFocus = true;
		else if (idSelection == 4) s->optionsMenu->fxVolumeSlider->sliderFocus = true;
		else if (idSelection == 5) s->optionsMenu->returnMenuButton->buttonFocus = true;
		else if (idSelection == 6) idSelection = -1;
		break;

	case LEVEL_UP:
		if (idSelection == 0) s->levelUpScene->skipButton->buttonFocus = true;
		else if (idSelection == 1) idSelection = -1;
		break;

	case END_SCREEN:
		if (idSelection == 0) s->endScene->backToMenuButton->buttonFocus = true;
		else if (idSelection == 1) idSelection = -1;
		break;

	case COMBAT:
		if (!s->combatScene->drawInventory)
		{
			if (s->combatScene->GetTurn() == CombatState::PLAYER_TURN)
			{
				if (idSelection == 0) s->attackButton->buttonFocus = true;
				else if (idSelection == 1) s->moveButton->buttonFocus = true;
				else if (idSelection == 2) s->itemButton->buttonFocus = true;
				else if (idSelection == 3) s->escapeButton->buttonFocus = true;
				else if (idSelection == 4) s->splitButton->buttonFocus = true;
				else if (idSelection == 5) idSelection = -1;
			}
			else if (s->combatScene->GetTurn() == CombatState::SECOND_PLAYER_TURN)
			{
				if (idSelection == 0) s->secondAttackButton->buttonFocus = true;
				else if (idSelection == 1) s->protectButton->buttonFocus = true;
				else if (idSelection == 2) s->buffButton->buttonFocus = true;
				else if (idSelection == 3) idSelection = -1;
			}
		}
		else
		{
			if (idSelection == 0) s->combatScene->smallMeatButton->buttonFocus = true;
			else if (idSelection == 1) s->combatScene->largeMeatButton->buttonFocus = true;
			else if (idSelection == 2) s->combatScene->featherButton->buttonFocus = true;
			else if (idSelection == 3) s->combatScene->mantisButton->buttonFocus = true;
			else if (idSelection == 4) s->combatScene->enemySplitButton->buttonFocus = true;
			else if (idSelection == 5) s->combatScene->moneyButton->buttonFocus = true;     
			else if (idSelection == 6) idSelection = -1;
		}
		break;

	case WORLD:
		if (s->world->inventoryOpen)
		{
			if (idSelection == 0) s->world->inventory->littleBeefButton->buttonFocus = true;
			else if (idSelection == 1)s->world->inventory->bigBeefButton->buttonFocus = true;
			else if (idSelection == 2)s->world->inventory->featherButton->buttonFocus = true;
			else if (idSelection == 3)s->world->inventory->mantisButton->buttonFocus = true;
			else if (idSelection == 4)s->world->inventory->coinButton->buttonFocus = true;
			else if (idSelection == 5)s->world->inventory->splitButton->buttonFocus = true;
			else if (idSelection == 6) idSelection = -1;
		}
		break;
	}
	s = nullptr;
}

bool GuiManager::CleanUp()
{
	return true;
}

