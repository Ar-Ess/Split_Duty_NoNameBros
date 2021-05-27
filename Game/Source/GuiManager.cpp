#include "App.h"
#include "Textures.h"

#include "GuiManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Combat.h"

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
	if (s->GetCurrScene() != Scenes::LOGO_SCENE && s->GetCurrScene() != Scenes::WORLD) SelectButtonsLogic();

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
	for (int i = 0; i < buttons.Count(); i++) buttons[i]->active = false;
}

void GuiManager::SelectButtonsLogic()
{
	bool anythingSelected = true;
	if (idSelection <= -1)
	{
		idSelection = -1;
		anythingSelected = false;
	}
	else if (idSelection > buttons.Count())
	{
		idSelection = -1;
		anythingSelected = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		GuiButton* b1 = nullptr;
		if (anythingSelected)
		{
			b1 = (GuiButton*)buttons[idSelection];
			if (b1->buttonFocus) b1->buttonFocus = false;
		}

		idSelection++;
		if (buttons.At(idSelection) == nullptr)
		{
			idSelection = 0;
		}

		for (int i = 0; i < buttons.Count(); i++)
		{
			if (!buttons[idSelection]->active || buttons[idSelection]->state == GuiControlState::LOCKED)
			{
				idSelection++;
				if (idSelection > buttons.Count())
				{
					idSelection = 0;
					i = 0;
				}
			}
			else
			{
				GuiButton* b = (GuiButton*)buttons[idSelection];
				b->buttonFocus = true;
				b = nullptr;
				return;
			}
		}

		b1 = nullptr;
		return;
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (idSelection > -1) idSelection--;
		return;
	}
}

bool GuiManager::CleanUp()
{
	return true;
}

