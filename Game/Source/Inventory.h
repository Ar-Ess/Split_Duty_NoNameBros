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

    void UseItems();

    void Draw();

    void DrawInterface();

    void DrawBar(iPoint pos,int current, int max, SDL_Color color);

    void SetButtons();

    void SetInventoryValues();

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

    
	GuiString* littleBeefText = nullptr;
	GuiString* bigBeefText = nullptr;
	GuiString* featherText = nullptr;
	GuiString* mantisText = nullptr;
	GuiString* coinText = nullptr;
	GuiString* splitText = nullptr;

    
 

    iPoint itemPos = { 100,160 };
    iPoint itemOff = { 190,134};
	iPoint numberItemPos = {170,205 };
	iPoint numberItemOff = { 190,132 };

    SDL_Texture* statsTexture = nullptr;

	int sprite = 80;
    SDL_Rect healthStatRect = { 0,0,60,60 };
    SDL_Rect strenghtStatRect = { sprite,0,60,60 };
    SDL_Rect defenseStatRect = { sprite * 2,0,60,60 };
    SDL_Rect velocityStatRect = { sprite * 3,0,60,60 };
    SDL_Rect luckStatRect = { sprite * 4,0,60,60 };
    SDL_Rect stabStatRect = { sprite * 5,0,60,60 };

    GuiString* healthStatText = nullptr;
    GuiString* strenghtStatText = nullptr;
    GuiString* defenseStatText = nullptr;
    GuiString* velocityStatText = nullptr;
    GuiString* luckStatText = nullptr;
    GuiString* stabStatText = nullptr;
    iPoint statsPos = { 1175,257 };
    iPoint statsOff = { 0,68 };

public:
    GuiButton* quitQuestButton = nullptr;

    GuiButton* exitInventoryButton = nullptr;

    //items
    GuiButton* bigBeefButton = nullptr;
    GuiButton* littlebeefButton = nullptr;
    GuiButton* featherButton = nullptr;
    GuiButton* mantisButton = nullptr;
    GuiButton* coinButton = nullptr;
    GuiButton* splitButton = nullptr;
};

#endif // __INVENTORY_H__

