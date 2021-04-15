#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"

#include "Combat.h"
#include "Enemy.h"
#include "Player.h"
#include "Collider.h"
#include "Player.h"

#include "Log.h"

Combat::Combat()
{
	currentPlayerAnim = nullptr;
	//enemy->currentEnemyAnim = nullptr;
	secondPlayer = false;
}

void Combat::Start()
{
	//Texture loading
	character1Spritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/combat_female_character_spritesheet.png");
	fullscreenAttack_0 = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/fullscreen_attack_0.png");
	fullscreenAttack_1 = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/fullscreen_attack_1.png");
	fullscreenAttack_2 = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/fullscreen_attack_2.png");
	fullscreenAttack_3 = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/fullscreen_attack_3.png");
	littleWolfSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/grey-wolf-spritesheet.png");
	grassyLandsBackground = app->tex->Load("Assets/Textures/Environment/Combat/grassy_lands_combat_scene.png");

	//Idle Animation Set
	currentPlayerAnim = &app->scene->player1->cIdleAnim;

	//Item Inventory amount
	Player* p = app->scene->player1;
	ItemSetup(p->smallMeatCount, p->largeMeatCount, p->featherCount, p->mantisRodCount, p->splitedEnemyCount, p->moneyCount);

	//Bool preparation for combat
	BoolStart();

	//Firts turn decision
	FirstTurnLogic();

	//LuckArray fill
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

	LOG("P1H: %d", app->scene->player1->health);
	if (secondPlayer) LOG("P2H: %d", app->scene->player2->health);
	LOG("EH: %d", enemy->health);
}

void Combat::Restart()
{
	combatState = NULL_STATE;
	enemy = nullptr;

	app->tex->UnLoad(character1Spritesheet);
	app->tex->UnLoad(fullscreenAttack_0);
	app->tex->UnLoad(fullscreenAttack_1);
	app->tex->UnLoad(fullscreenAttack_2);
	app->tex->UnLoad(fullscreenAttack_3);
	app->tex->UnLoad(littleWolfSpritesheet);
	app->tex->UnLoad(grassyLandsBackground);

	PlayerPosReset();
	app->scene->player2->colliderCombat.x = INIT2_COMBAT_POSX;
	app->scene->player2->colliderCombat.y = INIT2_COMBAT_POSY;
}

void Combat::Update()
{
	if (combatState != SECOND_PLAYER_TURN)
	{
		app->scene->moveButton->Update(0.0f);
		app->scene->itemButton->Update(0.0f);
	}

	app->scene->attackButton->Update(0.0f);
	app->scene->escapeButton->Update(0.0f);
	app->scene->splitButton->Update(0.0f);

	if (combatState != PLAYER_TURN)
	{
		app->scene->secondAttackButton->Update(0.0f);
		app->scene->protectButton->Update(0.0f);
		app->scene->buffButton->Update(0.0f);
	}

	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->Update(0.0f);

	currentPlayerAnim->Update(1.0f);
	//currentSecondPlayerAnim->Update(1.0f);

	/*LOG("PY: %d", app->scene->player1->colliderCombat.y + app->scene->player1->colliderCombat.h);
	LOG("EY: %d", enemy->colliderCombat.y + enemy->colliderCombat.h);*/

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debugCombat = !debugCombat;

	CombatLogic();
}

