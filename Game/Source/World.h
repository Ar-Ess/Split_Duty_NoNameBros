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

    void PlayerMovement();

    void CameraMovement();

    void RectifyCameraPosition();

    void UpdateWorldSpeed();

private:

    int worldSpeed;

    friend class Scene;

    Collision collisionUtils;

    Map* map = nullptr;
};

#endif // __COMBAT_H__

