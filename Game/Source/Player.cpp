#include "Player.h"
#include "App.h"

#include "Scene.h"
#include "Combat.h"

#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER1)
{

}

Player::Player(SDL_Rect collCombat, EntityType enType) : Entity(enType)
{
    texture = NULL;
    type = enType;

    colliderCombat = collCombat;
    colliderWorld = { 1722, 210, PLAYER_WORLD_WIDTH, 84 };
    collisionRect = { 1722, 210 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };

    smallMeatCount = 1;
    largeMeatCount = 1;
    featherCount = 1;
    mantisRodCount = 1;
    splitedEnemyCount = 1;
    moneyCount = 1;

    playerSpeed = PLAYER_SPEED;

    if (enType == EntityType::PLAYER1)
    {
        colliderCombat.x = INIT_COMBAT_POSX;
        colliderCombat.y = INIT_COMBAT_POSY;
    }
    else if (enType == EntityType::PLAYER2)
    {
        colliderCombat.x = INIT2_COMBAT_POSX;
        colliderCombat.y = INIT2_COMBAT_POSY;
    }

    if (enType == EntityType::PLAYER1)
    {
        // Define Player animations
        for (int i = 0; i < 6; i++)
        {
            cIdleAnim.PushBack({ SPRITE_TILE_WIDTH * i,0 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
        }
        cIdleAnim.speed = 0.120f;

        for (int i = 0; i < 7; i++)
        {
            cAttackAnim.PushBack({ 154 * i,SPRITE_TILE_HEIGHT * 7 , 154 ,SPRITE_TILE_HEIGHT });

        }
        cAttackAnim.speed = 0.16f;
        cAttackAnim.loop = false;
        cAttackAnim.pingpong = false;

        for (int i = 0; i < 8; i++)
        {
            cJumpAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT * 5 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });

        }
        cJumpAnim.speed = 0.1f;

        for (int i = 0; i < 6; i++)
        {
            cStepAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
        }
        cStepAnim.speed = 0.190f;

        for (int i = 6; i > 0; i--)
        {
            // y: 600
            cScapeAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT *4,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
        }
        cScapeAnim.speed = 0.200f;

        for (int i = 0; i < 10; i++)
        {

            cDieAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT * 2,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });

        }
        cDieAnim.speed = 0.08f;
        cDieAnim.loop = false;

        for (int i = 0; i < 6; i++)
        {

            cCrouchAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT * 6,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });

        }
        cCrouchAnim.speed = 0.5f;
        cCrouchAnim.loop = false;
    }

    //world animations

    walkDownAnim.PushBack({ 28 * 3,5 * 3,20 * 3,28 * 3 });
    walkDownAnim.PushBack({ 52 * 3,5 * 3,20 * 3,30 * 3 });
    walkDownAnim.PushBack({ 28 * 3,5 * 3,20 * 3,28 * 3 });
    walkDownAnim.PushBack({ 3*3,5 * 3,20 * 3,28 * 3 });
    walkDownAnim.PushBack({ 28 * 3,5 * 3,20 * 3,28 * 3 });
   
    walkLeftAnim.PushBack({ 29 * 3,37 * 3,20 * 3,28 * 3 });
    walkLeftAnim.PushBack({ 53 * 3,37 * 3,20 * 3,26 * 3 });
    walkLeftAnim.PushBack({ 29 * 3,37 * 3,20 * 3,28 * 3 });
    walkLeftAnim.PushBack({ 4*3,37 * 3,20 * 3,28 * 3 });
    walkLeftAnim.PushBack({ 29 * 3,37 * 3,20 * 3,28 * 3 });
   
    walkRightAnim.PushBack({ 28 * 3,69 * 3,20 * 3,28 * 3 });
    walkRightAnim.PushBack({ 51 * 3,69 * 3,20 * 3,26 * 3 });
    walkRightAnim.PushBack({ 28 * 3,69 * 3,20 * 3,28 * 3 });
    walkRightAnim.PushBack({ 3 * 3,69 * 3,20 * 3,28 * 3 });
    walkRightAnim.PushBack({ 28 * 3,69 * 3,20 * 3,28 * 3 });
    
    walkUpAnim.PushBack({ 28 * 3,101 * 3,20 * 3,28 * 3 });
    walkUpAnim.PushBack({ 52 * 3,101 * 3,20 * 3,26 * 3 });
    walkUpAnim.PushBack({ 28 * 3,101 * 3,20 * 3,28 * 3 });
    walkUpAnim.PushBack({ 4 * 3,101 * 3,20 * 3,28 * 3 });
    walkUpAnim.PushBack({ 28 * 3,101 * 3,20 * 3,28 * 3 });
   

    walkLeftAnim.speed = walkRightAnim.speed = walkDownAnim.speed = walkUpAnim.speed = 0.30f;
    walkDownAnim.loop = walkUpAnim.loop = walkLeftAnim.loop = walkRightAnim.loop = false;
}

