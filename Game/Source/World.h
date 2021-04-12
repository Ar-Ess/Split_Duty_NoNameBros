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

class World
{
public:

    World();

    void Start(const char*);

    void Restart();

    void Update();

    void Draw();

    void DrawPlayer();

    void DrawEnemy();

private:

    void WorldMovement();

    bool CollisionSolver(iPoint prevPos);

    bool PlayerMovement();

    void CameraMovement(bool move);

    void RectifyCameraPosition();

    void UpdateWorldSpeed();

private:

    friend class Map;

    int worldSpeed;

    friend class Scene;

    Collision collisionUtils;

    Map* map = nullptr;

    List<SDL_Rect> collisions;
    List<SDL_Rect> changeMap;
};

#endif // __COMBAT_H__

