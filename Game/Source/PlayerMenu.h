#ifndef __PLAYERMENU_H__
#define __PLAYERMENU_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"
#include "GuiManager.h"
#include "GuiButton.h"

#include "Collider.h"




class Player;
class Scene;
class Collision;
class Animation;
struct SDL_Texture;
struct SDL_Rect;
class GuiString;

enum ItemType
{
    COINS
};

struct Item 
{
    ItemType itemType;
    unsigned int amount;
};

class PlayerMenu
{
public:

    PlayerMenu();

    void Start();

    void Restart();

    void Update();

    void DrawInterface();

    void DrawBar(iPoint pos,int current, int max, SDL_Color color);

    void DrawItems();

    void DrawText();


private:
    int playerExp;
    int playerHp;
    int playerLvl;

    
    int playerItems;
    
private:
    //guide positions
    iPoint playerFacePos;
    iPoint healthBarPos;
    iPoint expBarPos;
    
private:
    GuiString* healthText = nullptr;
    GuiString* expText = nullptr;
    GuiString* lvlText = nullptr;


private:
    SDL_Texture* interfaceTexture;

    Animation* currPlayerFaceAnim;
    Animation idleFaceAnim;
    Animation angryFaceAnim;

};

#endif // __COMBAT_H__

