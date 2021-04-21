#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw(float scale = 1, bool useCamera = true, bool drawTexture = true);
    void ChangeTexture(const char* path);

private:
    //Texture sections
    const SDL_Rect Disabled = { 0,0,0,0 };
    const SDL_Rect Normal = { 0, 720, 162, 60 };
    const SDL_Rect Focused = { 162, 720, 162, 60 };
    const SDL_Rect Pressed = { 162 *2, 720, 162, 60 };
    const SDL_Rect Locked = { 162 * 3, 720, 162, 60 };
    SDL_Texture* texture = nullptr;
};

#endif // __GUIBUTTON_H__
