#include "App.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "Audio.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiButton::~GuiButton()
{
    delete &text;
}

bool GuiButton::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        if (state == GuiControlState::LOCKED)
        {

        }
        else
        {
            int mouseX, mouseY;
            app->input->GetMousePosition(mouseX, mouseY);

            if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
                (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
            {
                if (state == GuiControlState::NORMAL)
                {
                    //app->audio->PlayFx(Focus Mouse Sound);
                }
                state = GuiControlState::FOCUSED;
                if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
                {
                    state = GuiControlState::PRESSED;
                }

                if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
                {
                    //app->audio->PlayFx(Click Sound);
                    NotifyObserver();
                }
            }
            else state = GuiControlState::NORMAL;
        }
        
    }

    return true;
}

bool GuiButton::Draw(float scale, bool useCamera)
{
    if (useCamera)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Disabled);
            break;
        case GuiControlState::NORMAL:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Normal);
            break;
        case GuiControlState::FOCUSED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Focused);
            break;
        case GuiControlState::PRESSED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Pressed);
            break;
        case GuiControlState::SELECTED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Pressed);
            break;
        case GuiControlState::LOCKED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, &Locked);
            break;
        default:
            break;
        }

        if (app->guiManager->debugGui)
        {
            switch (state)
            {
            case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, { 100, 100, 100, 80 });
                break;
            case GuiControlState::NORMAL: app->render->DrawRectangle(bounds, { 0, 255, 0, 80 });
                break;
            case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, { 255, 255, 0, 80 });
                break;
            case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, { 0, 255, 255, 80 });
                break;
            case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, { 0, 255, 0, 80 });
                break;
            case GuiControlState::LOCKED: app->render->DrawRectangle(bounds, { 220, 155, 0, 80 });
                break;
            default:
                break;
            }
        }
    }
    else if (!useCamera)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false, &Disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        case GuiControlState::NORMAL:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false, &Normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        case GuiControlState::FOCUSED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false, &Focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        case GuiControlState::PRESSED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false,  &Pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        case GuiControlState::SELECTED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false, &Pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        case GuiControlState::LOCKED:
            app->render->DrawTexture(app->guiManager->GuiTexture, bounds.x, bounds.y, scale, false, &Locked, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
            break;
        default:
            break;
        }

        if (app->guiManager->debugGui)
        {
            switch (state)
            {
            case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, { 100, 100, 100, 80 }, true, false);
                break;
            case GuiControlState::NORMAL: app->render->DrawRectangle(bounds, { 0, 255, 0, 80 }, true, false);
                break;
            case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, { 255, 255, 0, 80 }, true, false);
                break;
            case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, { 0, 255, 255, 80 }, true, false);
                break;
            case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, { 0, 255, 0, 80 }, true, false);
                break;
            case GuiControlState::LOCKED: app->render->DrawRectangle(bounds, { 220, 155, 0, 80 }, true, false);
                break;
            default:
                break;
            }
        }
    }
    return false;
}
