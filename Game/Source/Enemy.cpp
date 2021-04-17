#include "App.h"
#include "Scene.h"+
#include "Combat.h"

#include "Enemy.h"

#include "Pathfinding.h"

#include "Log.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));

   

}

Enemy::Enemy (EnemyClass enClass) : Entity(EntityType::ENEMY)
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));

    //Load enemies textures
    //wolf
    enemyClass = enClass;

    switch (enClass)
    {
    case(EnemyClass::SMALL_WOLF):

        awakeAnim.PushBack({ 68,0,63,34 });
        awakeAnim.PushBack({ 133 ,0,63,34 });
        awakeAnim.PushBack({ 197 ,0,63,34 });
        awakeAnim.PushBack({ 262,0,63,34 });
        

        moveAnim.PushBack({ 0,132,62,34 });
        moveAnim.PushBack({ 0,132,62,34 });

        break;
    case(EnemyClass::BIRD):

        idleAnim.PushBack({ 32,0,32,32 });
        idleAnim.PushBack({ 32 * 2,0,32,32 });
        idleAnim.PushBack({ 32 * 3,0,32,32 });

        break;
    case(EnemyClass::MANTIS):

        idleAnim.PushBack({ 0,0,28,31 });
        idleAnim.PushBack({ 28,0,35,31 });
        idleAnim.PushBack({ 63,0,37,31 });
        
        break;
    }
    awakeAnim.loop = true;
    awakeAnim.speed = 0.1f;
    
    idleAnim.loop = true;
    idleAnim.speed = 0.1f;
    

  
    
}

Enemy::~Enemy()
{
}

void Enemy::SetUp( SDL_Rect combatCollider, SDL_Rect worldCollider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity)
{
    
    colliderCombat = combatCollider;
    colliderWorld = worldCollider;
    colliderRect = {worldCollider.x, worldCollider.y + worldCollider.h - 28, worldCollider.w, 28};
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
        colliderCombat.y = INIT_SMALLWOLF_POSY;
        jumpTime = 0;
    }
}

void Enemy::HighJump()
{
    if (jumpTime < 34)
    {
        colliderCombat.y -= 17;
        colliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        colliderCombat.y = INIT_MANTIS_POSY;
        jumpTime = 0;
    }
}

void Enemy::SmallWolfAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        colliderCombat.x -= 8;
        smallWolfTimeAttack1++;

        if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;
    }
    else if (typeOfAttack == 2)
    {
        if (smallWolfTimeAttack2 < 29)
        {
            Jump();
        }
        else if (smallWolfTimeAttack2 < 50)
        {
            int a = 0;
        }
        else if (smallWolfTimeAttack2 < 220)
        {
            if (app->scene->combatScene->steps == 0)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 135 && smallWolfTimeAttack2 < 165) Jump();
            }
            else if (app->scene->combatScene->steps == 1)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 113 && smallWolfTimeAttack2 < 143) Jump();
            }
            else if (app->scene->combatScene->steps == 2)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 90 && smallWolfTimeAttack2 < 120) Jump();
            }
            else if (app->scene->combatScene->steps == 3)
            {
                colliderCombat.x -= 8;

                if (colliderCombat.x + colliderCombat.w < 0) colliderCombat.x = 1280;

                if (smallWolfTimeAttack2 > 68 && smallWolfTimeAttack2 < 98) Jump();
            }
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
            else if (birdTimeAttack1 < 120) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 155) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 180) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 179) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 215) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 248)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 1)
        {
            if (birdTimeAttack1 < 75)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 100) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 135) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 160) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 160) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 195) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 228)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 2)
        {
            if (birdTimeAttack1 < 51)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 76) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 111) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 136) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 135) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 171) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 204)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }
        if (app->scene->combatScene->steps == 3)
        {
            if (birdTimeAttack1 < 31)
            {
                colliderCombat.x -= 8;
            }
            else if (birdTimeAttack1 < 56) // \*
            {
                colliderCombat.x -= 5;
                colliderCombat.y -= 5;
            }
            else if (birdTimeAttack1 < 91) // /*
            {
                colliderCombat.x += 9;
                colliderCombat.y -= 2;
            }
            else if (birdTimeAttack1 < 116) // \_
            {
                colliderCombat.x += 5;
                colliderCombat.y += 5;
                if (birdTimeAttack1 == 115) app->scene->combatScene->playerHitAble = true;
            }
            else if (birdTimeAttack1 < 151) // _/
            {
                colliderCombat.x -= 9;
                colliderCombat.y += 4;
            }
            else if (birdTimeAttack1 < 184)
            {
                colliderCombat.x -= 9;
                colliderCombat.y -= 4;
            }
            else if (birdTimeAttack1 < 280)
            {
                colliderCombat.x -= 8;
            }
        }

        birdTimeAttack1++;

        if (colliderCombat.x + colliderCombat.w < 0)
        {
            colliderCombat.x = 1280;
            colliderCombat.y = INIT_BIRD_POSY;
        }
    }
    else if (typeOfAttack == 2)
    {
        if (birdTimeAttack2 < 30)
        {
            colliderCombat.x += 3;
        }
        else if (birdTimeAttack2 < 50)
        {
            colliderCombat.x += 0;
        }
        else if (birdTimeAttack2 < 75)
        {
            colliderCombat.x -= 12;
            colliderCombat.y += 3;
        }
        else if (birdTimeAttack2 < 150)
        {
            colliderCombat.x -= 12;
            if (birdTimeAttack2 == 149)
            {
                app->scene->combatScene->playerResponseAble = true;
                app->scene->combatScene->playerHitAble = true;
            }
        }
        else if (birdTimeAttack2 < 168)
        {
            colliderCombat.y--;
            colliderCombat.x -= 12;
        }
        else if (birdTimeAttack2 < 280)
        {
            colliderCombat.x -= 12;
        }

        birdTimeAttack2++;

        if (colliderCombat.x + colliderCombat.w < 0)
        {
            colliderCombat.x = 1280;
            colliderCombat.y = INIT_BIRD_POSY;
        }
    }
}

