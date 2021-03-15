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