void Combat::Draw()
{
	DrawBakcground();

	if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->state == GuiControlState::NORMAL;
	else
	{
		app->scene->splitButton->state = GuiControlState::LOCKED;
	}

	// DEBUG COLLISIONS
	if (secondPlayer) app->render->DrawRectangle(app->scene->player2->colliderCombat, { 80, 100, 36, 255 }); //NEED TO BE IN FUNCTION DEBUGDRAW (when Animation Uploaded)

	app->render->DrawRectangle(enemy->colliderCombat, { 255, 0, 0 , 255 }); //NEED TO BE IN FUNCTION DEBUGDRAW (when Animation Uploaded)

	if (debugCombat) DebugDraw();

	//DRAW CHARACTERS
	DrawPlayer();

	if (secondPlayer) DrawSecondPlayer();

	DrawEnemy();

	//BULLETS
	if (enemy->enemyClass == EnemyClass::MANTIS) for (int i = 0; i < 5; i++) enemy->bullet[i].Draw();

	//INVENTORY
	if (drawInventory) app->render->DrawRectangle(inventorySimulation, { 0, 255, 100, 50 });

	app->guiManager->DrawCombatInterface(enemy);

	if (!secondPlayer)
	{
		if (combatState != ENEMY_TURN)
		{
			app->scene->attackButton->Draw();
			app->scene->moveButton->Draw();
			app->scene->itemButton->Draw();
			app->scene->escapeButton->Draw();
			app->scene->splitButton->Draw();

			app->scene->attackText->Draw();
			app->scene->moveText->Draw();
			app->scene->itemsText->Draw();
			app->scene->escapeText->Draw();
			app->scene->splitText->Draw();
		}
	}
	else if (secondPlayer)
	{
		if (combatState != ENEMY_TURN)
		{
			if (combatState == PLAYER_TURN)
			{
				app->scene->attackButton->Draw();
				app->scene->moveButton->Draw();
				app->scene->itemButton->Draw();

				app->scene->attackText->Draw();
				app->scene->moveText->Draw();
				app->scene->itemsText->Draw();
			}
			else if (combatState == SECOND_PLAYER_TURN)
			{
				app->scene->attackButton->Draw();
				app->scene->protectButton->Draw();
				app->scene->buffButton->Draw();

				app->scene->attackText->Draw();
				app->scene->protectText->Draw();
				app->scene->buffText->Draw();
			}

			app->scene->escapeButton->Draw();
			app->scene->splitButton->Draw();
			app->scene->escapeText->Draw();
			app->scene->splitText->Draw();
		}
	}

	DrawText();

	app->guiManager->DrawCursor();
}

void Combat::DrawPlayer()
{
	if (currentPlayerAnim == &app->scene->player1->cCrouchAnim)
	{
		app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x - 52, 400 - 52, &currentPlayerAnim->GetCurrentFrame());
	}
	else if (currentPlayerAnim == &app->scene->player1->cPos0AttackAnim)
	{
		app->render->DrawTexture(fullscreenAttack_0, 0,0, &currentPlayerAnim->GetCurrentFrame());
	}
	else if (currentPlayerAnim == &app->scene->player1->cPos1AttackAnim)
	{
		app->render->DrawTexture(fullscreenAttack_1, 0, 0, &currentPlayerAnim->GetCurrentFrame());
	}
	else if (currentPlayerAnim == &app->scene->player1->cPos2AttackAnim)
	{
		app->render->DrawTexture(fullscreenAttack_2, 0, 0, &currentPlayerAnim->GetCurrentFrame());
	}
	else if (currentPlayerAnim == &app->scene->player1->cPos3AttackAnim)
	{
		app->render->DrawTexture(fullscreenAttack_3, 0, 0, &currentPlayerAnim->GetCurrentFrame());
	}
	else
		app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x - 52, app->scene->player1->colliderCombat.y - 52, &currentPlayerAnim->GetCurrentFrame());
}

void Combat::DebugDraw()
{
	app->render->DrawRectangle(app->scene->player1->colliderCombat, { 100, 3, 56, 100 });
}

void Combat::DrawSecondPlayer()
{

}

void Combat::DrawEnemy()
{
	if (enemy != nullptr)
	{
		if (enemy->enemyClass == SMALL_WOLF)
		{
			enemy->currentEnemyAnim = &enemy->cLittleWolfAwakeAnim;
		}
		else if (enemy->enemyClass == BIRD)
		{

		}
		else if (enemy->enemyClass == MANTIS)
		{

		}
	}
}

void Combat::DrawBakcground()
{
	switch (app->scene->enviroment)
	{
		case GRASSY_LANDS:
			app->render->DrawTexture(grassyLandsBackground, 0, 0, &backgroundRect);
		break;

		case AUTUM_FALLS:
			app->render->DrawTexture(grassyLandsBackground, 0, 0, &backgroundRect);
			break;

		case MOSSY_LANDS:
			app->render->DrawTexture(grassyLandsBackground, 0, 0, &backgroundRect);
			break;
	}
}

void Combat::DrawText()
{
	turnText->Draw();
}

void Combat::FirstTurnLogic()
{
	if (app->scene->player1->velocity <= enemy->velocity)
	{
		combatState = ENEMY_TURN;
		turnText->SetString("ENEMY TURN");
	}
	else
	{
		playerChoice = true;
		combatState = PLAYER_TURN;
		turnText->SetString("PLAYER TURN");
	}
}

