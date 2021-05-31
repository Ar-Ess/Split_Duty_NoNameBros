#include "Boss.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
}

Boss::Boss(BossClass bClass) : Entity(EntityType::BOSS)
{
	bossClass = bClass;
	// LIFE -> ((x / 1.5) + 20)
	// STRG -> ((x / 2) + 8)
	// DEFS -> ((x / 4) + 2) nose

	switch (bossClass)
	{
	case BOSS_TUTORIAL: 
		SetUp({BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H}, 100, 10, 3, 2);
		break;
	case BOSS_I:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 129500, 44, 26, 11); // LVL 35
		break;
	case BOSS_II: 
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 252227, 60, 38, 17); // LVL 60
		break;
	case BOSS_III:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 0, 155, 53, 25); // LVL 90
		break;
	}
}

Boss::~Boss()
{
}

void Boss::SetUp(SDL_Rect combatCollider, int xexp, int xhealth, int xstrength, int xdefense)
{
	colliderCombat = combatCollider;
	exp = xexp;
	health = xhealth;
	maxHealth = xhealth;
	strength = xstrength;
	defense = xdefense;
}

void Boss::Refill()
{
	health = maxHealth;
}
