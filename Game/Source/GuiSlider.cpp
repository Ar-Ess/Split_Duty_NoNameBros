#include "App.h"
#include "GuiSlider.h"
#include "GuiManager.h"
#include "OptionsMenu.h"
#include "Audio.h"

#include "Log.h"
#include "Defs.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiSlider::~GuiSlider()
{
    delete &text;
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        if ((mouseX > slider.x) && (mouseX < (slider.x + slider.w)) &&
            (mouseY > slider.y) && (mouseY < (slider.y + slider.h)))
        {
            if (state == GuiControlState::NORMAL)
            {
                app->audio->SetFx(Effect::BUTTON_FOCUSSED);
            }
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN || app->input->GetControl(A) == KeyState::KEY_DOWN) sliderClicked = true;
        }
        else if (/*app->input->GetPadEnabled() &&*/ sliderFocus)
        {
            if (state == GuiControlState::NORMAL)
            {
                app->audio->SetFx(Effect::BUTTON_FOCUSSED);
            }
            state = GuiControlState::FOCUSED;

            sliderClicked = true;
        }
        else if (!sliderClicked) state = GuiControlState::NORMAL;

        if (sliderClicked)
        {
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetControl(A) == KeyState::KEY_REPEAT)
            {
                if (state == GuiControlState::FOCUSED)
                {
                    if (app->GetFrameCount() % 30 == 0 && strcmp(text.GetString(), "FxVolumeSlider") == 0) app->audio->SetFx(Effect::BUTTON_RELEASED);
                }
                state = GuiControlState::SELECTED;

                if (mouseX > (bounds.x + slider.w / 2) && mouseX < (bounds.x + bounds.w - slider.w / 2)) slider.x = mouseX - (slider.w * 0.5);
                else if (mouseX + (int)(slider.w * 0.5) > (bounds.x + bounds.w)) slider.x = (bounds.x + bounds.w) - slider.w;
                else if ((mouseX - (int)(slider.w * 0.5)) < bounds.x) slider.x = bounds.x;
                UpdateValue();
            }
            else if (app->input->GetControl(LEFT_PAD) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN)
            {
                if (state == GuiControlState::FOCUSED)
                {
                    if (strcmp(text.GetString(), "FxVolumeSlider") == 0) app->audio->SetFx(Effect::BUTTON_RELEASED);
                }
                state = GuiControlState::SELECTED;

                slider.x -= 10;

                if (((slider.w / 2) + slider.x) < bounds.x)
                {
                    slider.x = bounds.x - (slider.w / 2);
                }

                UpdateValue();
            }
            else if (app->input->GetControl(RIGHT_PAD) == KeyState::KEY_DOWN || app->input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN)
            {
                if (state == GuiControlState::FOCUSED)
                {
                    if (strcmp(text.GetString(), "FxVolumeSlider") == 0) app->audio->SetFx(Effect::BUTTON_RELEASED);
                }
                state = GuiControlState::SELECTED;

                slider.x += 10;

                if (((slider.w / 2) + slider.x) > bounds.x + bounds.w) slider.x = bounds.x + bounds.w - (slider.w / 2);

                UpdateValue();
            }
            else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetControl(A) == KeyState::KEY_UP || !sliderFocus) sliderClicked = false;
        }
    }

    return false;
}

