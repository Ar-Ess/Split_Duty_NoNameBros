#include "Boss.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
}

Boss::Boss(BossClass bClass) : Entity(EntityType::BOSS)
{
	bossClass = bClass;
	// LIFE -> ((x / 1.5) + 20)
	// STRG -> ((x / 2.3) + 6)
	// DEFS -> ((x / 6) + 2) nose

	switch (bossClass)
	{
	case BOSS_TUTORIAL: 
		SetUp({BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H}, 0, 10, 3, 2);
		break;
	case BOSS_I:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 0, 44, 21, 8); // LVL 35
		break;
	case BOSS_II: 
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 0, 60, 30, 12); // LVL 60
		break;
	case BOSS_III:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 0, 155, 42, 17); // LVL 90
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
