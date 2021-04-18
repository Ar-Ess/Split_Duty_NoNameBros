#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "Render.h"
#include "Textures.h"

#include "DynArray.h"
#include "Point.h"

#include "SDL/include/SDL.h"

#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141

struct SDL_Rect;
struct Animation;
struct SDL_Texture;

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
        
        
        //bulletSpritesheet = nullptr;
        
            bulletSpritesheet=app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis-acid-bullet.png");
    }

    SDL_Rect bulletRect;
    const SDL_Rect sprite= { 0,0,32,32 };
    int frame;

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
            
            app->render->DrawTexture(bulletSpritesheet, bulletRect.x - 40, bulletRect.y-20, 3.6f, &sprite,false);
        }
    }

    void DebugDraw()
    {
        if (active)
        {
            app->render->DrawRectangle(bulletRect, { 20, 100, 181, 100 });
        }
    }
};

class Enemy : public Entity
{
public:
    Enemy();

    Enemy(EnemyClass enClass);

    virtual ~Enemy();

    void SetUp( SDL_Rect combatCollider, SDL_Rect worldCollider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity);

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

    Animation awakeAnim;
    Animation idleAnim;
    Animation attackAnim;
    Animation moveAnim;
    Animation bulletMantis;
};

#endif // __ENEMY_H__
