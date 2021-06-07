#ifndef __WORLD_H__
#define __WORLD_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Player.h"
#include "Scene.h"

#include "Collider.h"

#define REL_CAMX_PLAYERX 612
#define REL_CAMY_PLAYERY 318

#define ENEMY_FIELD_ENEMY_MAX 5
#define GRASSY_1_ENEMY_MAX 4
#define GRASSY_3_ENEMY_MAX 6
#define AUTUMN_1_ENEMY_MAX 10
#define AUTUMN_2_ENEMY_MAX 5
#define MOSSY_1_ENEMY_MAX 8
#define MOSSY_2_ENEMY_MAX 4

#define ENEMY_RUN_SPEED 4
#define ENEMY_WALK_SPEED 2

#define PLAYER_INMUNITY_TIME 100

#define STAB_TIME 10
#define STAB_W 22
#define STAB_H 50

class Enemy;
enum EnemyClass;
class Player;
class Scene;
enum Scenes;
class Collision;
class Map;
struct SDL_Texture;
struct SDL_Rect;
struct Animation;
class LilipadPuzzle;
class StonePuzzle;
class ButtonPuzzle;

enum PlayerState
{
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DOWNLEFT,
    DOWNRIGHT,
    UPLEFT,
    UPRIGHT
};

enum Places
{
    NO_PLACE,
    MAIN_VILLAGE,
    ENEMY_FIELD,
    HOUSE,
    TAVERN,
    SHOP,
    GOLEM_STONES,
    CAVE,
    GRASSY_LAND_1,
    GRASSY_LAND_2,
    GRASSY_LAND_3,
    AUTUM_FALL_1,
    AUTUM_FALL_2,
    MOSSY_ROCKS_1,
    MOSSY_ROCKS_2
};


class World
{
public:

    World();

    void Start(Places place);

    void Restart(Scenes scene);

    void Update();

    void EnemyLogic();

    void Draw();

    void DrawPlayer();

    void DrawEnemy();

    void DrawNPC();

    void DrawText();

    void DrawObstacles();

    void DrawCollisions();

    void DrawFilters();

    void DrawStomps();

    void AlignCameraPosition();

    void NPCLogic();

    void PlayerInmunityLogic();

    void LoadNPCs(Places place);

    void GolemCall();

    Places GetPlace() const
    {
        return place;
    }

    iPoint GetPreviousPosition() const
    {
        return prevPosition;
    }

    int GetWorldSpeed() const
    {
        return worldSpeed;
    }

    void SetPrevPosition(int x, int y)
    {
        prevPosition = { x, y };
    }

    void SetPlace(int placex)
    {
        place = (Places)placex;
    }

    void SetInmunityTime(unsigned short int time)
    {
        playerInmuneTime = time;
    }

    PlayerState GetPlayerState() const
    {
        return playerState;
    }

public:
    bool godMode = false;

public: //GOLEMS
    bool tutorialBossActivation = false;
    bool secondBossActivation = false;
    bool thirdBossActivation = false;
    bool finalBossActivation = false;
    const SDL_Rect tutorialBossRect = { 1063, 450, 50, 25 };
    const SDL_Rect secondBossRect = { 1287, 363, 50, 25 };
    const SDL_Rect thirdBossRect = { 1513, 450, 50, 25 };
    //const SDL_Rect finalBossRect = { 0, 0, 32, 16 };
    GuiString* lvlRecomendedText = nullptr;
    bool drawRecomendedII = false;
    bool drawRecomendedIII = false;

public: //obstacles
    SDL_Texture* leaves = nullptr;
    SDL_Rect leavesCollision = { -10, 1960, 200, 200 };
    const SDL_Rect bush = { 0,0,58,50 };
    SDL_Rect logCollision = {4 * 28, 13 * 28, 8 * 28, 3 * 28};
    SDL_Texture* stomp = nullptr;
    const SDL_Rect stompRect = { 0, 40, 308, 270 };
    const SDL_Rect fallenWoodRect = { 330, 160, 320, 140 };
    SDL_Texture* statItems = nullptr;
    SDL_Rect velocityStatRect = { (40 * 28), (5 * 28) - 6, 37, 45, };
    SDL_Rect luckStatRect = { (24 * 28) + 40, (14 * 28) - 34, 37, 45, };
    const SDL_Rect velocityStat = { 0, 0, 74, 91 };
    const SDL_Rect stabStat = { 74, 0, 74, 91 };
    const SDL_Rect luckStat = { 148, 0, 74, 91 };
    bool velocityTaken = false;
    bool luckTaken = false;
    bool stabTaken = false;

private:

