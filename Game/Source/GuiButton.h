#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"

enum ButtonType {
    MENU,
    INVENTORY,
    QUEST,
    UPGRADE
};
class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw(float scale = 1, bool useCamera = true, bool drawTexture = true,ButtonType=ButtonType::MENU);
    void ChangeTexture(const char* path);

private:
    //Texture sections
    const SDL_Rect Disabled = { 0,0,0,0 };
    const SDL_Rect Normal = { 0, 720, 162, 60 };
    const SDL_Rect Focused = { 162, 720, 162, 60 };
    const SDL_Rect Pressed = { 162 *2, 720, 162, 60 };
    const SDL_Rect Locked = { 162 * 3, 720, 162, 60 };
    SDL_Texture* texture = nullptr;

    const SDL_Rect inventoryNormal = { 665,720,49,54 };
    const SDL_Rect inventoryFocused = { 665+49,720,49,54 };
    const SDL_Rect inventoryPressed = { 665+98,720,49,54 };

    const SDL_Rect quitNormal = { 0,793,128,60 };
    const SDL_Rect quitFocused = { 0 + 128,793,128,60 };
    const SDL_Rect quitPressed = { 0 + 256,793,128,60 };

};

#endif // __GUIBUTTON_H__