void Enemy::MantisAttack(unsigned short int typeOfAttack)
{
    if (typeOfAttack == 1)
    {
        if (mantisTimeAttack1 == 0) for (int i = 0; i < 5; i++) bullet[i].BulletReset();

        if (mantisTimeAttack1 < 35)
        {
            MantisAttack1Logic(35);
        }
        else if (mantisTimeAttack1 < 70)
        {
            MantisAttack1Logic(70);
        }
        else if (mantisTimeAttack1 < 105)
        {
            MantisAttack1Logic(105);
        }
        else if (mantisTimeAttack1 < 140)
        {
            MantisAttack1Logic(140);
        }
        else if (mantisTimeAttack1 < 175)
        {
            MantisAttack1Logic(175);
        }
        else if (mantisTimeAttack1 < 280)
        {
            if (mantisTimeAttack1 == 279) for (int i = 0; i < 5; i++) bullet[i].BulletReset();
        }

        //DRAW & UPDATE OF BULLETS
        for (int i = 0; i < 5; i++) bullet[i].Update();
    }
    else if (typeOfAttack == 2)
    {
        if (mantisTimeAttack2 < 92)
        {
            colliderCombat.x += 3;
        }
        else if (mantisTimeAttack2 > 140 && mantisTimeAttack2 < 202)
        {
            colliderCombat.x -= 22;
        }
        else if (mantisTimeAttack2 >= 202 && mantisTimeAttack2 < 220)
        {
            colliderCombat.x = 325;
            colliderCombat.y = -75;
            app->scene->combatScene->playerHitAble = true;
        }
        else if (mantisTimeAttack2 >= 220 && mantisTimeAttack2 < 242)
        {
            colliderCombat.x += 24;
            colliderCombat.y += 22;
        }
        else if (mantisTimeAttack2 == 242)
        {
            colliderCombat.y = INIT_MANTIS_POSY;
            colliderCombat.x += 22;
        }
        else if (mantisTimeAttack2 > 242 && mantisTimeAttack2 < 270)
        {
            int move = (20 - ((mantisTimeAttack2 - 242) * 2));

            if (move <= 0) move = 2;

            colliderCombat.x += move;
        }
        else if (mantisTimeAttack2 == 270) colliderCombat.x = INIT_MANTIS_POSX;
    }
    else if (typeOfAttack == 3)
    {
        if (mantisTimeAttack3 < 5)
        {
            colliderCombat.x -= 2;
        }
        else if (mantisTimeAttack3 < 10)
        {
            colliderCombat.x += 2;
        }
        else if (mantisTimeAttack3 < 15)
        {
            colliderCombat.x -= 2;
        }
        else if (mantisTimeAttack3 < 20)
        {
            colliderCombat.x += 2;
        }
    }
}

void Enemy::MantisAttack1Logic(unsigned short int timer)
{
    if (mantisTimeAttack1 == (timer - 35))
    {
        int random = rand() % 2;
        jumping = bool(random);
    }

    if (mantisTimeAttack1 == (timer - 25))
    {
        int i = (timer / 35) - 1;
        bullet[i].active = true;
        if (jumping) bullet[i].bulletRect.y = 338;
    }

    if (jumping) HighJump();
    else if (jumping) jumpTime = 0;
}
