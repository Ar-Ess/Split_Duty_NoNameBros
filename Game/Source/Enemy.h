#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

#include "DynArray.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Rect;

class Enemy : public Entity
{
public:

    Enemy();

    virtual ~Enemy();

    void SetUp(SDL_Rect collider, int xlvl, int xexp, int xhealth, int xstrength, int xdefense, int xvelocity);

public:
    SDL_Rect colliderCombat;

private:
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

};

#endif // __ENEMY_H__
