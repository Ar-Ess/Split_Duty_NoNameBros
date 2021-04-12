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
    colliderWorld = {60, 150, 56, 84};
    collisionRect = {60, 206, 56, 28};

    playerSpeed = 18;

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

        int j = 0;
        for (int i = 0; i < 10; i++)
        {
            cPos0AttackAnim.PushBack({ 0, 0 + (i * 720),1280,720 });
        }
        for (int i = 0; i < 7; i++)
        {
            cPos0AttackAnim.PushBack({ 1280, 0 + (i * 720),1280,720 });
        }
        cPos0AttackAnim.speed = 0.240f;
        cPos0AttackAnim.loop = false;
        cPos0AttackAnim.pingpong = false;

        j = 0;
        for (int i = 0; i < 10; i++)
        {
            cPos1AttackAnim.PushBack({ 0, 0 + (i * 720),1280,720 });
        }
        for (int i = 0; i < 7; i++)
        {
            cPos1AttackAnim.PushBack({ 1280, 0 + (i * 720),1280,720 });
        }
        cPos1AttackAnim.speed = 0.240f;
        cPos1AttackAnim.loop = false;
        cPos1AttackAnim.pingpong = false;

        j = 0;
        for (int i = 0; i < 10; i++)
        {
            cPos2AttackAnim.PushBack({ 0, 0 + (i * 720),1280,720 });
        }
        for (int i = 0; i < 7; i++)
        {
            cPos2AttackAnim.PushBack({ 1280, 0 + (i * 720),1280,720 });
        }
        cPos2AttackAnim.speed = 0.240f;
        cPos2AttackAnim.loop = false;
        cPos2AttackAnim.pingpong = false;

        j = 0;
        for (int i = 0; i < 10; i++)
        {
            cPos3AttackAnim.PushBack({ 0, 0 + (i * 720),1280,720 });
        }
        for (int i = 0; i < 7; i++)
        {
            cPos3AttackAnim.PushBack({ 1280, 0 + (i * 720),1280,720 });
        }
        cPos3AttackAnim.speed = 0.240f;
        cPos3AttackAnim.loop = false;
        cPos3AttackAnim.pingpong = false;

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
}

bool Player::Update(float dt)
{
    return true;
}

bool Player::Draw()
{
    return false;
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
        colliderCombat.y = INIT_COMBAT_POSY;
        jumpTime = 0;
        jump = false;
        app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
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
        colliderCombat.y = INIT_COMBAT_POSY;
        jumpFeatherTime = 0;
        jump = false;
        app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
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