bool Player::Update(float dt)
{
    return true;
}

bool Player::Draw()
{
    return false;
}

void Player::SetUp(short int healthx, short int maxHealthx, short int strengthx, short int defensex, short int luckx, short int velocityx, short int stabx, short int lvlx, short int expx, short int moneyx)
{
    health = healthx;
    maxHealth = maxHealthx;
    strength = strengthx;
    defense = defensex;
    luck = luckx;
    velocity = velocityx;
    stab = stabx;
    lvl = lvlx;
    exp = expx;
    moneyCount = moneyx;
}

void Player::SetTexture(SDL_Texture *tex)
{
    texture = tex;
}

void Player::Jump()
{
    if (jumpTime < 34)
    {
        colliderCombat.y -= 17;
        colliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        if (type == EntityType::PLAYER1) colliderCombat.y = INIT_COMBAT_POSY;
        else if (type == EntityType::PLAYER2 && !app->scene->combatScene->secondPlayerProtection) colliderCombat.y = INIT2_COMBAT_POSY;
        jumpTime = 0;
        jump = false;
        if (type == EntityType::PLAYER1) app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
    }
}

void Player::FeatherJump()
{
    if (jumpFeatherTime <= 17)
    {
        colliderCombat.y -= 17;
        colliderCombat.y += jumpFeatherTime;
        jumpFeatherTime++;
    }
    else if (jumpFeatherTime > 17 && jumpFeatherTime < 70)
    {
        colliderCombat.y -= (17 + jumpFeatherTime - 20);
        colliderCombat.y += jumpFeatherTime;
        jumpFeatherTime++;
    }
    else
    {
        if (type == EntityType::PLAYER1) colliderCombat.y = INIT_COMBAT_POSY;
        else if (type == EntityType::PLAYER2 && !app->scene->combatScene->secondPlayerProtection) colliderCombat.y = INIT2_COMBAT_POSY;
        jumpFeatherTime = 0;
        jump = false;
        if (type == EntityType::PLAYER1) app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
    }
}

void Player::Crouch()
{
   if (crouchTime < 22)
   {
        if (crouchTime == 0)
        {
            colliderCombat.y += 40;
            colliderCombat.h = 48;
        }
        crouchTime++;
   }
   else
   {
       colliderCombat.y = INIT_COMBAT_POSY;
       colliderCombat.h = 88;
       crouchTime = 0;
       crouch = false;
       app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
   }
}

void Player::Refill()
{
    health = maxHealth;
}

void Player::RestartPlayer()
{
    app->scene->player1->colliderWorld = { 1722, 210, PLAYER_WORLD_WIDTH, 84 };
    app->scene->player1->collisionRect = { 1722, 210 + 56, PLAYER_WORLD_WIDTH, 84 - 56 };

    smallMeatCount = 1;
    largeMeatCount = 1;
    featherCount = 1;
    mantisRodCount = 1;
    splitedEnemyCount = 1;
    moneyCount = 1;

    if (type == EntityType::PLAYER1)
    {
        SetUp(40, 40, 27, 8, 0, 0, 0, 10, 0, 0);
        colliderCombat.x = INIT_COMBAT_POSX;
        colliderCombat.y = INIT_COMBAT_POSY;
    }
    else if (type == EntityType::PLAYER2)
    {
        SetUp(15, 15, 15, 0, 0, 0, 0, 10, 0, 0);
        colliderCombat.x = INIT2_COMBAT_POSX;
        colliderCombat.y = INIT2_COMBAT_POSY;
    }

    //app->scene->combatScene->secondPlayer = false;

    playerSpeed = PLAYER_SPEED;
}

void Player::ResetWalkingAnimation()
{
    walkRightAnim.Reset();
    walkLeftAnim.Reset();
    walkDownAnim.Reset();
    walkUpAnim.Reset();
}
