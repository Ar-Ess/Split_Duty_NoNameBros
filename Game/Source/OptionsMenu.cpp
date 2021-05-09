#include "App.h"
#include "OptionsMenu.h"

#include "Audio.h"
#include "Scene.h"
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

}

void OptionsMenu::Restart()
{
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

void OptionsMenu::Draw()
{
	dFullScreenCheckBox->Draw();
	fullScreenCheckBox->Draw();
	vSyncCheckBox->Draw();
	fxVolumeSlider->Draw();
	musicVolumeSlider->Draw();
	returnMenuButton->Draw();
}

void OptionsMenu::DrawButtons()
{
}

void OptionsMenu::DrawText()
{
}
