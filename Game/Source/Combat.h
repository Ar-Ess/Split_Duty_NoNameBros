#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"
#include "QuestManager.h"
#include "ParticleSystem.h"

#include "Collider.h"

#define INIT_GROUND_POSY 444

#define MAX_MEAT 10
#define MAX_COMBATITEM 5

#define ATTACK_BUFF 25.0f
#define DEFENSE_BUFF 25.0f
#define THIRD_BUFF 25.0f

#define TUTORIAL_TEXT_MARGIN_X 20
#define TUTORIAL_TEXT_MARGIN_Y 30

class Enemy;
enum EnemyClass;
enum BossClass;
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
	BOSS_TURN,
    PLAYER_TURN,
    SECOND_PLAYER_TURN,
    WIN,
    LOSE,
    SPLIT,
    ESCAPE
};

struct InstaZone
{
    InstaZone()
    {

    }

    short int step = 0;
    SDL_Rect rect = { 1300, 488 - 15,  70, 30 };
    SDL_Texture* tex = nullptr;

    void SetUp(int stepx, SDL_Rect rectx)
    {
        step = stepx;
        rect = rectx;
    }

    void InstaKill();
};

struct ResponseZone
{
    ResponseZone()
    {

    }

    short int step = 0;
    SDL_Rect rect = { 1300, 488 - 15,  70, 30 };
    SDL_Texture* tex = nullptr;

    void SetUp(int stepx, SDL_Rect rectx)
    {
        step = stepx;
        rect = rectx;
    }
};

struct Pusher
{
    Pusher()
    {

    }

    void SetUp(int originx, SDL_Rect rectx, bool drawx, bool pushx)
    {
        origin = originx;
        rect = rectx;
        draw = drawx;
        pushing = pushx;
        Restart();
    }

    void DebugDraw()
    {
        if (draw) app->render->DrawRectangle(rect, {170, 10, 130, 100});
    }

    SDL_Rect rect;
    SDL_Texture* tex = nullptr;
    bool draw = false;
    bool pushing = false;
    int origin;
    void Move()
    {
        rect.x -= 12;
        draw = true;
    }
    void Restart()
    {
        rect.x = origin;
        draw = false;
        pushing = false;
    }
};


class Combat
{
public:

    Combat();

    void Start();

    void EnemyStart();

    void BossStart();

    void Restart();

    void Update();

    void UpdateButtons();

    void UpdatePopUps();

    void CombatLogic();

    void TutorialLogic();

    void JumpInstructionLogic();

    void Draw();

    void DrawPlayer();

    void DebugDraw();

    void DrawSecondPlayer();

    void DrawEnemy();

    void DrawBoss();

    void DrawBakcground();

    void DrawPopUps();

    void DrawButtons();

    void DrawText();

    void DrawTutorial();

    void DrawJumpInstructions();

    void FirstTurnLogic();

    void EndBattleSolving();

    void BoolStart();

    void PlayerChoiceLogic();

    void SecondPlayerChoiceLogic();

    int PlayerDamageLogic();

    int SecondPlayerDamageLogic();

    int EnemyDamageLogic();

    int BulletDamageLogic();

    void EnemyAttack(EnemyClass enemy);

    void BossAttack();

    void AfterEnemyAttack();

    void AfterBossAttack();

    void PlayerAttack();

    void SecondPlayerAttack();

    void PlayerMove();

    void PlayerItemChoose();

    void PlayerSplit();

    void PlayerResponse();

    void SecondPlayerProtect();

    void SecondPlayerBuff();

    void ItemUsage();

    void BuffLogic();

    int HealPlayer(int typeOfHeal);

    int EnemyItemDamage();

    int EnemyStabDamage();

    void ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xsplitedEnemy, int xmoney);

    void EnemyAttackProbability();

    void BossAttackProbability();

    void PlayerMoneyLose();

    void EscapeProbability(short int probabilityRange);

    void PlayerHitLogic();

    void PlayerWaveHitLogic(int i);

    void PlayerBigWaveHitLogic();

    void PlayerBulletHitLogic();

    void PlayerPosReset();

    void SecondPlayerPosReset();

    void ItemDrop(EnemyClass enemy);

    void UpdateBuffs();

private: //STATE CHANGING FUNCTIONS

    void EnemyTurn();

    void BossTurn();

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

    CombatState GetTurn() const
    {
        return combatState;
    }

public: //GENERAL BOSS INFO
    SDL_Rect wave[2] = { 1400, 0, 105, 60 };
    SDL_Rect bigWave = { 1400, 0, 120, 90 };
    bool once = false;

public: //TUTORIAL BOSS INFO
    bool tutorialActive = false;
    int tutorialStep = 0;
    short int bossTAttack1Time = 0;
    short int bossTAttack2Time = 0;
    bool jumpInstruction = false;
    int jumpInstructionStep = 0;
    bool endOfTutorial = false;
    SDL_Texture* tutorialBox = nullptr;
    SDL_Texture* enterTexture = nullptr;
    iPoint textBoxPos = { 0, 0 };
    GuiString* tutorialText;