void Combat::CombatLogic()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeWait < 60)
		{
			enemyTimeWait++; // Make enemy wait so it does not atack directly
			if (enemyTimeWait == 59) EnemyAttackProbability();
		}
		else
		{
			EnemyAttack(enemy->enemyClass);

			PlayerResponse();

			if (enemy->enemyClass == SMALL_WOLF || enemy->enemyClass == BIG_WOLF)
			{
				if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 30) playerResponseAble = false;
			}
			else if (enemy->enemyClass == BIRD)
			{
				if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 60 - (steps * 20)) playerResponseAble = false;
			}
		}
	}
	else if (combatState == PLAYER_TURN)
	{
		if (!secondPlayerProtection)
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

			if (playerSplit)
			{
				PlayerSplit();
			}
		}
		else if (secondPlayerProtection)
		{
			if (secondPlayerTimeProtection < 25)
			{
				app->scene->player2->colliderCombat.x -= 1;
				app->scene->player2->colliderCombat.y -= 3;
				secondPlayerTimeProtection++;
			}
			else
			{
				SecondPlayerPosReset();
				secondPlayerTimeProtection = 0;
				secondPlayerProtection = false;
			}
		}
	}
	else if (combatState == SECOND_PLAYER_TURN)
	{
		if (secondPlayerChoice)
		{
			SecondPlayerChoiceLogic();
		}

		if (secondPlayerAttack)
		{
			SecondPlayerAttack();
		}

		if (secondPlayerProtect)
		{
			SecondPlayerProtect();
		}

		if (secondPlayerBuff)
		{
			SecondPlayerBuff();
		}
	}
	else if (combatState == WIN)
	{
		ItemDrop(enemy->enemyClass);
		playerWin = true;
		enemy->active = false;
	}
	else if (combatState == LOSE)
	{
		if (CompareFrames(currentPlayerAnim->GetCurrentFrame(), app->scene->player1->cDieAnim.GetLastFrame()))
		{
			playerLose = true;
			app->scene->player1->Refill();
		}
	}
	else if (combatState == SPLIT)
	{
		playerSplitWin = true;
		enemy->active = false;
	}
	else if (combatState == ESCAPE)
	{
		if (playerTimeEscape < 99)
		{
			playerTimeEscape++;
			app->scene->player1->colliderCombat.x -= 3;
		}
		else
		{
			playerTimeEscape = 0;
			playerEscaped = true;
		}
	}
}

void Combat::EndBattleSolving()
{
	if (playerWin) app->scene->SetScene(LEVEL_UP, enemy->exp);
	else if (playerLose) app->scene->SetScene(WORLD, Places::MAIN_VILLAGE);
	else if (playerSplitWin) app->scene->SetScene(LEVEL_UP);
	else if (playerEscaped) app->scene->SetScene(WORLD, app->scene->world->GetPlace());
}

void Combat::BoolStart()
{
// Player 1
	playerAttack = false;
	playerItem = false;
	playerStep = false;
	playerSplit = false;
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

// Player 2
	secondPlayerAttack = false;
	secondPlayerProtect = false;
	secondPlayerBuff = false;
	secondPlayerChoice = true;

	secondPlayerAttack = false;
	secondPlayerProtect = false;
	secondPlayerBuff = false;
	drawBuffMenu = false;

//Items
	wearFeather = false;
	wearMantisLeg = false;

// END BATTLE BOOLS
	playerEscaped = false;
	playerWin = false;
	playerLose = false;
	playerSplitWin = false;
}

// ---------------------------------------------------

void Combat::PlayerChoiceLogic()
{
	if (app->scene->attackPressed)
	{
		playerAttack = true;
		playerChoice = false;
		return;
	}
	else if (app->scene->movePressed && steps < 3)
	{
		playerStep = true;
		playerChoice = false;

		currentPlayerAnim = &app->scene->player1->cStepAnim;
		currentPlayerAnim->Reset();

		return;
	}
	else if (app->scene->itemPressed)
	{
		playerItem = true;
		playerChoice = false;
		drawInventory = true;
		return;
	}
	else if (app->scene->escapePressed)
	{
		playerChoice = false;
		short int probabilityRange = enemy->lvl - app->scene->player1->lvl;
		EscapeProbability(probabilityRange);
		return;
	}
	else if (app->scene->splitPressed)
	{
		playerChoice = false;
		playerSplit = true;
		return;
	}
}

