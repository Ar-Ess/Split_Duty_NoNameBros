#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiButton.h"

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
	//HardCode ENEMY ------------------RECT------------------   LVL EXP  HP STR DEF VEL
	enemy->SetUp({ INIT_ENEMY1_POSX, INIT_ENEMY1_POSY, 70, 55 }, 2, 200, 30, 30, 10, 20);

	//Player HardCoded
	app->scene->player1->health = 35;
	app->scene->player1->strength = 30;
	app->scene->player1->defense = 5;
	app->scene->player1->luck = 0;
	app->scene->player1->velocity = 0;
	app->scene->player1->lvl = 10;
	app->scene->player1->colliderCombat.x = INIT_COMBAT_POSX;
	app->scene->player1->colliderCombat.y = INIT_COMBAT_POSY;

	currPlayerAnim = &app->scene->player1->cIdleAnim;

	//Item Inventory amount
	ItemSetup(1, 1, 1, 1, 1);

	playerScape = false;
	playerAttack = false;
	playerItem = false;
	playerStep = false;
	playerReap = false;
	playerResponseAble = true;
	playerHitAble = true;
	playerChoice = true;
	steps = 0;

	itemChoice = true;
	healPlayerSmall = false;
	healPlayerLarge = false;
	featherPlayerTurn = false;
	protectPlayerTurn = false;
	enemyThrow = false;

	wearFeather = false;
	wearMantisLeg = false;

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
	app->tex->UnLoad(character1Spritesheet);
}

void Combat::Update()
{
	app->scene->attackButton->Update(0.0f);
	app->scene->moveButton->Update(0.0f);
	app->scene->itemButton->Update(0.0f);
	app->scene->scapeButton->Update(0.0f);

	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->reapButton->Update(0.0f);

	currPlayerAnim->Update(1.0f);

	CombatLogic();
}

void Combat::Draw()
{
	app->scene->attackButton->Draw();
	app->scene->moveButton->Draw();
	app->scene->itemButton->Draw();
	app->scene->scapeButton->Draw();
	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->reapButton->Draw();

	app->render->DrawRectangle(app->scene->player1->colliderCombat, { 100, 3, 56, 100 });

	app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x, app->scene->player1->colliderCombat.y, &currPlayerAnim->GetCurrentFrame());

	app->render->DrawRectangle(enemy->colliderCombat, { 255, 0, 0 , 255 });

	if (drawInventory) app->render->DrawRectangle(inventorySimulation, { 0, 255, 100, 50 });
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

void Combat::CombatLogic()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeWait < 60) enemyTimeWait++; // Make enemy wait so it does not atack directly
		else
		{
			EnemyAttack();

			PlayerResponse();

			if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 30) playerResponseAble = false;
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

		if (playerReap)
		{
			PlayerSplit();
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

//---------------------------------------------------

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
		return;
	}
	else if (app->scene->itemPressed)
	{
		playerItem = true;
		playerChoice = false;
		drawInventory = true;
		return;
	}
	else if (app->scene->scapePressed)
	{
		playerChoice = false;
		int probabilityRange = enemy->lvl - app->scene->player1->lvl;

		if (probabilityRange <= -8) playerScape = true;
		else if (probabilityRange >= -7 && probabilityRange <= -4)
		{
			int random = rand() % 4;

			if (random != 0) playerScape = true;
			else
			{
				playerAttack = false;
				playerResponseAble = true;
				playerChoice = true;

				LOG("ENEMY TURN");
				LOG("Enemy Health: %d", enemy->health);
				combatState = ENEMY_TURN;
			}
		}
		else if (probabilityRange >= -3 && probabilityRange <= 3)
		{
			int random = rand() % 2;

			if (random == 0) playerScape = true;
			else
			{
				playerAttack = false;
				playerResponseAble = true;
				playerChoice = true;

				LOG("ENEMY TURN");
				LOG("Enemy Health: %d", enemy->health);
				combatState = ENEMY_TURN;
			}
		}
		else if (probabilityRange >= 4 && probabilityRange <= 7)
		{
			int random = rand() % 4;

			if (random == 0) playerScape = true;
			else
			{
				playerAttack = false;
				playerResponseAble = true;
				playerChoice = true;

				LOG("ENEMY TURN");
				LOG("Enemy Health: %d", enemy->health);
				combatState = ENEMY_TURN;
			}
		}
		else if (probabilityRange >= 8)
		{
			playerAttack = false;
			playerResponseAble = true;
			playerChoice = true;

			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
			combatState = ENEMY_TURN;
		}

		return;
	}
	else if (app->scene->splitPressed)
	{
		playerChoice = false;
		playerReap = true;
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

		if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, enemy->colliderCombat))
		{
			playerHitAble = false;
			if (!wearMantisLeg)
			{
				app->scene->player1->health -= EnemyDamageLogic();
				LOG("Player Hit - PH: %d", app->scene->player1->health);
			}
			else if (wearMantisLeg) wearMantisLeg = false;
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
			if (wearFeather) wearFeather = false;
			if (wearMantisLeg) wearMantisLeg = false;
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
		app->scene->player1->colliderCombat.x += 7;
		playerTimeAttack++;
	}
	else
	{
		enemy->health -= PlayerDamageLogic();
		LOG("Enemy Hit, EH: %d", enemy->health);

		playerTimeAttack = 0;
		app->scene->player1->colliderCombat.x = INIT_COMBAT_POSX;

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
	currPlayerAnim = &app->scene->player1->cStepAnim;
	if (playerTimeMove < 57)
	{
		app->scene->player1->colliderCombat.x += 3;
		playerTimeMove++;
	}
	else
	{
		LOG("ENEMY TURN");
		LOG("Enemy Health: %d", enemy->health);
		playerTimeMove = 0;
		combatState = ENEMY_TURN;
		playerStep = false;
		playerResponseAble = true;
		playerChoice = true;
		steps++;
		currPlayerAnim = &app->scene->player1->cIdleAnim;
	}
}

