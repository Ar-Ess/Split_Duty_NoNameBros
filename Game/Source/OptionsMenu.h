#ifndef __OPTIONS_MENU_H__
#define __OPTIONS_MENU_H__

#include "SString.h"

enum SliderType;

class Scene;
class GuiManager;
struct SDL_Texture;
class GuiString;
class GuiCheckBox;
class GuiButton;
class GuiSlider;

class OptionsMenu
{
public:

    OptionsMenu();

    void Start();

    void Restart();

    void Update();

    void Draw(int y);

    void DrawGui(int y);

    void DrawText(int y);

    void EasingsOptionsResolve(int value, bool way);

private:
    friend class Scene;
    friend class GuiManager;
  
    GuiCheckBox* dFullScreenCheckBox = nullptr;
    GuiCheckBox* fullScreenCheckBox = nullptr;
    GuiCheckBox* vSyncCheckBox = nullptr;
    GuiButton* returnMenuButton = nullptr;
public:
    GuiSlider* musicVolumeSlider = nullptr;
    GuiSlider* fxVolumeSlider = nullptr;
    

public:
    GuiString* fullScreenText = nullptr;
    GuiString* dFullScreenText = nullptr;
    GuiString* vSyncText = nullptr;

    GuiString* returnText = nullptr;

public:
    SDL_Texture* optionsBackground = nullptr;
    SDL_Texture* optionsGui = nullptr;
};

#endif // __OPTIONS_MENU_H__
