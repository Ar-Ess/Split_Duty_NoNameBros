#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Animation.h"
#include "Render.h"
#include "Textures.h"
#include "Collider.h"

#include "SDL/include/SDL.h"

#define BOSS_C_X 1004
#define BOSS_C_Y 413
#define BOSS_C_W 56
#define BOSS_C_H 75

struct SDL_Rect;
struct Animation;
struct SDL_Texture;

enum BossClass
{
	BOSS_TUTORIAL,
	BOSS_I,
	BOSS_II,
	BOSS_III
};

class Boss : public Entity
{
public:
	Boss();

	Boss(BossClass bClass);

	virtual ~Boss();

	void SetUp(SDL_Rect combatCollider, int xexp, int xhealth, int xstrength, int xdefense);

	void Refill();

	BossClass GetClass() const
	{
		return bossClass;
	}

public:
	SDL_Rect colliderCombat;

	bool jumping = false;
	bool active = true;

public:
	friend class Combat;

	int health;
	int maxHealth;
	int defense;
	int strength;
	int exp;

	short int attack = 0;

	BossClass bossClass;

public:

	Animation awakeAnim;
	Animation idleAnim;
	Animation attackAnim;
	Animation moveAnim;
	Animation bulletMantis;
};

#endif // __BOSS_H__

