#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"

#define INIT_ENEMY1_POSX 1007
#define INIT_ENEMY1_POSY 400

class Enemy;
class Player;
class Scene;

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

    void EnemyAttack();

    void PlayerAttack();

    void PlayerMove();

    void PlayerResponse();

public:
    int enemyTimeAttack = 0;
    int playerTimeAttack = 0;
    int playerTimeMove = 0;

    int steps = 0;

    bool playerResponseAble = true;

    bool playerAttack = false;
    bool playerScape = false;
    bool playerStep = false;

    bool playerChoice = true;

public:

    Enemy* enemy = nullptr;

    CombatState combatState = NULL_STATE;
};

#endif // __COMBAT_H__
