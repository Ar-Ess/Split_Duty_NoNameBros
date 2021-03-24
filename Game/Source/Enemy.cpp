#include "App.h"
#include "Scene.h"+
#include "Combat.h"

#include "Enemy.h"

#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));
}

Enemy::~Enemy()
{
}

void Enemy::SetUp(EnemyClass xenemyClass, SDL_Rect collider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity)
{
    enemyClass = xenemyClass;
    colliderCombat = collider;
    lvl = xlvl;
    exp = xexp;
    health = xhealth;
    maxHealth = xhealth;
    strength = xstrength;
    defense = xdefense;
    velocity = xvelocity;
}

void Enemy::Jump()
{
    if (jumpTime < 28)
    {
        colliderCombat.y -= 14;
        colliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        colliderCombat.y = INIT_SMALL_WOLF_POSY;
        jumpTime = 0;
    }
}

void Enemy::SmallWolfAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        colliderCombat.x -= 6;
        smallWolfTimeAttack1++;

        if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;
    }
    else if (typeOfAttack == 2)
    {
        if (smallWolfTimeAttack2 < 29)
        {
            Jump();
            smallWolfTimeAttack2++;
        }
        else if (smallWolfTimeAttack2 < 50)
        {
            smallWolfTimeAttack2++;
        }
        else if (smallWolfTimeAttack2 < 274)
        {
            if (app->scene->combatScene->steps == 0)
            {
                colliderCombat.x -= 6;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 165 && smallWolfTimeAttack2 < 195) Jump();
            }
            else if (app->scene->combatScene->steps == 1)
            {
                colliderCombat.x -= 6;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 136 && smallWolfTimeAttack2 < 166) Jump();
            }
            else if (app->scene->combatScene->steps == 2)
            {
                colliderCombat.x -= 6;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 107 && smallWolfTimeAttack2 < 137) Jump();
            }
            else if (app->scene->combatScene->steps == 3)
            {
                colliderCombat.x -= 6;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 78 && smallWolfTimeAttack2 < 108) Jump();
            }

            smallWolfTimeAttack2++;
        }
    }
}

void Enemy::BirdAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        if (app->scene->combatScene->steps == 0)
        {
            if (birdTimeAttack1 < 95)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 110)
            {
                colliderCombat.x -= 6;
                colliderCombat.y -= 6;
            }
            else if (birdTimeAttack1 < 125)
            {
                colliderCombat.x += 6;
                colliderCombat.y -= 6;
            }
            else if (birdTimeAttack1 < 140)
            {
                colliderCombat.x += 6;
                colliderCombat.y += 6;
            }
            else if (birdTimeAttack1 < 155)
            {
                colliderCombat.x -= 6;
                colliderCombat.y += 6;
            }

            birdTimeAttack1++;
        }
        if (app->scene->combatScene->steps == 1)
        {
            if (birdTimeAttack1 < 75)
            {
                colliderCombat.x -= 8;
            }
            birdTimeAttack1++;
        }
        if (app->scene->combatScene->steps == 2)
        {
            if (birdTimeAttack1 < 51)
            {
                colliderCombat.x -= 8;
            }
            birdTimeAttack1++;
        }
        if (app->scene->combatScene->steps == 3)
        {
            if (birdTimeAttack1 < 29)
            {
                colliderCombat.x -= 8;
            }
            birdTimeAttack1++;
        }
    }
    else if (typeOfAttack == 2)
    {

    }
}


