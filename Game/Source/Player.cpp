#include "Player.h"
#include "App.h"

#include "Scene.h"
#include "Combat.h"

#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
    texture = NULL;

    colliderCombat = {INIT_COMBAT_POSX, INIT_COMBAT_POSY, 48, 88};

    // Define Player animations
    for (int i = 0; i < 6; i++)
    {
        cIdleAnim.PushBack({ (i * SPRITE_TILE_WIDTH) + SPRITE_MARGIN + (i * 2), SPRITE_MARGIN ,70,88 });
    }
    cIdleAnim.speed = 0.120f;

    for (int i = 0; i < 6; i++)
    {
        cStepAnim.PushBack({ (i * SPRITE_TILE_WIDTH) + SPRITE_MARGIN , SPRITE_TILE_HEIGHT * 1,70,88 });
    }
    cStepAnim.speed = 0.190f;

    for (int i = 0; i < 8; i++)
    {
        cJumpAnim.PushBack({ (i * SPRITE_TILE_WIDTH) + SPRITE_MARGIN , SPRITE_TILE_HEIGHT * 5,70,88 });
       /* int y = SPRITE_TILE_HEIGHT + SPRITE_MARGIN;
        LOG("y : %d", y);*/
    }
    cJumpAnim.speed = 0.1f;

    for (int i = 0; i < 10; i++)
    {

        cDieAnim.PushBack({ (i * SPRITE_TILE_WIDTH) + SPRITE_MARGIN , 310,70,88 });
        int y = SPRITE_TILE_HEIGHT * 2;
        LOG("y : %d", y);
    }
    cDieAnim.speed = 0.08f;
    cDieAnim.loop = true;
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
        app->scene->combatScene->currPlayerAnim = &cIdleAnim;
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
        app->scene->combatScene->currPlayerAnim = &cIdleAnim;
    }
}

void Player::Crouch()
{
    if (crouchTime < 40)
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
        app->scene->combatScene->currPlayerAnim = &cIdleAnim;
    }
}
