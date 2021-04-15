#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "Render.h"

#include "DynArray.h"
#include "Point.h"

#include "SDL/include/SDL.h"

#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141

struct SDL_Rect;
struct Animation;

enum EnemyClass
{
    SMALL_WOLF,
    BIG_WOLF,
    MANTIS,
    BIRD
};

struct MantisBullet
{
    MantisBullet()
    {
        bulletRect = { 1004, 440, 40, 65 };
        bulletSpritesheet = nullptr;
    }

    SDL_Rect bulletRect;
    SDL_Texture* bulletSpritesheet;
    bool active = false;

    void BulletReset()
    {
        bulletRect = { 1004, 440, 40, 65 };
        active = false;
    }

    void Update()
    {
        if (active) bulletRect.x -= 9;
    }

    void Draw()
    {
        if (active)
        {
            app->render->DrawRectangle(bulletRect, { 20, 100, 181, 255 });
        }
    }
};

class Enemy : public Entity
{
public:
    Enemy();

    virtual ~Enemy();

    void SetUp(EnemyClass xenemyClass, SDL_Rect combatCollider, SDL_Rect worldCollider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity);

    void Jump();

    void HighJump();

    void SmallWolfAttack(unsigned short int typeOfAttack);

    void BirdAttack(unsigned short int typeOfAttack);

    void MantisAttack(unsigned short int typeOfAttack);

    void MantisAttack1Logic(unsigned short int timer);

    void MantisBulletShoot();

    EnemyClass GetClass() const
    {
        return enemyClass;
    }

private: // Attack Time
    short int attack = 0;
    short int jumpTime = 0;

    short int smallWolfTimeAttack1 = 0;
    short int smallWolfTimeAttack2 = 0;

    short int birdTimeAttack1 = 0;
    short int birdTimeAttack2 = 0;
    short int birdTimeAttack3 = 0;

    short int mantisTimeAttack1 = 0;
    short int mantisTimeAttack2 = 0;
    short int mantisTimeAttack3 = 0;

public:
    SDL_Rect colliderCombat;
    SDL_Rect colliderWorld;
    SDL_Rect colliderRect;
    bool jumping = false;
    bool active = true;

public:
    friend class Combat;

    int health;
    int maxHealth;
    int defense;
    int strength;
    int velocity;
    int lvl;
    int exp;

private:
    DynArray<iPoint>* path;
    EnemyClass enemyClass;
    MantisBullet bullet[5];

public:
    Animation* currentEnemyAnim;

    Animation cLittleWolfAwakeAnim;
    Animation cLittleWolfIdleAnim;
    Animation cLittleWolfRunAnim;
};

#endif // __ENEMY_H__
