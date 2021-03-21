#include "Player.h"

Player::Player() : Entity(EntityType::PLAYER)
{
    texture = NULL;

    colliderCombat = {INIT_COMBAT_POSX, INIT_COMBAT_POSY, 48, 88};

    // Define Player animations
    for (int i = 0; i < 6; i++)
    {
        cIdleAnim.PushBack(spriteSize);
        spriteSize.x += SPRITE_TILE_SIZE;
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
    }
}
