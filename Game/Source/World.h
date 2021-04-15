#ifndef __WORLD_H__
#define __WORLD_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"

#include "Collider.h"

#define REL_CAMX_PLAYERX 612
#define REL_CAMY_PLAYERY 318

class Enemy;
enum EnemyClass;
class Player;
class Scene;
enum Scenes;
class Collision;
class Map;
struct SDL_Texture;
struct SDL_Rect;

enum Places
{
    NO_PLACE,
    MAIN_VILLAGE,
    ENEMY_FIELD,
    HOUSE,
    TAVERN,
    SHOP,
    GOLEM_STONES,
    GRASSY_LAND,
    AUTUM_FALL,
    MOSSY_ROCKS
};

class World
{
public:

    World();

    void Start(Places place);

    void Restart();

    void Update();

    void Draw();

    void DrawPlayer();

    void DrawEnemy();

    void DrawCollisions();

    void AlignCameraPosition();

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

private:

    void WorldMovement();

    void WorldChange();

    void WorldEnemyDetection();

    bool CollisionSolver(iPoint prevPos);

    bool PlayerMovement();

    void CameraMovement(bool move);

    void RectifyCameraPosition(Places place);

    void UpdateWorldSpeed();

    void ChangeMap(Places place);

    void AsignPrevPosition();

private:

    friend class Map;
    friend class Scene;

    int worldSpeed;

    Collision collisionUtils;

    Map* map = nullptr;

    Places place;

    iPoint prevPosition = { 0, 0 };

    bool drawCollisions = false;

    List<SDL_Rect> houses;
    List<SDL_Rect> collisions;
    List<SDL_Rect> location1;
    List<SDL_Rect> location2;
    List<SDL_Rect> location3;
    List<SDL_Rect> tavern;
    List<SDL_Rect> shop;

    const SDL_Rect sensor1House = { 392, 616, 112, 56 };
    const SDL_Rect sensor2House = { 924, 616, 112, 56 };
    const SDL_Rect sensor3House = { 1400, 700, 112, 56 };
    const SDL_Rect sensor4House = { 2660, 1792, 112, 56 };
    const SDL_Rect sensor5House = { 2044, 1792, 112, 56 };
    const SDL_Rect sensor6House = { 3080, 1260, 112, 56 };
    const SDL_Rect sensorVillageField1 = { 0, 140, 28, 196};
    const SDL_Rect sensorVillageField2 = { 0, 1316, 28, 196 };
    const SDL_Rect sensorFieldVillage1 = { 1652, 280, 28, 196 };
    const SDL_Rect sensorFieldVillage2 = { 1652, 1456, 28, 196 };

private: //Textures

    SDL_Texture* wolfSpritesheet = nullptr;
    const SDL_Rect wolfRect = {0, 166, 60, 35};
};

#endif // __COMBAT_H__

