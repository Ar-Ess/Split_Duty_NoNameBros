#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Collider.h"

#define INIT_ENEMY1_POSX 996
#define INIT_ENEMY1_POSY 433

class Enemy;
class Player;
class Scene;
class Collision;
class Animation;
struct SDL_Texture;

enum CombatState
{
    NULL_STATE = -1,
    ENEMY_TURN,
    PLAYER_TURN,
    WIN,
    LOSE
};

class Combat
{
public:

    Combat();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void FirstTurnLogic();

    void CombatLogic();

    void PlayerChoiceLogic();

    int PlayerDamageLogic();

    int EnemyDamageLogic();

    void EnemyAttack();

    void PlayerAttack();

    void PlayerMove();

    void PlayerItemChoose();

    void PlayerSplit();

    void PlayerResponse();

    void ItemUsage();

    int HealPlayer(int typeOfHeal);

    int EnemyItemDamage();

    void ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xtamedEnemy);

public:
    short int enemyTimeAttack = 0;
    short int enemyTimeWait = 0;
    short int playerTimeAttack = 0;
    short int playerTimeMove = 0;
    short int playerTimeHeal = 0;
    short int playerTimeWearFeather = 0;
    short int playerTimeWearLeg = 0;
    short int playerTimeEnemyThrow = 0;
    short int playerTimeSplit = 0;

    short int steps = 0;

    bool playerResponseAble = true;
    bool playerHitAble = true;

    bool playerAttack = false;
    bool playerStep = false;
    bool playerItem = false;
    bool playerScape = false;
    bool playerSplit = false;
    bool playerChoice = true;

    bool itemChoice = true;
    bool drawInventory = false;
    bool healPlayerSmall = false;
    bool healPlayerLarge = false;
    bool featherPlayerTurn = false;
    bool protectPlayerTurn = false;
    bool enemyThrow = false;

    bool wearFeather = false;
    bool wearMantisLeg = false;

public:
    short int smallMeat = 0;
    short int largeMeat = 0;
    short int feather = 0;
    short int mantisLeg = 0;
    short int tamedEnemy = 0;

private:
    friend class Scene;

    SDL_Texture* character1Spritesheet = nullptr;

    const SDL_Rect test = {52, 52, 70, 88};

    Animation* currPlayerAnim = nullptr;

public:

    bool luckArray[100] = {};

    Collision collisionUtils;

    Enemy* enemy = nullptr;

    CombatState combatState = NULL_STATE;

    SDL_Rect inventorySimulation = {192, 108, 896, 504};
};

#endif // __COMBAT_H__