bool GuiSlider::Draw(SliderType type)
{
    iPoint off = { 0,0 };

    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, slider.x, slider.y, &normalSlider);
    }
    break;

    case GuiControlState::NORMAL:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        if (type == SliderType::MUSIC)
        {
            if(app->scene->optionsMenu->musicVolumeSlider->GetValue() >65)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &musicNormal);
            else if (app->scene->optionsMenu->musicVolumeSlider->GetValue()  >0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &lowMusicNormal);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedMusicNormal);

        }
        else
        {
            if (app->scene->optionsMenu->fxVolumeSlider->GetValue() > 0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &fxNormal);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedFxNormal);
            
        }
    }
    break;

    case GuiControlState::FOCUSED:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &pressedBar);
        if (type == SliderType::MUSIC)
        {
            if (app->scene->optionsMenu->musicVolumeSlider->GetValue() > 65)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &musicFocused);
            else if (app->scene->optionsMenu->musicVolumeSlider->GetValue() > 0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &lowMusicFocused);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedMusicFocused);

        }
        else
        {
            if (app->scene->optionsMenu->fxVolumeSlider->GetValue() > 0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &fxFocused);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedFxFocused);

        }
    }
    break;

    case GuiControlState::PRESSED:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &normalBar);
        if (type == SliderType::MUSIC)
        {
            if (app->scene->optionsMenu->musicVolumeSlider->GetValue() > 65)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &musicPressed);
            else if (app->scene->optionsMenu->musicVolumeSlider->GetValue() > 0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &lowMusicPressed);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedMusicPressed);

        }
        else
        {
            if (app->scene->optionsMenu->fxVolumeSlider->GetValue() > 0)
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &fxPressed);
            else
                app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x - off.x, slider.y - off.y, &mutedFxPressed);

        }
    }
    break;

    case GuiControlState::SELECTED:
    {
        app->render->DrawTexture(app->guiManager->sliderSpriteSheet, bounds.x, bounds.y, &pressedBar);
        if (type == SliderType::MUSIC)app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x-off.x, slider.y-off.y, &musicPressed);
        else app->render->DrawTexture(app->guiManager->buttonSpriteSheet, slider.x-off.x, slider.y-off.y, &fxPressed);
    }
    break;

    default:
        break;
    }
    if (app->guiManager->debugGui)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
        {
            app->render->DrawRectangle(bounds, { 100, 100, 100, 80 });
            app->render->DrawRectangle(slider, { 150, 100, 100, 80 });
        }
        break;

        case GuiControlState::NORMAL:
        {
            app->render->DrawRectangle(bounds, { 0, 255, 0, 80 });
            app->render->DrawRectangle(slider, { 0, 200, 0, 80 });
        }
        break;

        case GuiControlState::FOCUSED:
        {
            app->render->DrawRectangle(bounds, { 255, 255, 0, 80 });
            app->render->DrawRectangle(slider, { 255, 255, 0, 80 });
        }
        break;

        case GuiControlState::PRESSED:
        {
            app->render->DrawRectangle(bounds, { 0, 255, 255, 80 });
            app->render->DrawRectangle(slider, { 0, 200, 200, 80 });
        }
        break;

        case GuiControlState::SELECTED:
        {
            app->render->DrawRectangle(bounds, { 255, 255, 0, 80 });
            app->render->DrawRectangle(slider, { 200, 200, 0, 80 });
        }
        break;

        default:
            break;
        }
    }

    return false;
}

bool GuiSlider::CleanUp()
{
    slider = {0, 0, 0, 0};
    minValue = NULL;
    maxValue = NULL;

    return true;
}

bool GuiSlider::Destroy()
{
    CleanUp();
    bounds = { 0, 0, 0, 0 };
    observer = nullptr;
    text.Clear();
    id = NULL;

    return true;
}

void GuiSlider::SetMinValue(int val)
{
    minValue = val;
}

void GuiSlider::SetMaxValue(int val)
{
    maxValue = val;
}

void GuiSlider::SetValue(int val)
{
    value = val;
}

void GuiSlider::UpdateValue()
{
    float sliderPos = ((slider.x + (slider.w / 2)) - (bounds.x + (slider.w / 2)));
    float totalWidth = bounds.w - slider.w;

    percentValue = sliderPos / totalWidth;

    value = (int)floor(percentValue * maxValue);
}

int GuiSlider::GetMinValue() const
{
    return minValue;
}

int GuiSlider::GetMaxValue() const
{
    return maxValue;
}

int GuiSlider::GetValue() const
{
    return value;
}

float GuiSlider::GetPercentValue() const
{
    return percentValue * 100;
}

void GuiSlider::SetSlider(SDL_Rect bounds)
{
    slider = { (bounds.x + (int)(bounds.w * 0.5)) - (int)(bounds.h * 0.5), bounds.y, bounds.h,bounds.h };
    UpdateValue();
}
