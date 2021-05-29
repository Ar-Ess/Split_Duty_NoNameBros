#include "App.h"
#include "OptionsMenu.h"

#include "Audio.h"
#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiCheckBox.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "GuiSlider.h"

OptionsMenu::OptionsMenu()
{

}

void OptionsMenu::Start()
{
	optionsBackground = app->tex->Load("Assets/Screens/bg_mm.png");
	optionsGui = app->tex->Load("Assets/Screens/options_gui.png");
	app->guiManager->buttonSpriteSheet = app->tex->Load("Assets/Textures/UI/options.png");
	app->guiManager->sliderSpriteSheet = app->tex->Load("Assets/Textures/UI/slider.png");
}

void OptionsMenu::Restart()
{
	app->tex->UnLoad(app->guiManager->buttonSpriteSheet);
	app->tex->UnLoad(app->guiManager->sliderSpriteSheet);
	app->tex->UnLoad(optionsBackground);
	app->tex->UnLoad(optionsGui);
}

void OptionsMenu::Update()
{
	AudioManager* a = app->audio;

	dFullScreenCheckBox->Update(1.0f);
	fullScreenCheckBox->Update(1.0f);
	vSyncCheckBox->Update(1.0f);
	fxVolumeSlider->Update(1.0f);
	a->ChangeVolumeFx(a->ValueToVolume(fxVolumeSlider->GetValue(), fxVolumeSlider->GetMaxValue()));
	musicVolumeSlider->Update(1.0f);
	a->ChangeVolumeMusic(a->ValueToVolume(musicVolumeSlider->GetValue(), musicVolumeSlider->GetMaxValue()));
	returnMenuButton->Update(1.0f);

	a = nullptr;
}

void OptionsMenu::Draw(float y)
{
	app->render->DrawTexture(optionsBackground, 0, 0);
	app->render->DrawTexture(optionsGui, 0, y);

	y += 25;

	DrawGui(y);
	DrawText(y);
}

void OptionsMenu::DrawGui(float y)
{

	dFullScreenCheckBox->bounds.y += y;
	fullScreenCheckBox->bounds.y += y;
	vSyncCheckBox->bounds.y += y;
	fxVolumeSlider->bounds.y += y;
	fxVolumeSlider->slider.y += y;
	musicVolumeSlider->bounds.y += y;
	musicVolumeSlider->slider.y += y;
	returnMenuButton->bounds.y += y;

	dFullScreenCheckBox->Draw();
	fullScreenCheckBox->Draw();
	vSyncCheckBox->Draw();
	fxVolumeSlider->Draw(SliderType::FX);
	musicVolumeSlider->Draw(SliderType::MUSIC);
	returnMenuButton->Draw();

	dFullScreenCheckBox->bounds.y -= y;
	fullScreenCheckBox->bounds.y -= y;
	vSyncCheckBox->bounds.y -= y;
	fxVolumeSlider->bounds.y -= y;
	fxVolumeSlider->slider.y -= y;
	musicVolumeSlider->bounds.y -= y;
	musicVolumeSlider->slider.y -= y;
	returnMenuButton->bounds.y -= y;
}

void OptionsMenu::DrawText(float y)
{

	vSyncText->bounds.y += y;
	fullScreenText->bounds.y += y;
	dFullScreenText->bounds.y += y;
	returnText->bounds.y += y;

	vSyncText->Draw();
	fullScreenText->Draw();
	dFullScreenText->Draw();
	returnText->Draw();

	vSyncText->bounds.y -= y;
	fullScreenText->bounds.y -= y;
	dFullScreenText->bounds.y -= y;
	returnText->bounds.y -= y;
}
