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

    void Start(int exp);

    void Restart();

    void Update();

    void Draw(int x);

    void UpdateButtons();

    void SetText();

    void DrawText(int x);

    void DrawButtons(int x);

    void DrawBar(iPoint pos, int current, int max, SDL_Color color);

    void UpgradeStats(int lvl);
    void Upgrade2Stats(int lvl);
  
private:
    SDL_Texture* interfaceTexture = nullptr;
    SDL_Texture* interfaceGui = nullptr;
    SDL_Texture* barTexture = nullptr;

    const SDL_Rect expBarRect = { 0,742,261,22 };

    int maxExp = 0;

	int startLvl = 0;
    int endLvl = 0;
    int startExp = 0;
    int endExp = 0;

    bool levelUpBool = false;

    int counter = 0;
    int sumUp = 0;

public:

    GuiButton* skipButton = nullptr;

    GuiString* skipText = nullptr;

    GuiString* levelUpText = nullptr;

    GuiString* expGainedText = nullptr;

    GuiString* expText = nullptr;

    GuiString* winText = nullptr;

};

#endif // __LEVELUP_H__


