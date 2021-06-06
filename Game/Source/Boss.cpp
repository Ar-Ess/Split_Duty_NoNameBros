#include "Boss.h"
#include "App.h"
#include "Textures.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
}

Boss::Boss(BossClass bClass) : Entity(EntityType::BOSS)
{
	bossClass = bClass;
	// LIFE -> ((x / 1.5) + 20)
	// STRG -> ((x / 2) + 8)
	// DEFS -> ((x / 4) + 2) nose
	// EXPR -> expActLvl + expNextLvl / 2

	switch (bossClass)
	{
	case BOSS_TUTORIAL: 
		SetUp({BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H}, 1001, 6, 3, 0);
		bossSpritesheetTexture = app->tex->Load("Assets/Textures/Characters/Enemies/Bosses/golem_grassy_lands_spritesheet.png");
		break;
	case BOSS_I:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 129500, 44, 26, 11); // LVL 35
		bossSpritesheetTexture = app->tex->Load("Assets/Textures/Characters/Enemies/Bosses/golem_grassy_lands_spritesheet.png");
		break;
	case BOSS_II: 
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 278700, 63, 40, 18); // LVL 65
		bossSpritesheetTexture = app->tex->Load("Assets/Textures/Characters/Enemies/Bosses/golem_grassy_lands_spritesheet.png");
		break;
	case BOSS_III:
		SetUp({ BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H }, 418000, 155, 50, 24); // LVL 90
		bossSpritesheetTexture = app->tex->Load("Assets/Textures/Characters/Enemies/Bosses/golem_grassy_lands_spritesheet.png");
		break;
	}
	iPoint tile = { 100,100 };
	for (int i = 6; i < 10; i++)
	{
		idleAnim.PushBack({ i * tile.x,0,tile.x,tile.y });
	}
	for (int i = 2; i < 10; i++)
	{
		awakeAnim.PushBack({ i * tile.x,tile.y,tile.x,tile.y });
	}
	for (int i = 10; i >6; i++)
	{
		dieAnim.PushBack({ i * tile.x,tile.y*8,tile.x,tile.y });
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
