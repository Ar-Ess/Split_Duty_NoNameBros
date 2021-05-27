#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw();

    bool GetCheckedState() const
    {
        return checked;
    }

    bool checkBoxFocus = false;

private:

    bool checked;
    //options menu

    
    iPoint oTile = { 30 * 2,32 * 2 };
    iPoint cTile = { 28*2,31*2 };

    const SDL_Rect checkedNormal = { 0,oTile.y * 4,cTile.x,cTile.y };
    const SDL_Rect checkedFocused = { cTile.x,oTile.y * 4,cTile.x,cTile.y };
    const SDL_Rect checkedPressed = { cTile.x * 2,oTile.y * 4,cTile.x,cTile.y };

    const SDL_Rect uncheckedNormal = { 0,oTile.y * 4 + cTile.y,cTile.x,cTile.y };
    const SDL_Rect uncheckedFocused = { cTile.x,oTile.y * 4 + cTile.y,cTile.x,cTile.y };
    const SDL_Rect uncheckedPressed = { cTile.x * 2,oTile.y * 4 + cTile.y,cTile.x,cTile.y };
};

#endif // __GUICHECKBOX_H__