void Combat::SecondPlayerChoiceLogic()
{
	if (app->scene->secondAttackPressed)
	{
		secondPlayerAttack = true;
		secondPlayerChoice = false;
		return;
	}
	else if (app->scene->protectPressed)
	{
		secondPlayerProtect = true;
		secondPlayerChoice = false;
		return;
	}
	else if (app->scene->buffPressed)
	{
		secondPlayerBuff = true;
		secondPlayerChoice = false;
		drawBuffMenu = true;
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

	return 0;
}

int Combat::SecondPlayerDamageLogic()
{
	int damage = app->scene->player2->strength - enemy->defense;

	if (damage < 0) damage = 0;

	int variation = rand() % 3;
	bool negative = (bool)(rand() % 2);

	if (!negative) damage += variation;
	else if (negative) damage -= variation;

	return damage;
}

int Combat::EnemyDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defense;

	if (damage < 1) damage = 1;

	return damage;
}

int Combat::BulletDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defense;

	if (damage != 0) damage = ceil(damage / 5);
	if (damage < 1) damage = 1;

	return damage;
}

void Combat::EnemyAttack(EnemyClass enemyc)
{
	if (enemyc == EnemyClass::SMALL_WOLF)
	{
		if (enemy->attack == 1)
		{
			if (enemy->smallWolfTimeAttack1 < 170)
			{
				enemy->SmallWolfAttack(enemy->attack);

				PlayerHitLogic();
			}
			else
			{
				enemy->smallWolfTimeAttack1 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_SMALLWOLF_POSX;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
		else if (enemy->attack == 2)
		{
			if (enemy->smallWolfTimeAttack2 < 220)
			{
				enemy->SmallWolfAttack(enemy->attack);

				PlayerHitLogic();
				enemy->smallWolfTimeAttack2++;
			}
			else
			{
				enemy->smallWolfTimeAttack2 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_SMALLWOLF_POSX;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
	}
	else if (enemyc == EnemyClass::BIRD)
	{
		if (enemy->attack == 1)
		{
			if (enemy->birdTimeAttack1 < 280)
			{
				enemy->BirdAttack(enemy->attack);

				PlayerHitLogic();
			}
			else
			{
				enemy->birdTimeAttack1 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_BIRD_POSX;
				enemy->colliderCombat.y = INIT_BIRD_POSY;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
		else if (enemy->attack == 2)
		{
			if (enemy->birdTimeAttack2 < 280)
			{
				enemy->BirdAttack(enemy->attack);

				PlayerHitLogic();
			}
			else
			{
				enemy->birdTimeAttack2 = 0;
				enemyTimeWait = 0;
				enemy->colliderCombat.x = INIT_BIRD_POSX;
				enemy->colliderCombat.y = INIT_BIRD_POSY;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
		else if (enemy->attack == 3)
		{
			if (enemy->birdTimeAttack3 < 200)
			{
				enemy->birdTimeAttack3++;

				if (enemy->birdTimeAttack3 >= 60 && enemy->birdTimeAttack3 < 145)
				{
					app->scene->player1->colliderCombat.x -= 2;
				}
				else if (enemy->birdTimeAttack3 == 145)
				{
					app->scene->player1->colliderCombat.x -= 1;
					steps--;
				}
			}
			else
			{
				enemy->birdTimeAttack3 = 0;
				enemyTimeWait = 0;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
	}
	else if (enemyc == EnemyClass::MANTIS)
	{
		if (enemy->attack == 1)
		{
			if (enemy->mantisTimeAttack1 < 280)
			{
				enemy->MantisAttack(enemy->attack);

				PlayerBulletHitLogic();

				enemy->mantisTimeAttack1++;
			}
			else
			{
				enemy->mantisTimeAttack1 = 0;
				enemy->jumpTime = 0;
				enemyTimeWait = 0;
				playerHitAble = true;

				bulletHitted = -1;

				AfterEnemyAttack();
			}
		}
		else if (enemy->attack == 2)
		{
			if (enemy->mantisTimeAttack2 < 280)
			{
				enemy->MantisAttack(enemy->attack);

				PlayerHitLogic();

				enemy->mantisTimeAttack2++;
			}
			else
			{
				enemy->mantisTimeAttack2 = 0;
				enemyTimeWait = 0;
				playerHitAble = true;

				AfterEnemyAttack();
			}
		}
		else if (enemy->attack == 3)
		{
			if (enemy->mantisTimeAttack3 < 250)
			{
				enemy->MantisAttack(enemy->attack);

				enemy->mantisTimeAttack3++;
			}
			else
			{
				enemy->mantisTimeAttack3 = 0;
				enemyTimeWait = 0;

				AfterEnemyAttack();
			}
		}
	}
}

void Combat::AfterEnemyAttack()
{
	if (app->scene->player1->health > 0)
	{
		if (enemy->enemyClass == MANTIS && enemy->attack == 3)
		{
			playerStepDenied = true;
			app->scene->moveButton->state = GuiControlState::LOCKED;
		}
		PlayerTurn();
	}
	else if (app->scene->player1->health <= 0)
	{
		PlayerDie();
	}
}

void Combat::PlayerAttack()
{
	//app->scene->player1->cPos0AttackAnim.Reset();
	switch (steps)
	{
	case(0):
		currentPlayerAnim = &app->scene->player1->cPos0AttackAnim;
		break;
	case(1):
		currentPlayerAnim = &app->scene->player1->cPos1AttackAnim;
		break;
	case(2):
		currentPlayerAnim = &app->scene->player1->cPos2AttackAnim;
		break;
	case(3):
		currentPlayerAnim = &app->scene->player1->cPos3AttackAnim;
		break;
	}

	if (playerTimeAttack < 200)
	{
		playerTimeAttack++;
	}
	else
	{
		enemy->health -= PlayerDamageLogic();
		LOG("Enemy Hit, EH: %d", enemy->health);

		playerTimeAttack = 0;
		playerAttack = false;

		currentPlayerAnim->Reset();

		PlayerPosReset();

		if (enemy->health > 0)
		{
			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();
		}
		else if (enemy->health <= 0)
		{
			PlayerWin();
		}
	}
}

void Combat::SecondPlayerAttack()
{
	if (secondPlayerTimeAttack < 215)
	{
		secondPlayerTimeAttack++;
		
		if (enemy->enemyClass == SMALL_WOLF)
		{
			app->scene->player2->colliderCombat.x += 6;
		}
		else if (enemy->enemyClass == BIRD) app->scene->player2->colliderCombat.x += 6;
		else if (enemy->enemyClass == MANTIS)
		{
			app->scene->player2->colliderCombat.x += 6;
		}

		if (app->scene->player2->colliderCombat.x > 1280) app->scene->player2->colliderCombat.x = 0;
	}
	else
	{
		enemy->health -= SecondPlayerDamageLogic();
		LOG("Enemy Hit, EH: %d", enemy->health);

		secondPlayerTimeAttack = 0;
		secondPlayerAttack = false;

		SecondPlayerPosReset();

		if (enemy->health > 0)
		{
			EnemyTurn();
		}
		else if (enemy->health <= 0)
		{
			PlayerWin();
		}
	}
}

void Combat::PlayerMove()
{
	if (playerTimeMove < 57)
	{
		app->scene->player1->colliderCombat.x += 3;
		app->scene->player2->colliderCombat.x += 3;
		playerTimeMove++;
	}
	else
	{
		playerTimeMove = 0;
		playerStep = false;
		steps++;
		LOG("Player moved to position : %d", steps);

		if (!secondPlayer) EnemyTurn();
		else if (secondPlayer) SecondPlayerTurn();
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
		else if (itemChoice && splitedEnemy > 0 && app->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN)
		{
			itemChoice = false;
			enemyThrow = true;
			splitedEnemy--;
		}
	}
	else
	{
		drawInventory = false;
		ItemUsage();
	}
}

void Combat::SecondPlayerProtect()
{
	if (secondPlayerTimeProtection < 25)
	{
		app->scene->player2->colliderCombat.x += 1;
		app->scene->player2->colliderCombat.y += 3;
		secondPlayerTimeProtection++;
	}
	else
	{
		secondPlayerTimeProtection = 0;
		secondPlayerProtect = false;
		secondPlayerProtection = true;
		EnemyTurn();
	}
}

void Combat::SecondPlayerBuff()
{
	EnemyTurn();
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
			playerItem = false;
			itemChoice = true;
			healPlayerSmall = false;

			app->scene->player1->health += HealPlayer(1);

			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();
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
			playerItem = false;
			itemChoice = true;
			healPlayerLarge = false;

			app->scene->player1->health += HealPlayer(2);

			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();
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
			playerItem = false;
			itemChoice = true;
			featherPlayerTurn = false;

			wearFeather = true;
			app->scene->player1->health += HealPlayer(3);

			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();
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
			playerItem = false;
			itemChoice = true;
			protectPlayerTurn = false;

			wearMantisLeg = true;

			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();
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
			playerItem = false;
			itemChoice = true;
			enemyThrow = false;

			enemy->health -= EnemyItemDamage();

			if (enemy->health > 0)
			{
				if (!secondPlayer) EnemyTurn();
				else if (secondPlayer) SecondPlayerTurn();
			}
			else if (enemy->health <= 0)
			{
				PlayerWin();
			}
		}
	}
}

void Combat::PlayerSplit()
{
	if (playerTimeSplit < 200)
	{
		playerTimeSplit++;
	}
	else
	{
		playerTimeSplit = 0;
		playerSplit = false;

		int random = rand() % 5;

		// USING LUCK
		if (app->scene->player1->luck > 0)
		{
			int luck = rand() % 100;

			if (luckArray[luck]) //40% to lose
			{
				if (random < 2)
				{
					PlayerPosReset();

					if (!secondPlayer) EnemyTurn();
					else if (secondPlayer) SecondPlayerTurn();

					return;
				}
				else if (random >= 2)
				{
					app->scene->player1->health -= enemy->health;

					if (app->scene->player1->health <= 0)
					{
						PlayerDie();
					}
					else if (app->scene->player1->health > 0)
					{
						PlayerSplitWin();
					}
					return;
				}
			}
		}

		// WITHOUT LUCK
		if (random < 3) //60% to lose
		{
			PlayerPosReset();

			if (!secondPlayer) EnemyTurn();
			else if (secondPlayer) SecondPlayerTurn();

			return;
		}
		else if (random >= 3)
		{
			app->scene->player1->health -= enemy->health;

			if (app->scene->player1->health <= 0)
			{
				PlayerDie();
			}
			else if (app->scene->player1->health > 0)
			{
				PlayerSplitWin();
			}
		}
	}

	//Màxim d'enemics spliteats? 3
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

	return 0;
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
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->player1->jump && playerResponseAble && !app->scene->player1->crouch)
	{
		app->scene->player1->jump = true;
		currentPlayerAnim = &app->scene->player1->cJumpAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump)
	{
		app->scene->player1->crouch = true;
		currentPlayerAnim = &app->scene->player1->cCrouchAnim;
	}

	if (app->scene->player1->jump)
	{
		if (wearFeather)
		{
			app->scene->player1->FeatherJump();
			app->scene->player2->FeatherJump();
		}
		else
		{
			app->scene->player1->Jump();
			app->scene->player2->Jump();
		}
	}

	if (app->scene->player1->crouch) app->scene->player1->Crouch();
}

void Combat::ItemSetup(int xsmallMeat, int xlargeMeat, int xfeather, int xmantisLeg, int xsplitedEnemy, int xmoney)
{
	smallMeat = xsmallMeat;
	largeMeat = xlargeMeat;
	feather = xfeather;
	mantisLeg = xmantisLeg;
	splitedEnemy = xsplitedEnemy;
	money = xmoney;
}

void Combat::EnemyAttackProbability()
{
	if (enemy->enemyClass == EnemyClass::SMALL_WOLF)
	{
		int random = rand() % 8;
		if (random < 6) enemy->attack = 1;
		else if (random >= 6) enemy->attack = 2;
	}
	else if (enemy->enemyClass == EnemyClass::BIRD)
	{
		if (steps > 0)
		{
			int random = rand() % 6;
			if (random < 1 && enemy->attack != 3)
			{
				enemy->attack = 3;
				return;
			}
		}

		int randomx = rand() % 3;
		if (randomx == 2) enemy->attack = 2;
		else if (randomx != 2) enemy->attack = 1;
	}
	else if (enemy->enemyClass == EnemyClass::MANTIS)
	{
		int random = rand() % 10;

		if (random < 5) enemy->attack = 1;
		else if (random > 4 && random < 9) enemy->attack = 2;
		else if (random > 8)
		{
			if (steps != 3) enemy->attack = 3;
			else 
			{
				enemy->attack = 2;
			}
		}
	}
}

void Combat::PlayerMoneyLose()
{
	int lostMoney = ceil(app->scene->player1->lvl / 10);
	app->scene->player1->moneyCount -= lostMoney;

	if (app->scene->player1->moneyCount < 0) app->scene->player1->moneyCount = 0;
}

void Combat::EscapeProbability(short int probabilityRange)
{
	if (probabilityRange <= -8) PlayerEscape();
	else if (probabilityRange >= -7 && probabilityRange <= -4)
	{
		int random = rand() % 4;

		if (random != 0) PlayerEscape();
		else
		{
			playerAttack = false;

			EnemyTurn();
		}
	}
	else if (probabilityRange >= -3 && probabilityRange <= 3)
	{
		int random = rand() % 2;

		if (random == 0) PlayerEscape();
		else
		{
			playerAttack = false;

			EnemyTurn();
		}
	}
	else if (probabilityRange >= 4 && probabilityRange <= 7)
	{
		int random = rand() % 4;

		if (random == 0) PlayerEscape();
		else
		{
			playerAttack = false;

			EnemyTurn();
		}
	}
	else if (probabilityRange >= 8)
	{
		playerAttack = false;

		EnemyTurn();
	}
}

void Combat::PlayerHitLogic()
{
	if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, enemy->colliderCombat))
	{
		if (app->scene->player1->godMode) LOG("Player is inmune");
		else
		{
			playerHitAble = false;
			if (!wearMantisLeg)
			{
				if (!secondPlayerProtection) app->scene->player1->health -= EnemyDamageLogic();
				else if (secondPlayerProtection)
				{
					app->scene->player1->health -= floor((EnemyDamageLogic() - app->scene->player2->defense) / 2);
					app->scene->player2->health -= ceil((EnemyDamageLogic() - app->scene->player2->defense) / 2);
					LOG("Second Player Hit - PH: %d", app->scene->player2->health);
				}

				LOG("Player Hit - PH: %d", app->scene->player1->health);
			}
			else if (wearMantisLeg) wearMantisLeg = false;
		}
	}
}

void Combat::PlayerBulletHitLogic()
{
	for (int i = 0; i < 5; i++)
	{
		if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, enemy->bullet[i].bulletRect))
		{
			playerHitAble = false;
			if (!wearMantisLeg)
			{
				if (!secondPlayerProtection) app->scene->player1->health -= EnemyDamageLogic();
				else if (secondPlayerProtection)
				{
					app->scene->player1->health -= floor(EnemyDamageLogic() / 2);
					app->scene->player2->health -= ceil(EnemyDamageLogic() / 2);
					LOG("Second Player Hit - PH: %d", app->scene->player2->health);
				}

				LOG("Player Hit - PH: %d", app->scene->player1->health);
			}
			else if (wearMantisLeg) wearMantisLeg = false;

			bulletHitted = i;
		}
	}

	if (bulletHitted != -1 && enemy->bullet[bulletHitted].bulletRect.x < app->scene->player1->colliderCombat.x - 42)
	{
		playerHitAble = true;
	}
}

void Combat::PlayerPosReset()
{
	app->scene->player1->colliderCombat.x = INIT_COMBAT_POSX;
	app->scene->player1->colliderCombat.y = INIT_COMBAT_POSY;

	if (secondPlayer)
	{
		app->scene->player2->colliderCombat.x = INIT2_COMBAT_POSX;
		app->scene->player2->colliderCombat.y = INIT2_COMBAT_POSY;
	}
	steps = 0;
}

void Combat::SecondPlayerPosReset()
{
	app->scene->player2->colliderCombat.x = INIT2_COMBAT_POSX + (171 * steps);
	app->scene->player2->colliderCombat.y = INIT2_COMBAT_POSY;
}

void Combat::ItemDrop(EnemyClass enemy)
{
	int random = rand() % 20;
	int a = rand() % 100;

	if (enemy == EnemyClass::SMALL_WOLF)
	{
		if (random < 9)
		{
			if (luckArray[a]) smallMeat += 2;
			else if (!luckArray[a]) smallMeat++;

			if (smallMeat > MAX_MEAT) smallMeat = MAX_MEAT;
		}
	}
	else if (enemy == EnemyClass::BIG_WOLF)
	{
		if (random < 6)
		{
			if (luckArray[a]) largeMeat += 2;
			else if (!luckArray[a])
			{
				largeMeat++;
				random = rand() % 20;
				if (random < 3)
				{
					a = rand() % 100;
					if (luckArray[a]) smallMeat += 2;
					else if (!luckArray[a]) smallMeat++;
				}
			}

			if (largeMeat > MAX_MEAT) largeMeat = MAX_MEAT;
			if (smallMeat > MAX_MEAT) smallMeat = MAX_MEAT;
		}
	}
	else if (enemy == EnemyClass::BIRD)
	{
		int probability = 3;
		if (luckArray[a]) probability = 10;

		if (random < probability) feather++;

		if (feather > MAX_COMBATITEM) feather = MAX_COMBATITEM;
	}
	else if (enemy == EnemyClass::MANTIS)
	{
		if (random < 2)
		{
			int probability = 2;
			if (luckArray[a]) probability = 8;

			if (random < probability) mantisLeg++;

			if (mantisLeg > MAX_COMBATITEM) mantisLeg = MAX_COMBATITEM;
		}
	}
}

// State Changing Functions

void Combat::EnemyTurn()
{
	LOG("ENEMY TURN");
	LOG("Enemy Health: %d", enemy->health);

	combatState = ENEMY_TURN;

	currentPlayerAnim = &app->scene->player1->cIdleAnim;
	currentPlayerAnim->Reset();

	playerResponseAble = true;
	playerChoice = true;
	secondPlayerChoice = true;
	if (playerStepDenied)
	{
		app->scene->moveButton->state == GuiControlState::NORMAL;
		playerStepDenied = false;
	}

	if (secondPlayer) secondPlayerChoice = true;

	if (steps == 3) app->scene->moveButton->state == GuiControlState::LOCKED;
	else
	{
		app->scene->moveButton->state == GuiControlState::LOCKED;
	}

	app->scene->escapeButton->state = GuiControlState::NORMAL;
	app->scene->splitButton->state = GuiControlState::NORMAL;

	turnText->SetString("ENEMY TURN");
}

void Combat::PlayerTurn()
{
	combatState = PLAYER_TURN;
	LOG("PLAYER TURN");
	LOG("Player Health: %d", app->scene->player1->health);

	if (wearFeather) wearFeather = false;
	if (wearMantisLeg) wearMantisLeg = false;

	if (app->scene->player2->health <= 0) SecondPlayerDie();

	turnText->SetString("PLAYER TURN");
}

void Combat::SecondPlayerTurn()
{
	LOG("SECOND PLAYER TURN");
	LOG("Second Player Health: %d", app->scene->player2->health);

	combatState = SECOND_PLAYER_TURN;

	currentPlayerAnim = &app->scene->player1->cIdleAnim;
	currentPlayerAnim->Reset();

	app->scene->escapeButton->state = GuiControlState::LOCKED;
	app->scene->splitButton->state = GuiControlState::LOCKED;

	turnText->SetString("SECOND PLAYER TURN");
}

void Combat::PlayerWin()
{
	LOG("PLAYER WIN");
	combatState = WIN;
}

void Combat::PlayerDie()
{
	currentPlayerAnim = &app->scene->player1->cDieAnim;
	currentPlayerAnim->Reset();
	LOG("PLAYER LOSE");
	combatState = LOSE;
}

void Combat::SecondPlayerDie()
{
	/*currentPlayerAnim = &app->scene->player1->cDieAnim;
	currentPlayerAnim->Reset();*/
	LOG("SECOND PLAYER DIED");
	secondPlayer = false;
}

void Combat::PlayerEscape()
{
	combatState = ESCAPE;

	PlayerMoneyLose();

	currentPlayerAnim = &app->scene->player1->cStepAnim;
	currentPlayerAnim->Reset();
}

void Combat::PlayerSplitWin()
{
	LOG("ENEMY SPLITED");
	splitedEnemy++;
	combatState = SPLIT;
}