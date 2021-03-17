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

	playerScape = false;
	playerResponseAble = true;
	playerChoice = true;

	steps = 0;

	FirstTurnLogic();
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
		EnemyAttack();

		PlayerResponse();

		if (enemy->colliderCombat.x < app->scene->player1->playerColliderCombat.x - enemy->colliderCombat.w) playerResponseAble = false;
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

void Combat::EnemyAttack()
{
	if (enemyTimeAttack < 268)
	{
		enemy->colliderCombat.x -= 5;
		enemyTimeAttack++;

		if (enemy->colliderCombat.x + enemy->colliderCombat.w < 0) enemy->colliderCombat.x = 1280;
	}
	else
	{
		enemyTimeAttack = 0;
		enemy->colliderCombat.x = INIT_ENEMY1_POSX;
		combatState = PLAYER_TURN;
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
		playerTimeAttack = 0;
		app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
		combatState = ENEMY_TURN;
		playerAttack = false;
		playerResponseAble = true;
		playerChoice = true;
		steps = 0;
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