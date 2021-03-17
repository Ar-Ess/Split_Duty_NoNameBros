#include "Player.h"

Player::Player() : Entity(EntityType::PLAYER)
{
    texture = NULL;

    playerColliderCombat = {INIT_COMBAT_POSX, INIT_COMBAT_POSY, 48, 88};

    // Define Player animations
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
        playerColliderCombat.y -= 17;
        playerColliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        playerColliderCombat.y = INIT_COMBAT_POSY;
        jumpTime = 0;
        jump = false;
    }
}

void Player::Crouch()
{
    if (crouchTime < 40)
    {
        if (crouchTime == 0)
        {
            playerColliderCombat.y += 40;
            playerColliderCombat.h = 48;
        }
        crouchTime++;
    }
    else
    {
        playerColliderCombat.y = INIT_COMBAT_POSY;
        playerColliderCombat.h = 88;
        crouchTime = 0;
        crouch = false;
    }
}
