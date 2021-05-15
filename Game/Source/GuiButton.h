#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"

enum ButtonType {
    MENU,
    INVENTORY,
    QUEST,
    UPGRADE,
    LITTLE_BEEF_B,
    BIG_BEEF_B,
    FEATHER_B,
    MANTIS_B,
    COIN_B,
    SPLIT_B
};
class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool UpdateOnClick(float dt);
    bool Draw(float scale = 1, bool useCamera = true, bool drawTexture = true, ButtonType = ButtonType::MENU);
    void ChangeTexture(const char* path);

private:
    //Texture sections
    const SDL_Rect Disabled = { 0,0,0,0 };
    const SDL_Rect Normal = { 0, 720, 162, 60 };
    const SDL_Rect Focused = { 162, 720, 162, 60 };
    const SDL_Rect Pressed = { 162 * 2, 720, 162, 60 };
    const SDL_Rect Locked = { 162 * 3, 720, 162, 60 };
    SDL_Texture* texture = nullptr;
    SDL_Texture* itemsTexture = nullptr;


    const SDL_Rect inventoryNormal = { 665,720,49,54 };
    const SDL_Rect inventoryFocused = { 665 + 49,720,49,54 };
    const SDL_Rect inventoryPressed = { 665 + 98,720,49,54 };

    const SDL_Rect quitNormal = { 0,793,128,60 };
    const SDL_Rect quitFocused = { 0 + 128,793,128,60 };
    const SDL_Rect quitPressed = { 0 + 256,793,128,60 };

    iPoint itemsPos = { 0,853 };
    iPoint itemsOff = { 180, 110 };
    iPoint tile = { 80,81 };

    const SDL_Rect lb_normal = { 0,0,tile.x,tile.y };
    const SDL_Rect lb_disabled = { 0,tile.y,tile.x,tile.y };
    const SDL_Rect lb_focused = { 0,tile.y*2,tile.x,tile.y };
    const SDL_Rect lb_pressed = { 0,tile.y * 3,tile.x,tile.y };

    const SDL_Rect bb_normal = { tile.x,0,tile.x,tile.y };
    const SDL_Rect bb_disabled = { tile.x,tile.y,tile.x,tile.y };
    const SDL_Rect bb_focused = { tile.x,tile.y * 2,tile.x,tile.y };
    const SDL_Rect bb_pressed = { tile.x,tile.y * 3,tile.x,tile.y };

    const SDL_Rect f_normal = { tile.x*2,0,tile.x,tile.y };
    const SDL_Rect f_disabled = { tile.x*2,tile.y,tile.x,tile.y };
    const SDL_Rect f_focused = { tile.x*2,tile.y * 2,tile.x,tile.y };
    const SDL_Rect f_pressed = { tile.x*2,tile.y * 3,tile.x,tile.y };

    const SDL_Rect m_normal = { tile.x * 3,0,tile.x,tile.y };
    const SDL_Rect m_disabled = { tile.x * 3,tile.y,tile.x,tile.y };
    const SDL_Rect m_focused = { tile.x * 3,tile.y * 2,tile.x,tile.y };
    const SDL_Rect m_pressed = { tile.x * 3,tile.y * 3,tile.x,tile.y };

    const SDL_Rect c_normal = { tile.x * 4,0,tile.x,tile.y };
    const SDL_Rect c_disabled = { tile.x * 4,tile.y,tile.x,tile.y };
    const SDL_Rect c_focused = { tile.x * 4,tile.y * 2,tile.x,tile.y };
    const SDL_Rect c_pressed = { tile.x * 4,tile.y * 3,tile.x,tile.y };

    const SDL_Rect s_normal = { tile.x * 5,0,tile.x,tile.y };
    const SDL_Rect s_disabled = { tile.x * 5,tile.y,tile.x,tile.y };
    const SDL_Rect s_focused = { tile.x * 5,tile.y * 2,tile.x,tile.y };
    const SDL_Rect s_pressed = { tile.x * 3,tile.y * 3,tile.x,tile.y };

    //lvl up scene
    const SDL_Rect upgradeNormal = { 0,0,52,55 };
    const SDL_Rect upgradePressed = { 52,0,52,55 };
    
};

#endif // __GUIBUTTON_H__
