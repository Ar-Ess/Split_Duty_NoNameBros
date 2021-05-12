#include "App.h"
#include "World.h"
#include "Scene.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Textures.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    this->texture = app->guiManager->GuiTexture;
    this->itemsTexture = app->guiManager->itemsTexture;
    
}

GuiButton::~GuiButton()
{
    text.Clear();
    text.~SString();
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
                    app->audio->SetFx(Effect::BUTTON_FOCUSSED);
                }
                state = GuiControlState::FOCUSED;
                if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || ( app->input->GetControl(A) == KeyState::KEY_REPEAT && app->input->gamePad !=nullptr))
                {
                    state = GuiControlState::PRESSED;
                }

                if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetControl(A) == KeyState::KEY_UP)
                {
                    app->audio->SetFx(Effect::BUTTON_RELEASED);
                    NotifyObserver();
                }
            }
            else state = GuiControlState::NORMAL;
        } 
    }

    return true;
}

bool GuiButton::Draw(float scale, bool useCamera, bool drawTexture, ButtonType type)
{
    if (useCamera)
    {
        if (drawTexture)
        {
            switch (type)
            {
            case ButtonType::INVENTORY:
            {
                LOG("Drawing inventory buttons...");
                switch (state)
                {

                case GuiControlState::NORMAL:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &inventoryNormal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &inventoryFocused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &inventoryPressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;


                    break;
                }
            }
            case ButtonType::QUEST:
            {
                LOG("Drawing inventory buttons...");
                switch (state)
                {

                case GuiControlState::NORMAL:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &quitNormal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &quitFocused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.0f, scale, false, &quitPressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    LOG("Quit quest...");
                    break;


                    break;
                }
            }
            case ButtonType::MENU:
            {
                LOG("Drawing menu buttons...");
                switch (state)
                {
                case GuiControlState::DISABLED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Disabled);
                    break;
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Normal);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Focused);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Pressed);
                    break;
                case GuiControlState::SELECTED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Pressed);
                    break;
                case GuiControlState::LOCKED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, &Locked);
                    break;
                }
                break;
            }


            case ButtonType::LITTLE_BEEF_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &lb_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &lb_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &lb_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &lb_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }
                break;
            }

            case ButtonType::BIG_BEEF_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &bb_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &bb_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &bb_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &bb_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }
                break;
            }

            case ButtonType::FEATHER_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &f_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &f_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &f_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &f_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }
                break;
            }

            case ButtonType::MANTIS_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &m_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &m_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &m_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &m_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }

                break;
            }

            case ButtonType::COIN_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &c_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &c_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &c_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &c_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }

                break;
            }

            case ButtonType::SPLIT_B:
            {
                switch (state)
                {

                case GuiControlState::DISABLED:
                 app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &s_disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                    if (state != GuiControlState::DISABLED)
                    {

                case GuiControlState::NORMAL:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &s_normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &s_focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(itemsTexture, bounds.x, bounds.y, scale + 0.0f, scale, false, &s_pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    app->scene->world->inventoryOpen = false;
                    break;
                    }
                }

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
        }
        else if (!useCamera)
        {
            if (drawTexture)
            {
                switch (state)
                {
                case GuiControlState::DISABLED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Disabled, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::NORMAL:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Normal, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::FOCUSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Focused, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::PRESSED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::SELECTED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Pressed, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                case GuiControlState::LOCKED:
                    app->render->DrawTexture(texture, bounds.x, bounds.y, scale + 0.4f, scale, false, &Locked, 0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
                    break;
                default:
                    break;
                }
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
}

void GuiButton::ChangeTexture(const char* path)
{
    if (texture != nullptr)
    {
        app->tex->UnLoad(texture);
        app->tex->Load(path);
    }
}
