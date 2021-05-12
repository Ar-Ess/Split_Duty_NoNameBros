#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"

#include "Point.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141
#define PLAYER_SPEED 6

//WORLD PLAYER
#define INIT_PLAYER_WORLD_X 1722
#define INIT_PLAYER_WORLD_Y 210
#define INIT_PLAYER_WORLD_W 46
#define INIT_PLAYER_WORLD_H 84

//COMBAT PLAYER 1
#define INIT_COMBAT_POSX 225
#define INIT_COMBAT_POSY 400
#define INIT2_COMBAT_POSX 275
#define INIT2_COMBAT_POSY 350

//COMBAT PLAYER 2
#define PROTECT2_COMBAT_POSX 275
#define PROTECT2_COMBAT_POSY 350


class Animation;

enum ItemType {
    LITTLE_BEEF_I,
    BIF_BEEF_I,
    FEATHER_I,
    MANTIS_I,
    COINS_I,
    SPLIT_I
};

class PlayerInventory {
 
public:
    unsigned int littleBeef;
    unsigned int bigBeef;
    unsigned int feather;
    unsigned int mantisLeg;
    unsigned int coins;
    unsigned int split;

    PlayerInventory()
    {
        littleBeef = 0;
        bigBeef = 0;
        feather = 0;
        mantisLeg = 0;
        coins = 0;
        split = 0;
    }

    void PlayerInventory::Reset()
    {
        littleBeef = 0;
        bigBeef = 0;
        feather = 0;
        mantisLeg = 0;
        coins = 0;
        split = 0;
    }

    void AddItem(ItemType type, int amount)
    {
        switch (type)
        {
        case ItemType::LITTLE_BEEF_I:
            littleBeef += amount;
            break;
        case ItemType::BIF_BEEF_I:
            bigBeef += amount;
            break;
        case ItemType::FEATHER_I:
            feather += amount;
            break;
        case ItemType::MANTIS_I:
            mantisLeg += amount;
            break;
        case ItemType::COINS_I:
            coins += amount;
            break;
        case ItemType::SPLIT_I:
            split += amount;
            break;
        }
    }

    void UseItem(ItemType type)
    {
        LOG("Item used...");
        switch (type)
        {
        case ItemType::LITTLE_BEEF_I:
            littleBeef--;
            break;
        case ItemType::BIF_BEEF_I:
            bigBeef--;
            break;
        case ItemType::FEATHER_I:
            feather--;
            break;
        case ItemType::MANTIS_I:
            mantisLeg--;
            break;
        case ItemType::COINS_I:
            coins--;
            break;
        case ItemType::SPLIT_I:
            split--;
            break;
        }
    }
};

class Player : public Entity
{
public:

    Player();

    Player(SDL_Rect, EntityType);

    bool Update(float dt);

    bool Draw();

    void SetUp(short int healthx, short int maxHealthx,short int strengthx, short int defensex, short int luckx, short int velocityx, short int stabx, short int lvlx, short int expx, short int moneyx);

    void ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xsplitedEnemy, int xmoney);

    void SetTexture(SDL_Texture *tex);

    void Jump();

    void FeatherJump();

    void Crouch();

    void Refill();

    void RestartPlayer();

    void ResetWalkingAnimation();

    SDL_Rect GetCombatBounds()
    {
        return colliderCombat;
    }

    SDL_Rect GetWorldBounds()
    {
        return colliderWorld;
    }

    SDL_Rect GetCollisionBounds()
    {
        return collisionRect;
    }

    int GetCollCombatWidth() const
    {
        return colliderCombat.w;
    }

    int GetCollCombatHeight() const
    {
        return colliderCombat.h;
    }

    int GetCollWorldWidth() const
    {
        return colliderWorld.w;
    }

    int GetCollWorldHeight() const
    {
        return colliderWorld.h;
    }

public:
    PlayerInventory* playerInventory;
public:
    //player 1 animations
    Animation walkLeftAnim;
    Animation walkRightAnim;
    Animation walkUpAnim;
    Animation walkDownAnim;

    Animation cIdleAnim;
    Animation cAttackAnim;
    Animation cStepAnim;
    Animation cJumpAnim;
    Animation cDieAnim;
    Animation cCrouchAnim;
    Animation cScapeAnim;

    bool jump = false;
    bool crouch = false;
    int jumpTime = 0;
    int jumpFeatherTime = 0;
    int crouchTime = 0;

    //player 2
    Animation secIdleAnim;
    

public:
    friend class Combat;

    int health = 0;
    int maxHealth = 0;

    int healthStat = 1;
    int defenseStat = 1;
    int strengthStat = 1;
    int velocityStat = 1;
    int luckStat = 1;
    int stabStat = 1;
    
    int lvl = 0;
    int exp = 0;

    int playerSpeed = 8;

public: //Items
    int smallMeatCount = 0;
    int largeMeatCount = 0;
    int featherCount = 0;
    int mantisRodCount = 0;
    int splitedEnemyCount = 0;
    int moneyCount = 0;

public:
    SDL_Texture* texture;

    SDL_Rect colliderWorld;
    SDL_Rect collisionRect;
    SDL_Rect colliderCombat;

public:
    bool godMode = false;
};

#endif // __PLAYER_H__
