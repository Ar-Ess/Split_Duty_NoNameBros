#ifndef __INVENTORY_H__
#define __INVENTORY_H__

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



class Inventory
{
public:

    Inventory();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void DrawInterface();

    void DrawBar(iPoint pos,int current, int max, SDL_Color color);

    void DrawItems();

    void DrawStats();

    void DrawButtons();

	void UpdateFace();

    void DrawFace();

	void SetText();

    void DrawText();

private:
    int playerExp;
    int playerHp;
    int playerLvl;

    
    int playerItems;

	int healthPercent = 1;
    
private:
    //guide positions
    iPoint playerFacePos = { 848,69 };
    iPoint healthBarPos = { 960,60 };
    iPoint expBarPos = { 960,120 };
    
private:
    GuiString* healthText = nullptr;
    GuiString* expText = nullptr;
    GuiString* lvlText = nullptr;

	GuiString* upgradeText = nullptr;

private:
	GuiString* questText = nullptr;

private:
    SDL_Texture* interfaceTexture;

    //const SDL_Rect healthBarRect = { 0,0,261,422 };
    const SDL_Rect healthBarRect = { 0,720,261,22 };
    const SDL_Rect expBarRect = { 0,742,261,22 };
        
    SDL_Texture* faceAnimationTexture;
    Animation* currPlayerFaceAnim;
    Animation idleFaceAnim;
	Animation blinkFaceAnim;
    Animation angryFaceAnim;

	int counter = 0;
	int seconds = 0;

private:
    SDL_Texture* itemsTexture;

    SDL_Rect littleBeefRect = { 0,0,80,80 };
	GuiString* littleBeefText = nullptr;
    SDL_Rect bigBeefRect = { 0,0,80,80 };
	GuiString* bigBeefText = nullptr;
    SDL_Rect featherRect = { 160,0,80,80 };
	GuiString* featherText = nullptr;
    SDL_Rect mantisRect = { 240,0,80,80 };
	GuiString* mantisText = nullptr;
    SDL_Rect coinRect = { 320,0,80,80 };
	GuiString* coinText = nullptr;
    SDL_Rect splitRect = { 400,0,80,80 };
	GuiString* splitText = nullptr;

    SDL_Rect mLittleBeefRect = { 0,81,80,80 };
    SDL_Rect mBigBeefRect = { 0,81,80,80 };
    SDL_Rect mFeatherRect = { 160,81,80,80 };
    SDL_Rect mMantisRect = { 240,81,80,80 };
    SDL_Rect mCoinRect = { 320,81,80,80 };
    SDL_Rect mSplitRect = { 400,81,80,80 };
 

    iPoint itemPos = { 100,160 };
    iPoint itemOff = { 190,134};
	iPoint numberItemPos = { 83,222 };
	iPoint numberItemOff = { 180,110 };

    SDL_Texture* statsTexture = nullptr;

	iPoint statsPos = { 850,200 };
	iPoint statsOff = { 0,60 };

	int sprite = 80;
    SDL_Rect healthStatRect = { 0,0,60,60 };
    SDL_Rect strenghtStatRect = { sprite,0,60,60 };
    SDL_Rect defenseStatRect = { sprite * 2,0,60,60 };
    SDL_Rect velocityStatRect = { sprite * 3,0,60,60 };
    SDL_Rect luckStatRect = { sprite * 4,0,60,60 };
    SDL_Rect stabStatRect = { sprite * 5,0,60,60 };

public:
    GuiButton* quitQuestButton = nullptr;

    GuiButton* exitInventoryButton = nullptr;
};

#endif // __INVENTORY_H__

