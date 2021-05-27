#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"

enum SliderType {
    MUSIC,
    FX,
};

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw(SliderType type);
    bool CleanUp();
    bool Destroy();

    void SetMinValue(int val);
    void SetMaxValue(int val);
    void SetValue(int val);
    void UpdateValue();

    int GetMinValue() const;
    int GetMaxValue() const;
    int GetValue() const;
    float GetPercentValue() const;
    void SetSlider(SDL_Rect bounds);

    bool sliderFocus = false;

private:

    SDL_Rect slider;
    int value;
    float percentValue;

    int minValue;
    int maxValue;

    bool sliderClicked = false;

    const SDL_Rect normalBar = { 0, 0, 430, 20 };
    const SDL_Rect pressedBar = normalBar;
    const SDL_Rect normalSlider = { 0, 0, 0, 0 };
    const SDL_Rect pressedSlider = { 0, 0, 0, 0 };

    iPoint oTile = { 30 * 2,32 * 2 };

    const SDL_Rect musicNormal = { 0,0,oTile.x,oTile.y };
    const SDL_Rect musicFocused = { oTile.x,0,oTile.x,oTile.y };
    const SDL_Rect musicPressed = { oTile.x * 2,0,oTile.x,oTile.y };

    const SDL_Rect lowMusicNormal = { 0,380,oTile.x,oTile.y };
    const SDL_Rect lowMusicFocused = { oTile.x,380,oTile.x,oTile.y };
    const SDL_Rect lowMusicPressed = { oTile.x * 2,380,oTile.x,oTile.y };

    const SDL_Rect mutedMusicNormal = { 0,oTile.y,oTile.x,oTile.y };
    const SDL_Rect mutedMusicFocused = { oTile.x,oTile.y,oTile.x,oTile.y };
    const SDL_Rect mutedMusicPressed = { oTile.x * 2,oTile.y,oTile.x,oTile.y };

    const SDL_Rect fxNormal = { 0,oTile.y * 2,oTile.x,oTile.y };
    const SDL_Rect fxFocused = { oTile.x,oTile.y * 2,oTile.x,oTile.y };
    const SDL_Rect fxPressed = { oTile.x * 2,oTile.y * 2,oTile.x,oTile.y };

    const SDL_Rect mutedFxNormal = { 0,oTile.y * 3,oTile.x,oTile.y };
    const SDL_Rect mutedFxFocused = { oTile.x,oTile.y * 3,oTile.x,oTile.y };
    const SDL_Rect mutedFxPressed = { oTile.x * 2,oTile.y * 3,oTile.x,oTile.y };
};

#endif // __GUISLIDER_H__
