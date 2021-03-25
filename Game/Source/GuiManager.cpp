#include "App.h"
#include "Textures.h"

#include "GuiManager.h"

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
	case GuiControlType::BUTTON: control = new GuiButton(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::CHECKBOX: control = new GuiCheckBox(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::SLIDER: control = new GuiSlider(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::TEXT: control = new GuiString(); break;
	default: break;
	}

	if (control != nullptr) controls.Add(control);

	currentButton1Anim = nullptr;
	currentButton2Anim = nullptr;
	currentButton3Anim = nullptr;
	currentButton4Anim = nullptr;
	currentButton5Anim = nullptr;

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	delete entity;
	entity = nullptr;
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
	idleButtonAnim.PushBack({ 0,720,162,61 });

	currentButton1Anim = &idleButtonAnim;
	currentButton2Anim = &idleButtonAnim;
	currentButton3Anim = &idleButtonAnim;
	currentButton4Anim = &idleButtonAnim;
	currentButton5Anim = &idleButtonAnim;

	return true;
}

bool GuiManager::Update(float dt)
{
	frameCounter++;
	if (frameCounter % 25 == 0)
		secondsCounter++;

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//Activate Debug Draw
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debugGui = !debugGui;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	

	return true;
}

void GuiManager::DrawCursor()
{
	currentCursorAnim->Update(1.0f);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	app->render->DrawTexture(cursorTexture, mouseX, mouseY, &currentCursorAnim->GetCurrentFrame());

	if(app->input->GetMouseButtonDown(1) && currentCursorAnim != &clickCursorAnim)
	{
		currentCursorAnim = &clickCursorAnim;
		LOG("Clicked");
	}
	if (currentCursorAnim->HasFinished() )
	{
		currentCursorAnim = &idleCursorAnim;
		clickCursorAnim.Reset();
		LOG("change cursor anim");
		
	}
	


}

void GuiManager::DrawPlayerLifeBar(int life,int maxLife,int x,int y)
{
	int size = 4;
	int thickness = 20;
	maxLifeBar = { x,y,maxLife*size,thickness };
	app->render->DrawRectangle(maxLifeBar, MAGENTA);
	lifeBar = { x,y,life*size,thickness };
	
	//if life is critical blinks
	if (life <= maxLife / 3)
	{
		BlinkLifeBar(life, RED, SOFT_RED);
		
	}
	else
		app->render->DrawRectangle(lifeBar, RED);
}

void GuiManager::DrawEnemyLifeBar(int life, int maxLife, int x, int y)
{
	int size = 4;
	int thickness = 20;
	int offset = maxLife * size;
	app->render->DrawRectangle({x - offset,y,maxLife*size,thickness }, MAGENTA);
	lifeBar = { x-life*4,y,life * size,thickness };

	//if life is critical blinks
	if (life <= maxLife / 3)
	{
		BlinkLifeBar(life, RED, SOFT_RED);

	}
	else
		app->render->DrawRectangle(lifeBar, RED);
}

void GuiManager::BlinkLifeBar(int life, SDL_Color color1, SDL_Color color2)
{
	
	if(secondsCounter % 2 ==0)
		app->render->DrawRectangle(lifeBar, color1);
	else
		app->render->DrawRectangle(lifeBar, color2);
}

void GuiManager::DrawCombatInterface()
{
	
	/*currentButton1Anim->Update(1.0f);
	currentButton2Anim->Update(1.0f);
	currentButton3Anim->Update(1.0f);
	currentButton4Anim->Update(1.0f);
	currentButton5Anim->Update(1.0f);*/

	const SDL_Rect GuiRect = { 0,0,1280,720 };
	app->render->DrawTexture(GuiTexture, 0, 0, &GuiRect);

	//app->render->DrawTexture(GuiTexture, 0, 0, &currentButton1Anim->GetCurrentFrame());
}

bool GuiManager::CleanUp()
{
	return true;
}