    void WorldMovement();

    void WorldChange();

    void WorldEnemySpawn();

    void WorldEnemyDetection();

    void WorldEnemyChasing();

    void WorldStabLogic();

    void EnemyStatsGeneration(Enemy* enemy, Player* player, int lvl);

    void EnemyRunChasing(Enemy* e, Player* p);

    void EnemyWalkReturn(Enemy* e, Player* p);

    bool CollisionSolver(iPoint prevPos);

    void ChangePlayerState();

    bool PlayerMovement();

    void CameraMovement(bool move);

    void RectifyCameraPosition(Places place);

    void UpdateWorldSpeed();

    void ChangeMap(Places place);

    void AsignPrevPosition();

    void Teleport(iPoint pos);

    void WorldStatGet();

public:
    bool inventoryOpen = false;

public: //TUTORIAL
    bool gameStart = false;
    bool tutorialDialogueOnce = false;
    SDL_Texture* night = nullptr;

public:
    bool stabActive = false;
    int stabTime = 0;
    bool stabLeft = false;
    bool stabRight = false;
    bool stabUp = false;
    bool stabDown = false;
    SDL_Rect stabRect = {};

private:

    friend class Map;
    friend class Scene;
    friend class Inventory;

    int worldSpeed;

    SDL_Rect contactPlayerZone; //Borrar!

    Collision collisionUtils;

    Map* map = nullptr;

    LilipadPuzzle* lilipadPuzzle1 = nullptr;

    StonePuzzle* stonePuzzle1 = nullptr;

    ButtonPuzzle* buttonPuzzle1 = nullptr;

    DialogueManager* dialogs = nullptr; //Dialogs update twice because in world there is a dialogManager Update as well as the update for being module (it is not a module)

    Places place;

    PlayerState playerState;
public:
    Inventory* inventory = nullptr;
private:
    iPoint prevPosition = { 0, 0 };

    bool debugCollisions = false;

    unsigned short int playerInmuneTime = 0;

    bool playerInmunity = false;

    List<SDL_Rect> houses;
    List<SDL_Rect> collisions;
    List<SDL_Rect> location1;
    List<SDL_Rect> location2;
    List<SDL_Rect> location3;
    List<SDL_Rect> tavern;
    List<SDL_Rect> shop;
    List<SDL_Rect> collisionsOnOff;

    const SDL_Rect sensor1House = { 392, 616, 112, 56 };
    const SDL_Rect sensor2House = { 924, 616, 112, 56 };
    const SDL_Rect sensor3House = { 1400, 700, 112, 56 };
    const SDL_Rect sensor4House = { 2660, 1792, 112, 56 };
    const SDL_Rect sensor5House = { 2044, 1792, 112, 56 };
    const SDL_Rect sensor6House = { 3080, 1260, 112, 56 };
    const SDL_Rect sensorVillageField1 = { 0, 140, 28, 196 };
    const SDL_Rect sensorVillageField2 = { 0, 1316, 28, 196 };
    const SDL_Rect sensorFieldVillage1 = { 1652, 280, 28, 196 };
    const SDL_Rect sensorFieldVillage2 = { 1652, 1456, 28, 196 };

private: //Textures

    SDL_Texture* wolfSpritesheet = nullptr;
    SDL_Texture* birdSpritesheet = nullptr;
    SDL_Texture* mantisSpritesheet = nullptr;
    SDL_Texture* walkingSpritesheet = nullptr;

    const SDL_Rect wolfRect = { 65,0, 60, 35 };
    const SDL_Rect birdRect = { 32 * 3, 0, 32, 32 };
    const SDL_Rect mantisRect = { 0, 0, 30, 35 };

    Animation* currentPlayerAnimation;
};

#endif // __WORLD_H__