void Combat::PlayerItemChoose()
{
	if (itemChoice)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			playerChoice = true;
			drawInventory = false;
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
		else if (itemChoice && feather > 0 && app->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN)
		{
			itemChoice = false;
			featherPlayerTurn = true;
			feather--;
		}
		else if (itemChoice && mantisLeg > 0 && app->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN)
		{
			itemChoice = false;
			protectPlayerTurn = true;
			mantisLeg--;
		}
		else if (itemChoice && tamedEnemy > 0 && app->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
		{
			itemChoice = false;
			enemyThrow = true;
			tamedEnemy--;
		}
	}
	else
	{
		drawInventory = false;
		ItemUsage();
	}
}

void Combat::PlayerSplit()
{
	//No pots reapear si l'enemy no est� a menys del 20% de vida
	//Tamejar et dona exp? si, no, no, no
	//Probabilitat de fallar del 60%
	//Luck interv� en reduir el % de split? si
	//M�xim d'enemics spliteats? 3

	//En cas d'�xit, 7EH, PH - 7.
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
	else if (featherPlayerTurn)
	{
		if (playerTimeWearFeather < 200)
		{
			playerTimeWearFeather++;
		}
		else
		{
			playerTimeWearFeather = 0;
			playerChoice = true;
			playerItem = false;
			itemChoice = true;
			featherPlayerTurn = false;
			playerResponseAble = true;

			wearFeather = true;
			app->scene->player1->health += HealPlayer(3);

			combatState = ENEMY_TURN;
			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
		}
	}
	else if (protectPlayerTurn)
	{
		if (playerTimeWearLeg < 200)
		{
			playerTimeWearLeg++;
		}
		else
		{
			playerTimeWearLeg = 0;
			playerChoice = true;
			playerItem = false;
			itemChoice = true;
			protectPlayerTurn = false;
			playerResponseAble = true;

			wearMantisLeg = true;

			combatState = ENEMY_TURN;
			LOG("ENEMY TURN");
			LOG("Enemy Health: %d", enemy->health);
		}
	}
	else if (enemyThrow)
	{
		if (playerTimeEnemyThrow < 120)
		{
			playerTimeEnemyThrow++;
		}
		else
		{
			playerTimeEnemyThrow = 0;
			playerChoice = true;
			playerItem = false;
			itemChoice = true;
			enemyThrow = false;
			playerResponseAble = true;

			enemy->health -= EnemyItemDamage();

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
}

int Combat::HealPlayer(int typeOfHeal)
{
	int healthLeft = app->scene->player1->maxHealth - app->scene->player1->health;
	int healthRestored = 0;

	if (typeOfHeal == 1) healthRestored = ceil(app->scene->player1->maxHealth * 30 / 100);
	else if (typeOfHeal == 2) healthRestored = ceil(app->scene->player1->maxHealth * 60 / 100);
	else if (typeOfHeal == 3)
	{
		int random = rand() % 5;
		int neg = rand() % 2;
		if (neg == 0) random = -random;
		healthRestored = ceil(app->scene->player1->maxHealth * (15 + random) / 100);
	}

	if (healthLeft <= healthRestored) return healthLeft;
	else if (healthLeft > healthRestored) return healthRestored;
}

int Combat::EnemyItemDamage()
{
	int itemDamage = 25;
	int damage = itemDamage + floor(app->scene->player1->strength / 5) - enemy->defense;

	int damagePlus = rand() % 5;
	int negOrPos = rand() % 2;

	if (negOrPos == 0) damagePlus = -damagePlus;

	return damage + damagePlus;
}

void Combat::PlayerResponse()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->player1->jump && playerResponseAble && !app->scene->player1->crouch) app->scene->player1->jump = true;

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump) app->scene->player1->crouch = true;

	if (app->scene->player1->jump)
	{
		if (wearFeather) app->scene->player1->FeatherJump();
		else
		{
			app->scene->player1->Jump();
		}
	}

	if (app->scene->player1->crouch) app->scene->player1->Crouch();
}

void Combat::ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xtamedEnemy)
{
	smallMeat = xsmallMeat;
	largeMeat = xlargeMeat;
	feather = xfeather;
	mantisLeg = xmantisLeg;
	tamedEnemy = xtamedEnemy;
}