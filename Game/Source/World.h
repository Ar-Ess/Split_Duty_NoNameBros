#ifndef __WORLD_H__
#define __WORLD_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"
#include "Scene.h"

#include "Collider.h"

class Enemy;
enum EnemyClass;
class Player;
class Scene;
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

    Places GetPlace() const
    {
        return place;
    }

private:

    void WorldMovement();

    void WorldChange();

    void WorldEnemyDetection();

    bool CollisionSolver(iPoint prevPos);

    bool PlayerMovement();

    void CameraMovement(bool move);

    void RectifyCameraPosition();

    void UpdateWorldSpeed();

    void ChangeMap(Places place);

private:

    friend class Map;

    int worldSpeed;

    friend class Scene;

    Collision collisionUtils;

    Map* map = nullptr;

    Places place;

    iPoint prevPosition = { 0, 0 };
    iPoint prevCam = {0, 0};

    List<SDL_Rect> houses;
    List<SDL_Rect> collisions;
    List<SDL_Rect> location1;
    List<SDL_Rect> location2;
    List<SDL_Rect> location3;
    List<SDL_Rect> tavern;
    List<SDL_Rect> shop;

private: //Textures

    SDL_Texture* wolfSpritesheet = nullptr;
    const SDL_Rect wolfRect = {0, 166, 60, 35};
};

#endif // __COMBAT_H__

