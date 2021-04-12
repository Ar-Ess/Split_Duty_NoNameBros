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
    ENEMY_FILD,
    HOUSE,
    GOLEM_STONES,
    GRASSY_LAND,
    AUTUM_FALLS,
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

    List<SDL_Rect> houses;
    List<SDL_Rect> collisions;
    List<SDL_Rect> location1;
    List<SDL_Rect> location2;
    List<SDL_Rect> location3;
};

#endif // __COMBAT_H__

