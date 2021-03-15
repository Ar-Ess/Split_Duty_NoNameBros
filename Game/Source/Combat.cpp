#include "App.h"

#include "Scene.h"

#include "Combat.h"
#include "Enemy.h"
#include "Player.h"



Combat::Combat()
{

}

void Combat::Start()
{
	enemy->SetUp({ INIT_ENEMY1_POSX, INIT_ENEMY1_POSY, 48, 88 });
	enemy->health = 30;
	enemy->defense = 15;
	enemy->strength = 40;
	enemy->velocity = 20;
	FirstTurnLogic();
}

void Combat::Restart()
{

}

void Combat::Update()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeAttack < 200)
		{
			enemy->colliderCombat.x -= 4;
			enemyTimeAttack++;
		}
		else
		{
			enemyTimeAttack = 0;
			enemy->colliderCombat.x = INIT_ENEMY1_POSX;
			combatState = PLAYER_TURN;
		}
	}
	else if (combatState == PLAYER_TURN)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) playerAttack = true;

		if (playerAttack)
		{
			if (playerTimeAttack < 125)
			{
				app->scene->player1->playerColliderCombat.x += 7;
				playerTimeAttack++;
			}
			else
			{
				playerTimeAttack = 0;
				app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
				combatState = ENEMY_TURN;
				playerAttack = false;
			}
		}
	}
}

void Combat::FirstTurnLogic()
{
	if (app->scene->player1->velocity <= enemy->velocity) combatState = ENEMY_TURN;
	else
	{
		combatState = PLAYER_TURN;
	}
}
