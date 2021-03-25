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
	buttonSpriteSheet = nullptr;
	checkBoxSpriteSheet = nullptr;
	sliderSpriteSheet = nullptr;

	cursorTexture = app->tex->Load("Assets/Textures/UI/grab_hand.png");

	idleCursorAnim.PushBack({ 0,0,30,30});

	//clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 64,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.loop = false;
	clickCursorAnim.speed = 0.5f;

	currentCursorAnim = &idleCursorAnim;


	return true;
}

bool GuiManager::Update(float dt)
{
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

bool GuiManager::CleanUp()
{
	return true;
}

