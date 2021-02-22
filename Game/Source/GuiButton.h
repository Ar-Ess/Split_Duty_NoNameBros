#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw();

private:
    //Texture sections
    const SDL_Rect Disabled = { 0, 0, 0, 0 };
    const SDL_Rect Normal = { 0, 0, 0, 0 };
    const SDL_Rect Focused = { 0, 0, 0, 0 };
    const SDL_Rect Pressed = { 0, 0, 0, 0 };
};

#endif // __GUIBUTTON_H__
