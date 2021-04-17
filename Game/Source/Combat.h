#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"

#include "Collider.h"

#define INIT_SMALLWOLF_POSX 988
#define INIT_SMALLWOLF_POSY 444 
#define INIT_BIRD_POSX 1011
#define INIT_BIRD_POSY 350
#define INIT_MANTIS_POSX 1004
#define INIT_MANTIS_POSY 413
#define INIT_GROUND_POSY 444

#define MAX_MEAT 10
#define MAX_COMBATITEM 5

class Enemy;
enum EnemyClass;
class Player;
class Scene;
class Collision;
class Animation;
struct SDL_Texture;
struct SDL_Rect;
class GuiString;

enum CombatState
{
    NULL_STATE = -1,
    ENEMY_TURN,
    PLAYER_TURN,
    SECOND_PLAYER_TURN,
    WIN,
    LOSE,
    SPLIT,
    ESCAPE
};

class Combat
{
public:

    Combat();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void DrawPlayer();

    void DebugDraw();

    void DrawSecondPlayer();

    void DrawEnemy();

    void DrawBakcground();

    void DrawText();

    void FirstTurnLogic();

    void CombatLogic();

    void EndBattleSolving();

    void BoolStart();

    void PlayerChoiceLogic();

    void SecondPlayerChoiceLogic();

    int PlayerDamageLogic();

    int SecondPlayerDamageLogic();

    int EnemyDamageLogic();

    int BulletDamageLogic();

    void EnemyAttack(EnemyClass enemy);

    void AfterEnemyAttack();

    void PlayerAttack();

    void SecondPlayerAttack();

    void PlayerMove();

    void PlayerItemChoose();

    void PlayerSplit();

    void PlayerResponse();

    void SecondPlayerProtect();

    void SecondPlayerBuff();

    void ItemUsage();

    int HealPlayer(int typeOfHeal);

    int EnemyItemDamage();

    void ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xsplitedEnemy, int xmoney);

    void EnemyAttackProbability();

    void PlayerMoneyLose();

    void EscapeProbability(short int probabilityRange);

    void PlayerHitLogic();

    void PlayerBulletHitLogic();

    void PlayerPosReset();

    void SecondPlayerPosReset();

    void ItemDrop(EnemyClass enemy);

private: //STATE CHANGING FUNCTIONS

    void EnemyTurn();

    void PlayerTurn();

    void SecondPlayerTurn();

    void PlayerWin();

    void PlayerDie();

    void SecondPlayerDie();

    void PlayerEscape();

    void PlayerSplitWin();

public: //Getters

    bool GetSecondPlayerExistance() const
    {
        return secondPlayer;
    }

public:
    short int enemyTimeWait = 0;
    short int playerTimeAttack = 0;
    short int secondPlayerTimeAttack = 0;
    short int playerTimeMove = 0;
    short int playerTimeHeal = 0;
    short int playerTimeWearFeather = 0;
    short int playerTimeWearLeg = 0;
    short int playerTimeEnemyThrow = 0;
    short int playerTimeSplit = 0;
    short int playerTimeEscape = 0;

    short int secondPlayerTimeProtection = 0;

    short int bulletHitted = -1;

    short int steps = 0;

    short int stepDeniedTurns = 0;

    bool playerResponseAble = true;
    bool playerHitAble = true;
    bool playerStepDenied = false;
    bool secondPlayerReturning = false;

    bool enemyAttack = true;

//Player 1 Choices
    bool playerAttack = false;
    bool playerStep = false;
    bool playerItem = false;
    bool playerSplit = false;
    bool playerChoice = true;

    bool itemChoice = true;
    bool drawInventory = false;
    bool healPlayerSmall = false;
    bool healPlayerLarge = false;
    bool featherPlayerTurn = false;
    bool protectPlayerTurn = false;
    bool secondPlayerProtection = false;
    bool enemyThrow = false;

//Player 2 Choices
    bool secondPlayerAttack = false;
    bool secondPlayerProtect = false;
    bool secondPlayerBuff = false;
    bool secondPlayerChoice = true;

    bool drawBuffMenu = false;

//Items
    bool wearFeather = false;
    bool wearMantisLeg = false;

    bool secondPlayer = false;

// END BATTLE BOOLS
    bool playerEscaped = false;
    bool playerWin = false;
    bool playerLose = false;
    bool playerSplitWin = false;

public:
    short int smallMeat = 0;
    short int largeMeat = 0;
    short int feather = 0;
    short int mantisLeg = 0;
    short int splitedEnemy = 0;
    short int money = 0;

private:
    friend class Scene;
    friend class Enemy;
    friend class Player;

    const SDL_Rect test = {52, 52, 70, 88};

    GuiString* turnText = nullptr;

    Animation* currentPlayerAnim = nullptr;
    Animation* currentSecondPlayerAnim = nullptr;
    Animation* currentEnemyAnim = nullptr;

    bool CompareFrames(SDL_Rect a, SDL_Rect b)
    {
        int x = (a.x - b.x) + (a.y - b.y) + (a.w - b.w) + (a.h - b.h);
        if (x == 0) return true;

        return false;
    }

public:

    bool luckArray[100] = {};

    Collision collisionUtils;

    Enemy* enemy = nullptr;

    CombatState combatState = NULL_STATE;

    SDL_Rect inventorySimulation = {192, 108, 896, 504};

    bool debugCombat = false;

private:
    const SDL_Rect backgroundRect = { 0,0,1280,720 };

    SDL_Texture* character1Spritesheet = nullptr;
    SDL_Texture* fullscreenAttack_0 = nullptr;
    SDL_Texture* fullscreenAttack_1 = nullptr;
    SDL_Texture* fullscreenAttack_2 = nullptr;
    SDL_Texture* fullscreenAttack_3 = nullptr;

    SDL_Texture* littleWolfSpritesheet = nullptr;
    SDL_Texture* mantisSpritesheet = nullptr;
    SDL_Texture* batSpritesheet = nullptr;

    SDL_Texture* grassyLandsBackground = nullptr;
    SDL_Texture* autummFallsBackground = nullptr;
    SDL_Texture* mossyLandsBackground = nullptr;

};

#endif // __COMBAT_H__
