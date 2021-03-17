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
	enemy->SetUp({ INIT_ENEMY1_POSX, INIT_ENEMY1_POSY, 70, 55 });
	enemy->health = 30;
	enemy->defense = 10;
	enemy->strength = 30;
	enemy->velocity = 20;

	//Player HardCoded
	app->scene->player1->health = 35;
	app->scene->player1->strength = 30;
	app->scene->player1->defense = 5;
	app->scene->player1->luck = 0;
	app->scene->player1->velocity = 30;
	app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
	app->scene->player1->playerColliderCombat.y = INIT_COMBAT_POSY;

	//Item quantity (hardcoded for the moment)
	smallMeat = 1;
	largeMeat = 1;
	feather = 1;
	mantisLeg = 1;

	playerScape = false;
	playerAttack = false;
	playerItem = false;
	playerStep = false;
	playerResponseAble = true;
	playerHitAble = true;
	playerChoice = true;
	steps = 0;

	itemChoice = true;
	healPlayerSmall = false;


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

	LOG("PH: %d", app->scene->player1->health);
	LOG("EH: %d", enemy->health);
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

			if (enemy->colliderCombat.x < app->scene->player1->playerColliderCombat.x - enemy->colliderCombat.w - 50) playerResponseAble = false;
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

		if (playerItem)
		{
			PlayerItemChoose();
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
	else if (app->scene->movePressed && steps < 3)
	{
		playerStep = true;
		playerChoice = false;
		steps++;
		return;
	}
	else if (app->scene->itemPressed)
	{
		playerItem = true;
		playerChoice = false;
		return;
	}
	else if (app->scene->scapePressed)
	{
		playerScape = true;
		playerChoice = false;
		return;
	}
}

int Combat::PlayerDamageLogic()
{
	int damage = 0;
	int pDamage = app->scene->player1->strength - enemy->defense;
	int pLuck = app->scene->player1->luck;

	if (steps == 0) damage += floor(15 * pDamage / 100);
	else if (steps == 1) damage += floor(35 * pDamage / 100);
	else if (steps == 2) damage += floor(65 * pDamage / 100);
	else if (steps == 3) damage += pDamage;

	if (damage < 1) //Normal enemy 0 damage, Boss 1 damage (for speedrunners) | To implement
	{
		damage = 0;
		return damage;
	}

	//Set luck
	if (pLuck == 0) return damage;
	else if (pLuck > 0)
	{
		if (pLuck > 25) pLuck = 25;
		int a = rand() % 100;
		if (luckArray[a]) return damage + floor(20 * (pDamage - enemy->defense) / 100);
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
	if (enemyTimeAttack < 225)
	{
		enemy->colliderCombat.x -= 6;
		enemyTimeAttack++;

		if (enemy->colliderCombat.x + enemy->colliderCombat.w < 0)enemy->colliderCombat.x = 1280;

		if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->playerColliderCombat, enemy->colliderCombat))
		{
			playerHitAble = false;
			app->scene->player1->health -= EnemyDamageLogic();
			LOG("Player Hit - PH: %d", app->scene->player1->health);
		}
	}
	else
	{
		enemyTimeAttack = 0;
		enemyTimeWait = 0;
		enemy->colliderCombat.x = INIT_ENEMY1_POSX;
		playerHitAble = true;

		if (app->scene->player1->health > 0)
		{
			LOG("PLAYER TURN");
			LOG("Player Health: %d", app->scene->player1->health);
			combatState = PLAYER_TURN;
		}
		else if (app->scene->player1->health <= 0)
		{
			LOG("PLAYER LOSE");
			combatState = LOSE;
		}
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
		LOG("Enemy Hit, EH: %d", enemy->health);

		playerTimeAttack = 0;
		app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;

		playerAttack = false;
		playerResponseAble = true;
		playerChoice = true;
		steps = 0;

		if (enemy->health > 0)
		{
			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
			combatState = ENEMY_TURN;
		}
		else if (enemy->health <= 0)
		{
			LOG("PLAYER WIN");
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
		LOG("ENEMY TURN");
		LOG("Enemy Health: %d", enemy->health);
		playerTimeMove = 0;
		//app->scene->player1->playerColliderCombat.x = INIT_COMBAT_POSX;
		combatState = ENEMY_TURN;
		playerStep = false;
		playerResponseAble = true;
		playerChoice = true;
	}
}

void Combat::PlayerItemChoose()
{
	if (itemChoice)
	{
		app->render->DrawRectangle(inventorySimulation, { 0, 255, 100, 70 });

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			playerChoice = true;
			playerItem = false;
		}
		else if (itemChoice && smallMeat > 0 && app->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
		{
			itemChoice = false;
			healPlayerSmall = true;
			smallMeat--;
		}
		else if (itemChoice && largeMeat > 0 && app->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		{
			itemChoice = false;
			healPlayerLarge = true;
			largeMeat--;
		}
	}
	else
	{
		ItemUsage();
	}
}

void Combat::ItemUsage()
{
	if (healPlayerSmall)
	{
		if (playerTimeHeal < 100)
		{
			playerTimeHeal++;
		}
		else
		{
			playerTimeHeal = 0;
			playerChoice = true;
			playerItem = false;
			itemChoice = true;
			healPlayerSmall = false;
			playerResponseAble = true;

			app->scene->player1->health += HealPlayer(1);

			combatState = ENEMY_TURN;
			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
		}
	}
	else if (healPlayerLarge)
	{
		if (playerTimeHeal < 100)
		{
			playerTimeHeal++;
		}
		else
		{
			playerTimeHeal = 0;
			playerChoice = true;
			playerItem = false;
			itemChoice = true;
			healPlayerLarge = false;
			playerResponseAble = true;

			app->scene->player1->health += HealPlayer(2);

			combatState = ENEMY_TURN;
			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
		}
	}
}

int Combat::HealPlayer(int typeOfHeal)
{
	int healthLeft = app->scene->player1->maxHealth - app->scene->player1->health;
	int healthRestored = 0;
	if (typeOfHeal == 1) healthRestored = ceil(app->scene->player1->maxHealth * 25 / 100);
	else if (typeOfHeal == 2) healthRestored = ceil(app->scene->player1->maxHealth * 50 / 100);

	if (healthLeft <= healthRestored) return healthLeft;
	else if (healthLeft > healthRestored) return healthRestored;
}

void Combat::PlayerResponse()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->player1->jump && playerResponseAble && !app->scene->player1->crouch) app->scene->player1->jump = true;

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump) app->scene->player1->crouch = true;

	if (app->scene->player1->jump) app->scene->player1->Jump();

	if (app->scene->player1->crouch) app->scene->player1->Crouch();
}