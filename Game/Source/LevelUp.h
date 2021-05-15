#ifndef __LEVELUP_H__
#define __LEVELUP_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"
#include "GuiManager.h"
#include "GuiButton.h"

#include "Collider.h"

#define MAGENTA {190,0,0,150}
#define RED {255,0,0,255}
#define SOFT_RED {240,80,0,255}
#define BLUE {0,143,255,255}
#define CYAN {0,255,247,255}	

class Player;
class Scene;
class Collision;
class Animation;
struct SDL_Texture;
struct SDL_Rect;
class GuiString;



class LevelUp
{
public:

    LevelUp();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void SetButtons();

    void UpdateButtons();

    void SetText();

    void UpdateText();

    void DrawText();

    void DrawButtons();
  
private:
    SDL_Texture* interfaceTexture = nullptr;
    

public:
    SDL_Texture* buttonsTexture = nullptr;      

    iPoint buttonPos = { 1000,200 };
    iPoint buttonOff = { 0,30 };

    GuiButton* upgradeHealthButton = nullptr;
    GuiButton* upgradeDefenseButton = nullptr;
    GuiButton* upgradeAttackButton = nullptr;
    GuiButton* upgradeSpeedButton = nullptr;
    GuiButton* upgradeLuckButton = nullptr;
    GuiButton* upgradeStabButton = nullptr;

    GuiString* upgradeText = nullptr;

    GuiString* battleOvercomeText = nullptr;

    GuiString* upgradePointsText = nullptr;

};

#endif // __LEVELUP_H__


