#include "App.h"

#include "Scene.h"

#include "Combat.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"

#include "Log.h"

Combat::Combat()
{

}

void Combat::Start()
{
	//Enemy HardCoded
	enemy->SetUp({ INIT_ENEMY1_POSX, INIT_ENEMY1_POSY, 48, 88 });
	enemy->health = 30;
	enemy->defense = 10;
	enemy->strength = 30;
	enemy->velocity = 20;

	//Player HardCoded
	app->scene->player1->health = 35;
	app->scene->player1->strength = 30;
	app->scene->player1->defense = 5;
	app->scene->player1->luck = 0;
	app->scene->player1->velocity = 0;
	app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
	app->scene->player1->playerColliderCombat.y = INIT_COMBAT_POSY;

	playerScape = false;
	playerResponseAble = true;
	playerHitAble = true;
	playerChoice = true;
	steps = 0;


	FirstTurnLogic();

	// LuckArray fill
	int pLuck = app->scene->player1->luck;
	if (pLuck > 0)
	{
		for (int i = 0; i < 100; i++)
		{
			int trufals = i - pLuck;

			if (trufals < 0) luckArray[i] = true;
			else if (trufals >= 0) luckArray[i] = false;
		}
	}
}

void Combat::Restart()
{
	combatState = NULL_STATE;
	enemy = nullptr;
}

void Combat::Update()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeWait < 60) enemyTimeWait++; // Make enemy wait so it does not atack directly
		else
		{
			EnemyAttack();

			PlayerResponse();

			if (enemy->colliderCombat.x < app->scene->player1->playerColliderCombat.x - enemy->colliderCombat.w) playerResponseAble = false;
		}
	}
	else if (combatState == PLAYER_TURN)
	{
		if (playerChoice)
		{
			PlayerChoiceLogic();
		}

		if (playerAttack)
		{
			PlayerAttack();
		}

		if (playerStep)
		{
			PlayerMove();
		}
	}
	else if (combatState == WIN)
	{
		playerScape = true; //Provisional, will lead to win animation and level upgrade
	}
	else if (combatState == LOSE)
	{
		playerScape = true; //Provisional, will lead to lose animation and respawn
	}
}

void Combat::FirstTurnLogic()
{
	if (app->scene->player1->velocity <= enemy->velocity) combatState = ENEMY_TURN;
	else
	{
		playerChoice = true;
		combatState = PLAYER_TURN;
	}
}

void Combat::PlayerChoiceLogic()
{
	if (app->scene->attackPressed)
	{
		playerAttack = true;
		playerChoice = false;
	}
	else if (app->scene->scapePressed)
	{
		playerScape = true;
		playerChoice = false;
		return;
	}
	else if (app->scene->movePressed && steps < 3)
	{
		playerStep = true;
		playerChoice = false;
		steps++;
		return;
	}
}

int Combat::PlayerDamageLogic()
{
	int damage = 0;
	int pDamage = app->scene->player1->strength - enemy->defense;
	int pLuck = app->scene->player1->luck;

	if (steps == 0) damage += floor(25 * pDamage / 100);
	else if (steps == 1) damage += floor(50 * pDamage / 100);
	else if (steps == 2) damage += floor(75 * pDamage / 100);
	else if (steps == 3) damage += pDamage;

	if (damage < 1) damage = 1;

	if (pLuck == 0) return damage;
	else if (pLuck > 0)
	{
		if (pLuck > 25) pLuck = 25;
		int a = rand() % 100;
		if (luckArray[a]) return damage + floor(25 * damage / 100);
		else if (!luckArray[a]) return damage;
	}
}

int Combat::EnemyDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defense;

	if (damage < 1) damage = 1;

	return damage;
}

void Combat::EnemyAttack()
{
	if (enemyTimeAttack < 268)
	{
		enemy->colliderCombat.x -= 5;
		enemyTimeAttack++;

		if (enemy->colliderCombat.x + enemy->colliderCombat.w < 0) enemy->colliderCombat.x = 1280;

		if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->playerColliderCombat, enemy->colliderCombat))
		{
			playerHitAble = false;
			app->scene->player1->health -= EnemyDamageLogic();
		}
	}
	else
	{
		enemyTimeAttack = 0;
		enemyTimeWait = 0;
		enemy->colliderCombat.x = INIT_ENEMY1_POSX;
		playerHitAble = true;

		if (app->scene->player1->health > 0) combatState = PLAYER_TURN;
		else if (app->scene->player1->health <= 0) combatState = LOSE;
	}
}

void Combat::PlayerAttack()
{
	if (playerTimeAttack < 125)
	{
		app->scene->player1->playerColliderCombat.x += 7;
		playerTimeAttack++;
	}
	else
	{
		enemy->health -= PlayerDamageLogic();
		LOG("Enemy Health: %d", enemy->health);

		playerTimeAttack = 0;
		app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;

		playerAttack = false;
		playerResponseAble = true;
		playerChoice = true;
		steps = 0;

		if (enemy->health > 0) combatState = ENEMY_TURN;
		else if (enemy->health <= 0)
		{
			combatState = WIN;
		}
	}
}

void Combat::PlayerMove()
{
	if (playerTimeMove < 57)
	{
		app->scene->player1->playerColliderCombat.x += 3;
		playerTimeMove++;
	}
	else
	{
		playerTimeMove = 0;
		//app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
		combatState = ENEMY_TURN;
		playerStep = false;
		playerResponseAble = true;
		playerChoice = true;
	}
}

void Combat::PlayerResponse()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->player1->jump && playerResponseAble) app->scene->player1->jump = true;

	if (app->scene->player1->jump) app->scene->player1->Jump();
}