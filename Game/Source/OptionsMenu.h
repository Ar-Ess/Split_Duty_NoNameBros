#ifndef __OPTIONS_MENU_H__
#define __OPTIONS_MENU_H__

#include "SString.h"

class Scene;
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

    void Draw();

    void DrawGui();

    void DrawText();

private:
    friend class Scene;

    GuiCheckBox* dFullScreenCheckBox = nullptr;
    GuiCheckBox* fullScreenCheckBox = nullptr;
    GuiCheckBox* vSyncCheckBox = nullptr;
    GuiSlider* musicVolumeSlider = nullptr;
    GuiSlider* fxVolumeSlider = nullptr;
    GuiButton* returnMenuButton = nullptr;
};

#endif // __OPTIONS_MENU_H__