public: //BOSS I info
    int shieldStep = 0;
    SDL_Rect shield = { 0, 285, 40, 215 };
    const int shieldPos[4] = { 155, 325, 495, 665 };
    short int boss1Attack4Time = 0;
    short int boss1Attack5Time = 0;
    short int boss1Attack6Time = 0;
    short int boss1Attack7Time = 0;

    Emitter* shieldParticle = nullptr;

public: //BOSS II info
    int spikeStep[2] = { 0, 0 };
    SDL_Rect spike[2] = { 1300, 488 - 30, 60, 30 };
    const int spikePos[4] = { 1300, 310, 480, 650 };
    short int boss2Attack1Time = 0;
    short int boss2Attack2Time = 0;
    short int boss2Attack3Time = 0;
    short int boss2Attack4Time = 0;
    short int boss2Attack5Time = 0;
    short int boss2Attack6Time = 0;
    short int boss2Attack7Time = 0;
    short int bossIIStep = 0;
    bool spikesFattenator = false;
    bool b2heal = false;

public: //BOSS III info
    InstaZone iZone = {};
    ResponseZone rZone = {};
    const short int zonePos[5] = { 1300, 249 - 35, 419 - 35, 589 - 35, 759 - 35 };
    short int boss3Attack1Time = 0;
    short int boss3Attack2Time = 0;
    short int boss3Attack3Time = 0;
    short int boss3Attack4Time = 0;
    short int boss3Attack5Time = 0;
    short int boss3Attack6Time = 0;
    short int boss3Attack7Time = 0;
    short int bossIIIStep = 0;
    Pusher pusher;

public:
    short int enemyTimeWait = 0;
    short int bossTimeWait = 0;
    short int playerTimeAttack = 0;
    short int secondPlayerTimeAttack = 0;
    short int playerTimeMove = 0;
    short int playerTimeHeal = 0;
    short int playerTimeWearFeather = 0;
    short int playerTimeWearLeg = 0;
    short int playerTimeEnemyThrow = 0;
    short int playerTimeMoneyThrow = 0;
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
    bool moneyThrow = false;

    //Player 2 Choices
    bool secondPlayerAttack = false;
    bool secondPlayerProtect = false;
    bool secondPlayerBuff = false;
    bool secondPlayerChoice = true;

    bool buffChoice = true;
    bool drawBuffMenu = false;
    short int buffCooldown = 0;

    bool attackBuff = false;
    bool defenseBuff = false;
    bool thirdBuff = false;

    short int lastStatNotBuffed = 0;
    short int buffGenerationTime = 0;

    //Items
    bool wearFeather = false;
    bool wearMantisLeg = false;

    bool secondPlayer = false;

    // END BATTLE BOOLS
    bool playerEscaped = false;
    bool playerWin = false;
    bool playerLose = false;
    bool playerSplitWin = false;

    // BATTLE TYPE
    bool enemyBattle = false;
    bool bossBattle = false;

    //PARTICLES
    Emitter* damamgeParticle = nullptr;
    Emitter* hurtParticle = nullptr;
    Emitter* moveParticle = nullptr;

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

    GuiString* turnText = nullptr;

    Animation* currentPlayerAnim = nullptr;
    Animation* currentSecondPlayerAnim = nullptr;
    Animation* currentEnemyAnim = nullptr;
    Animation* currentBossAnim = nullptr;

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

    Boss* boss = nullptr;

    CombatState combatState = NULL_STATE;

    bool debugCombat = false;

public: // BUTTONS & TEXT

    // BUTTON
    GuiButton* smallMeatButton = nullptr;

    GuiButton* largeMeatButton = nullptr;

    GuiButton* featherButton = nullptr;

    GuiButton* mantisButton = nullptr;

    GuiButton* enemySplitButton = nullptr;

    GuiButton* moneyButton = nullptr;

    // TEXT
    GuiString* attackText = nullptr;

    GuiString* moveText = nullptr;

    GuiString* itemsText = nullptr;

    GuiString* escapeText = nullptr;

    GuiString* splitText = nullptr;

    GuiString* protectText = nullptr;

    GuiString* buffText = nullptr;

    GuiString* smallMeatDescription = nullptr;

    GuiString* largeMeatDescription = nullptr;

    GuiString* featherDescription = nullptr;

    GuiString* mantisRodDescription = nullptr;

    GuiString* enemySplitDescription = nullptr;

    GuiString* moneyDescription = nullptr;

private:
    const SDL_Rect backgroundRect = { 0,0,1280,720 };

    SDL_Texture* character1Spritesheet = nullptr;
    SDL_Texture* character2Spritesheet = nullptr;

    SDL_Texture* enemySpritesheet = nullptr;
    SDL_Texture* bossSpritesheet = nullptr;

    SDL_Texture* background = nullptr;

    SDL_Texture* combatInventory = nullptr;
};
#endif // __COMBAT_H__
