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

void Enemy::SetUp(SDL_Rect collider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity)
{
    colliderCombat = collider;
    lvl = xlvl;
    exp = xexp;
    health = xhealth;
    strength = xstrength;
    defense = xdefense;
    velocity = xvelocity;
}
