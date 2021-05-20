#include "Boss.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
}

Boss::Boss(BossClass bClass) : Entity(EntityType::BOSS)
{
	bossClass = bClass;

	switch (bossClass)
	{
	case BOSS_TUTORIAL: 
		SetUp({BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H}, 0, 10, 3, 2);
		break;
	case BOSS_I: break;
	case BOSS_II: break;
	case BOSS_III: break;
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
