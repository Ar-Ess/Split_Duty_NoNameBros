#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"

#include "Collider.h"

#define INIT_ENEMY1_POSX 996
#define INIT_ENEMY1_POSY 433

class Enemy;
class Player;
class Scene;
class Collision;

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

    void FirstTurnLogic();

    void PlayerChoiceLogic();

    int PlayerDamageLogic();

    int EnemyDamageLogic();

    void EnemyAttack();

    void PlayerAttack();

    void PlayerMove();

    void PlayerItemChoose();

    void PlayerResponse();

    void ItemUsage();

    int HealPlayer(int typeOfHeal);

    int EnemyItemDamage();

public:
    int enemyTimeAttack = 0;
    int enemyTimeWait = 0;
    int playerTimeAttack = 0;
    int playerTimeMove = 0;
    int playerTimeHeal = 0;
    int playerTimeWearFeather = 0;
    int playerTimeWearLeg = 0;
    int playerTimeEnemyThrow = 0;

    int steps = 0;

    bool playerResponseAble = true;
    bool playerHitAble = true;

    bool playerAttack = false;
    bool playerStep = false;
    bool playerItem = false;
    bool playerScape = false;
    bool playerChoice = true;

    bool itemChoice = true;
    bool healPlayerSmall = false;
    bool healPlayerLarge = false;
    bool featherPlayerTurn = false;
    bool protectPlayerTurn = false;
    bool enemyThrow = false;

    bool wearFeather = false;
    bool wearMantisLeg = false;

public:
    int smallMeat = 0;
    int largeMeat = 0;
    int feather = 0;
    int mantisLeg = 0;
    int tamedEnemy = 0;

public:

    bool luckArray[100];

    Collision collisionUtils;

    Enemy* enemy = nullptr;

    CombatState combatState = NULL_STATE;

    SDL_Rect inventorySimulation = {192, 108, 896, 504};
};

#endif // __COMBAT_H__
