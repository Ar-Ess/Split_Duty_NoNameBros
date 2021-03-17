#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Input.h"
#include "Render.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

#define INIT_COMBAT_POSX 225
#define INIT_COMBAT_POSY 400

enum class PlayerAnim
{
    IDLE,
    STEP,
    JUMP
};

class Player : public Entity
{
public:

    Player();

    bool Update(float dt);

    bool Draw();

    void SetTexture(SDL_Texture *tex);

    void Jump();

    void Crouch();

    SDL_Rect GetBounds();

    int GetCombatWidth() const
    {
        return playerColliderCombat.w;
    }

    int GetCombatHeight() const
    {
        return playerColliderCombat.h;
    }

    int GetLiveWidth() const
    {
        return playerColliderLive.w;
    }

    int GetLiveHeight() const
    {
        return playerColliderLive.h;
    }

public:

    SDL_Texture* texture;

    PlayerAnim currentAnim;

    bool jump = false;
    bool crouch = false;
    int jumpTime = 0;
    int crouchTime = 0;

private:
    friend class Combat;

    int health = 35;
    int maxHealth = 35;
    int defense = 5;
    int strength = 30;
    int velocity = 0;
    int luck = 0;
    int stab;
    int exp;

public:
    iPoint positionLive;
    int positionCombat;

    SDL_Rect playerColliderLive;
    SDL_Rect playerColliderCombat;
};

#endif // __PLAYER_H__
