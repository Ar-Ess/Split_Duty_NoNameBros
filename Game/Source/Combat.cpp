#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "Audio.h"
#include "EntityManager.h"

#include "Combat.h"
#include "Enemy.h"
#include "Boss.h"
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

// START

void Combat::Start()
{
	Player* p = app->scene->player1;

	if (enemy != nullptr) enemyBattle = true;
	else if (boss != nullptr) bossBattle = true;

	//Texture loading
	character1Spritesheet = app->tex->Load("Assets/Textures/Characters/Female_Main_Character/combat_female_character_spritesheet.png");
	character2Spritesheet = app->tex->Load("Assets/Textures/Characters/Second_Player/second_player.png");
	grassyLandsBackground = app->tex->Load("Assets/Textures/Environment/grassy_lands_combat_scene.png");
	combatInventory = app->tex->Load("Assets/Screens/combat_inventory.png");

	if (enemyBattle) EnemyStart();
	else if (bossBattle) BossStart();

	//Idle Animation Set
	currentPlayerAnim = &p->cIdleAnim;

	//Item Inventory amount
	ItemSetup(p->smallMeatCount, p->largeMeatCount, p->featherCount, p->mantisRodCount, p->splitedEnemyCount, p->moneyCount);

	//Bool preparation for combat
	BoolStart();

	//LuckArray fill
	int pLuck = p->luckStat;
	if (pLuck > 0)
	{
		for (int i = 0; i < 100; i++)
		{
			int trufals = i - pLuck;

			if (trufals < 0) luckArray[i] = true;
			else if (trufals >= 0) luckArray[i] = false;
		}
	}

	p = nullptr;
}

void Combat::EnemyStart()
{
	switch (enemy->enemyClass)
	{
	case(EnemyClass::SMALL_WOLF):
		enemySpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/grey_wolf_spritesheet.png");
		break;
	case(EnemyClass::BIRD):
		enemySpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		break;
	case(EnemyClass::MANTIS):
		enemySpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
	}

	//Firts turn decision
	FirstTurnLogic();

	currentEnemyAnim = &enemy->idleAnim;
}

void Combat::BossStart()
{
	combatState = BOSS_TURN;

	switch (boss->bossClass)
	{
	case(BossClass::BOSS_TUTORIAL):
		//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Wolf/grey_wolf_spritesheet.png");
		break;
	case(BossClass::BOSS_I):
		//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Bat/bat_spritesheet.png");
		break;
	case(BossClass::BOSS_II):
		//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		break;
	case(BossClass::BOSS_III):
		//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
		break;
	}

	currentBossAnim = &boss->idleAnim;
}

void Combat::Restart()
{
	combatState = NULL_STATE;
	enemy = nullptr;
	boss = nullptr;
	enemyBattle = false;
	bossBattle = false;

	SDL_ShowCursor(1);

	buffChoice = 0;
	UpdateBuffs();

	app->tex->UnLoad(character1Spritesheet);
	app->tex->UnLoad(character2Spritesheet);
	app->tex->UnLoad(enemySpritesheet);
	app->tex->UnLoad(bossSpritesheet);
	app->tex->UnLoad(grassyLandsBackground);
	app->tex->UnLoad(combatInventory);

	PlayerPosReset();
	app->scene->player2->colliderCombat.x = INIT2_COMBAT_POSX;
	app->scene->player2->colliderCombat.y = INIT2_COMBAT_POSY;
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
	moneyThrow = false;

	drawInventory = false;

	playerStepDenied = false;
	protectPlayerTurn = false;

	// Player 2
	buffChoice = true;
	secondPlayerAttack = false;
	secondPlayerProtect = false;
	secondPlayerProtection = false;
	secondPlayerBuff = false;
	secondPlayerChoice = true;
	drawBuffMenu = false;
	secondPlayerReturning = false;

	attackBuff = false;
	defenseBuff = false;
	thirdBuff = false;

	// ENEMY
	enemyAttack = false;

	//Items
	wearFeather = false;
	wearMantisLeg = false;

	// END BATTLE BOOLS
	playerEscaped = false;
	playerWin = false;
	playerLose = false;
	playerSplitWin = false;
}

void Combat::FirstTurnLogic()
{
	if (app->scene->player1->velocityStat < enemy->velocity)
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

// UPDATE

void Combat::Update()
{
	if (!drawInventory) UpdateButtons();

	UpdatePopUps();

	currentPlayerAnim->Update(1.0f);
	if (enemyBattle) currentEnemyAnim->Update(1.0f);
	else if (bossBattle) currentBossAnim->Update(1.0f);

	CombatLogic();

	if (enemyBattle)
	{
		if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->state == GuiControlState::NORMAL;
		else app->scene->splitButton->state = GuiControlState::LOCKED;
	}
	else if (bossBattle) app->scene->splitButton->state = GuiControlState::LOCKED;

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (enemyBattle) enemy->health -= enemy->health;
		else if (bossBattle) boss->health -= boss->health;
	}
}

void Combat::UpdateButtons()
{
	Scene* s = app->scene;
	if (combatState != SECOND_PLAYER_TURN)
	{
		s->moveButton->Update(0.0f);
		s->itemButton->Update(0.0f);
	}

	s->attackButton->Update(0.0f);
	s->escapeButton->Update(0.0f);
	s->splitButton->Update(0.0f);

	if (combatState != PLAYER_TURN)
	{
		s->secondAttackButton->Update(0.0f);
		s->protectButton->Update(0.0f);
		s->buffButton->Update(0.0f);
	}
	s = nullptr;
}

void Combat::UpdatePopUps()
{
	if (drawInventory)
	{
		smallMeatButton->Update(1.0f);
		largeMeatButton->Update(1.0f);
		featherButton->Update(1.0f);
		mantisButton->Update(1.0f);
		enemySplitButton->Update(1.0f);
		moneyButton->Update(1.0f);
	}
}

void Combat::CombatLogic()
{
	if (combatState == ENEMY_TURN)
	{
		if (enemyTimeWait < 60)
		{
			enemyTimeWait++; // Make enemy wait so it does not atack directly
			if (enemyTimeWait == 59)
			{
				EnemyAttackProbability();
				currentEnemyAnim = &enemy->moveAnim;

				switch (enemy->enemyClass)
				{
				case(EnemyClass::SMALL_WOLF): app->audio->SetFx(Effect::WOLF_ATTACK_FX); break;
				case(EnemyClass::BIRD): /*app->audio->SetFx(Effect::BIRD_ATTACK_FX);*/ break;
				case(EnemyClass::MANTIS): app->audio->SetFx(Effect::MANTIS_ATTACK_FX); break;
				}
			}
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
			else if (enemy->enemyClass == MANTIS)
			{
				if (enemy->attack == 1)
				{
					if (enemy->bullet[4].bulletRect.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 40) playerResponseAble = false;
				}
				else if (enemy->attack == 2)
				{
					if (steps != 3) if (enemy->colliderCombat.x < app->scene->player1->colliderCombat.x - enemy->colliderCombat.w - 30) playerResponseAble = false;
					else
					{
						if (enemy->mantisTimeAttack2 == 242) playerResponseAble = false;
					}
				}
			}
		}
	}
	else if (combatState == BOSS_TURN)
	{
		if (bossTimeWait < 60)
		{
			bossTimeWait++; // Make enemy wait so it does not atack directly
			if (bossTimeWait == 59)
			{
				BossAttackProbability();
				currentBossAnim = &boss->moveAnim;

				//app->audio->SetFx(Effect::BOSS_ATTACK_FX);
			}
		}
		else
		{
			BossAttack(boss->bossClass);

			PlayerResponse();

			//PLAYER RESPONSE FALSE LOGIC TODO
			switch (boss->bossClass)
			{
			case BossClass::BOSS_TUTORIAL:
				break;
			case BossClass::BOSS_I:
				break;
			case BossClass::BOSS_II:
				break;
			case BossClass::BOSS_III:
				break;
			}
		}
	}
	else if (combatState == PLAYER_TURN)
	{
		//DEBUG MODE
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) enemy->health -= enemy->health;

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
			if (secondPlayer) app->scene->player2->colliderCombat.x -= 3;
		}
		else
		{
			enemy->health = enemy->maxHealth;
			playerTimeEscape = 0;
			playerEscaped = true;
		}
	}
}

// DRAW

void Combat::Draw()
{
	DrawBakcground();

	if (debugCombat) DebugDraw();

	DrawPlayer();

	if (secondPlayer) DrawSecondPlayer();

	if (enemyBattle)
	{
		DrawEnemy();
		app->guiManager->DrawCombatInterface(enemy);
	}
	else if (bossBattle)
	{
		DrawBoss();
		app->guiManager->DrawCombatInterface(boss);
	}

	DrawButtons();

	DrawText();

	DrawPopUps();

	app->guiManager->DrawCursor();
}

void Combat::DrawPlayer()
{
	if (currentPlayerAnim == &app->scene->player1->cCrouchAnim)
	{
		app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x - 29, 400 - 52, &currentPlayerAnim->GetCurrentFrame());
	}
	else if (currentEnemyAnim == &app->scene->player1->cAttackAnim)
	{
		switch (steps)
		{
		case(0):
			
			break;
		case(1):
			
			break;
		case(2):
			
			break;
		case(3):
			
			break;
		default:
			app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x - 29, 400 - 52, &currentPlayerAnim->GetCurrentFrame());
			break;
		}
	}
	else
		app->render->DrawTexture(character1Spritesheet, app->scene->player1->colliderCombat.x - 29, app->scene->player1->colliderCombat.y - 52, &currentPlayerAnim->GetCurrentFrame());
}

void Combat::DebugDraw()
{
	app->render->DrawRectangle(app->scene->player1->colliderCombat, { 100, 3, 56, 150 });
	app->render->DrawRectangle(enemy->colliderCombat, { 255, 0, 0 , 150 });
	if (secondPlayer) app->render->DrawRectangle(app->scene->player2->colliderCombat, { 80, 100, 36, 255 });
	//BULLETS
	if (enemy->enemyClass == EnemyClass::MANTIS) for (int i = 0; i < 5; i++) enemy->bullet[i].DebugDraw();
}

void Combat::DrawSecondPlayer()
{
	currentSecondPlayerAnim = &app->scene->player2->secIdleAnim;
	currentSecondPlayerAnim->Update(1.0f);
	app->render->DrawTexture(character2Spritesheet, app->scene->player2->colliderCombat.x, app->scene->player2->colliderCombat.y, 2.0f, &currentSecondPlayerAnim->GetCurrentFrame(), false);
}

void Combat::DrawEnemy()
{
	if (currentEnemyAnim != nullptr)
	{
		if (enemy->enemyClass == EnemyClass::MANTIS)
		{
			app->render->DrawTexture(enemySpritesheet, enemy->colliderCombat.x - 10, enemy->colliderCombat.y - 20, 3, &currentEnemyAnim->GetCurrentFrame(), false);
			for (int i = 0; i < 5; i++) enemy->bullet[i].Draw();
		}
		else if (enemy->enemyClass == EnemyClass::BIRD)
			app->render->DrawTexture(enemySpritesheet, enemy->colliderCombat.x - 38, enemy->colliderCombat.y - 15, 3.5, &currentEnemyAnim->GetCurrentFrame(), false);
		else if (enemy->enemyClass == EnemyClass::SMALL_WOLF)
			app->render->DrawTexture(enemySpritesheet, enemy->colliderCombat.x - 10, enemy->colliderCombat.y - 20, 2, &currentEnemyAnim->GetCurrentFrame(), false);
	}
}

void Combat::DrawBoss()
{
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

void Combat::DrawPopUps()
{
	//INVENTORY
	if (drawInventory)
	{
		app->render->DrawTexture(combatInventory, 0, -20, 1, false);

		// DRAW BUTTONS
		smallMeatButton->Draw(1, true, true, ButtonType::LITTLE_BEEF_B);
		largeMeatButton->Draw(1, true, true, ButtonType::BIG_BEEF_B);
		featherButton->Draw(1, true, true, ButtonType::FEATHER_B);
		mantisButton->Draw(1, true, true, ButtonType::MANTIS_B);
		enemySplitButton->Draw(1, true, true, ButtonType::SPLIT_B);
		moneyButton->Draw(1, true, true, ButtonType::COIN_B);

		//DRAW TEXT
		if (smallMeatButton->state == GuiControlState::FOCUSED || smallMeatButton->state == GuiControlState::PRESSED) smallMeatDescription->Draw();
		if (largeMeatButton->state == GuiControlState::FOCUSED || largeMeatButton->state == GuiControlState::PRESSED) largeMeatDescription->Draw();
		if (featherButton->state == GuiControlState::FOCUSED || featherButton->state == GuiControlState::PRESSED) featherDescription->Draw();
		if (mantisButton->state == GuiControlState::FOCUSED || mantisButton->state == GuiControlState::PRESSED) mantisRodDescription->Draw();
		if (enemySplitButton->state == GuiControlState::FOCUSED || enemySplitButton->state == GuiControlState::PRESSED) enemySplitDescription->Draw();
		if (moneyButton->state == GuiControlState::FOCUSED || moneyButton->state == GuiControlState::PRESSED) moneyDescription->Draw();
	}

	//BUFFS MENU
	if (drawBuffMenu)
	{
		app->render->DrawRectangle({ 300, 200, 600, 400 }, {100, 210, 70, 200});
	}
}

void Combat::DrawButtons()
{
	if (!secondPlayer)
	{
		app->scene->attackButton->Draw();
		app->scene->moveButton->Draw();
		app->scene->itemButton->Draw();
		app->scene->escapeButton->Draw();
		app->scene->splitButton->Draw();
	}
	else if (secondPlayer)
	{
		if (combatState != SECOND_PLAYER_TURN)
		{
			app->scene->attackButton->Draw();
			app->scene->moveButton->Draw();
			app->scene->itemButton->Draw();
		}
		else if (combatState == SECOND_PLAYER_TURN)
		{
			app->scene->attackButton->Draw();
			app->scene->protectButton->Draw();
			app->scene->buffButton->Draw();
		}

		app->scene->escapeButton->Draw();
		app->scene->splitButton->Draw();
	}
}

void Combat::DrawText()
{
	turnText->Draw();

	if (!secondPlayer)
	{
		attackText->Draw();
		moveText->Draw();
		itemsText->Draw();
		escapeText->Draw();
		splitText->Draw();
	}
	else if (secondPlayer)
	{
		if (combatState != SECOND_PLAYER_TURN)
		{
			attackText->Draw();
			moveText->Draw();
			itemsText->Draw();
		}
		else if (combatState == SECOND_PLAYER_TURN)
		{
			attackText->Draw();
			protectText->Draw();
			buffText->Draw();
		}
		escapeText->Draw();
		splitText->Draw();
	}
}

// END

void Combat::EndBattleSolving()
{
	if (playerWin)
	{
		app->questManager->CheckKillQuest(enemy);

		if (enemyBattle) ItemDrop(enemy->enemyClass);
		app->scene->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);

		short int experience = 0;
		if (enemyBattle) experience = enemy->exp;
		else if (bossBattle) experience = boss->exp;

		short int id = app->entityManager->enemies.Find(enemy);
		app->entityManager->enemies.Del(app->entityManager->enemies.At(id));

		//TODO DELETE BOSS

		app->scene->SetScene(LEVEL_UP, experience);
	}
	else if (playerLose)
	{
		app->scene->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);
		if (enemyBattle) enemy->Refill();
		else if (bossBattle) boss->Refill();
		app->scene->SetScene(END_SCREEN);
	}
	else if (playerSplitWin)
	{
		app->scene->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);

		short int id = app->entityManager->enemies.Find(enemy);
		app->entityManager->enemies.Del(app->entityManager->enemies.At(id));

		//TODO DELETE BOSS

		app->scene->SetScene(LEVEL_UP);
	}
	else if (playerEscaped)
	{
		Scene* s = app->scene;
		s->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);

		if (enemyBattle) enemy->Refill();
		else if (bossBattle) boss->Refill();

		s->world->SetInmunityTime(PLAYER_INMUNITY_TIME);
		s->SetScene(WORLD, app->scene->world->GetPlace());
		s->world->AlignCameraPosition();
	}
}

// ---------------------------------------------------
// ---------------------------------------------------

void Combat::PlayerChoiceLogic()
{
	if (app->scene->attackPressed)
	{
		playerAttack = true;
		playerChoice = false;
		app->audio->SetFx(Effect::PLAYER_ATTACK_FX);
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
	int pDamage = app->scene->player1->strengthStat - enemy->defense;
	int pLuck = app->scene->player1->luckStat;

	if (steps == 0) damage += floor(15 * pDamage / 100);
	else if (steps == 1) damage += floor(35 * pDamage / 100);
	else if (steps == 2) damage += floor(65 * pDamage / 100);
	else if (steps == 3) damage += pDamage;

	if (damage < 1) //Normal enemy 0 damage, Boss 1 damage (for speedrunners) | To implement
	{
		damage = 1;
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
	int damage = app->scene->player2->strengthStat - enemy->defense;

	int variation = rand() % 3;
	bool negative = (bool)(rand() % 2);

	if (!negative) damage += variation;
	else if (negative) damage -= variation;

	if (damage <= 0) damage = 1;

	return damage;
}

int Combat::EnemyDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defenseStat;

	if (damage < 1) damage = 1;

	return damage;
}

int Combat::BulletDamageLogic()
{
	int damage = 0;
	damage += enemy->strength - app->scene->player1->defenseStat;

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
				enemy->colliderCombat.x = SMALLWOLF_C_X;
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
				enemy->colliderCombat.x = SMALLWOLF_C_X;
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
				enemy->colliderCombat.x = BIRD_C_X;
				enemy->colliderCombat.y = BIRD_C_Y;
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
				enemy->colliderCombat.x = BIRD_C_X;
				enemy->colliderCombat.y = BIRD_C_Y;
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
					app->scene->player2->colliderCombat.x -= 2;
				}
				else if (enemy->birdTimeAttack3 == 145)
				{
					app->scene->player1->colliderCombat.x -= 1;
					app->scene->player2->colliderCombat.x -= 1;
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

void Combat::BossAttack(BossClass boss)
{
	switch (boss)
	{
	case(BossClass::BOSS_TUTORIAL):
		break;
	case(BossClass::BOSS_I):
		break;
	case(BossClass::BOSS_II):
		break;
	case(BossClass::BOSS_III):
		break;
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
	currentPlayerAnim = &app->scene->player1->cAttackAnim;

	if (playerTimeAttack < 35)
	{
		playerTimeAttack++;
	}
	else
	{
		switch (enemy->enemyClass)
		{
		case(EnemyClass::SMALL_WOLF): app->audio->SetFx(Effect::WOLF_HURT_FX); break;
		case(EnemyClass::BIRD): /*app->audio->SetFx(Effect::BIRD_HURT_FX);*/ break;
		case(EnemyClass::MANTIS): app->audio->SetFx(Effect::MANTIS_HURT_FX); break;
		}

		enemy->health -= PlayerDamageLogic();
		LOG("Enemy Hit, EH: %d", enemy->health);

		playerTimeAttack = 0;
		playerAttack = false;

		currentPlayerAnim->Reset();

		PlayerPosReset();

		if (enemy->health > 0)
		{
			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
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
			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
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

		if (!secondPlayer)
		{
			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
		else if (secondPlayer) SecondPlayerTurn();
	}
}

void Combat::PlayerItemChoose()
{
	if (itemChoice)
	{
		Scene* s = app->scene;
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControl(B) == KEY_DOWN || app->input->GetControl(BACK) == KEY_DOWN)
		{
			playerChoice = true;
			drawInventory = false;
			playerItem = false;
		}
		else if (itemChoice && smallMeat > 0 && s->smallMeatPressed)
		{
			itemChoice = false;
			healPlayerSmall = true;
			smallMeat--;
		}
		else if (itemChoice && largeMeat > 0 && s->largeMeatPressed)
		{
			itemChoice = false;
			healPlayerLarge = true;
			largeMeat--;
		}
		else if (itemChoice && feather > 0 && s->featherPressed)
		{
			itemChoice = false;
			featherPlayerTurn = true;
			feather--;
		}
		else if (itemChoice && mantisLeg > 0 && s->mantisPressed)
		{
			itemChoice = false;
			protectPlayerTurn = true;
			mantisLeg--;
		}
		else if (itemChoice && splitedEnemy > 0 && s->enemySplitPressed)
		{
			itemChoice = false;
			enemyThrow = true;
			splitedEnemy--;
		}
		else if (itemChoice && splitedEnemy > 0 && s->moneyPressed)
		{
			itemChoice = false;
			moneyThrow = true;
			money--;
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
		if (enemyBattle) EnemyTurn();
		else if (bossBattle) BossTurn();
	}
}

void Combat::SecondPlayerBuff()
{
	if (buffChoice)
	{
		Scene* s = app->scene;
		Player* p = s->player1;
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->GetControl(B) == KEY_DOWN || app->input->GetControl(BACK) == KEY_DOWN)
		{
			secondPlayerChoice = true;
			drawBuffMenu = false;
			secondPlayerBuff = false;
		}
		else if (buffChoice && app->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
		{
			buffChoice = false;
			attackBuff = true;
			buffCooldown = 2;
			lastStatNotBuffed = p->strengthStat;
			p->strengthStat += (p->strengthStat * (ATTACK_BUFF / 100.0f));
		}
		else if (buffChoice && app->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
		{
			buffChoice = false;
			defenseBuff = true;
			buffCooldown = 2;
			lastStatNotBuffed = p->defenseStat;
			p->defenseStat += (p->defenseStat * (DEFENSE_BUFF / 100.0f));
		}
		else if (buffChoice && app->input->GetKey(SDL_SCANCODE_KP_3) == KEY_DOWN)
		{
			buffChoice = false;
			thirdBuff = true;
			buffCooldown = 1;
			//lastStatNotBuffed = p->strength;
			//p->elquesigui += (p->elquesigui * (THIRD_BUFF / 100.0f));
		}
	}
	else
	{
		drawBuffMenu = false;
		if (buffGenerationTime < 100)
		{
			buffGenerationTime++;
		}
		else
		{
			buffGenerationTime = 0;
			secondPlayerBuff = false;
			buffChoice = true;
			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
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
			playerItem = false;
			itemChoice = true;
			healPlayerSmall = false;

			app->scene->player1->health += HealPlayer(1);

			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
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

			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
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

			if (!secondPlayer)
			{
				if (enemyBattle)
				{
					if (enemyBattle) EnemyTurn();
					else if (bossBattle) BossTurn();
				}
				else if (bossBattle) BossTurn();
			}
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

			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
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
				if (!secondPlayer)
				{
					if (enemyBattle) EnemyTurn();
					else if (bossBattle) BossTurn();
				}
				else if (secondPlayer) SecondPlayerTurn();
			}
			else if (enemy->health <= 0)
			{
				PlayerWin();
			}
		}
	}
	else if (moneyThrow)
	{
		if (playerTimeMoneyThrow < 120)
		{
			playerTimeMoneyThrow++;
		}
		else
		{
			playerTimeMoneyThrow = 0;
			playerItem = false;
			itemChoice = true;
			moneyThrow = false;

			enemy->health -= 1;

			if (enemy->health > 0)
			{
				if (!secondPlayer)
				{
					if (enemyBattle)
					{
						if (enemyBattle) EnemyTurn();
						else if (bossBattle) BossTurn();
					}
					else if (bossBattle) BossTurn();
				}
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
		if (app->scene->player1->luckStat > 0)
		{
			int luck = rand() % 100;

			if (luckArray[luck]) //40% to lose
			{
				if (random < 2)
				{
					PlayerPosReset();

					if (!secondPlayer)
					{
						if (enemyBattle) EnemyTurn();
						else if (bossBattle) BossTurn();
					}
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

			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
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
	int damage = itemDamage + floor(app->scene->player1->strengthStat / 5) - enemy->defense;

	int damagePlus = rand() % 5;
	int negOrPos = rand() % 2;

	if (negOrPos == 0) damagePlus = -damagePlus;

	return damage + damagePlus;
}

void Combat::PlayerResponse()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetControl(UP_PAD) == KEY_DOWN)
	{
		if (!app->scene->player1->jump && playerResponseAble && !app->scene->player1->crouch)
		{
			app->scene->player1->jump = true;
			currentPlayerAnim = &app->scene->player1->cJumpAnim;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || app->input->GetControl(DOWN_PAD) == KEY_DOWN)
	{
		if (!app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump)
		{
			app->scene->player1->crouch = true;
			currentPlayerAnim = &app->scene->player1->cCrouchAnim;
		}
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
			else enemy->attack = 2;
		}
	}
}

void Combat::BossAttackProbability()
{
	switch (boss->bossClass)
	{
	case BossClass::BOSS_TUTORIAL:
		boss->attack = 1;
		break;
	case BossClass::BOSS_I:
		boss->attack = 1;
		break;
	case BossClass::BOSS_II:
		boss->attack = 1;
		break;
	case BossClass::BOSS_III:
		boss->attack = 1;
		break;
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

			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
	}
	else if (probabilityRange >= -3 && probabilityRange <= 3)
	{
		int random = rand() % 2;

		if (random == 0) PlayerEscape();
		else
		{
			playerAttack = false;

			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
	}
	else if (probabilityRange >= 4 && probabilityRange <= 7)
	{
		int random = rand() % 4;

		if (random == 0) PlayerEscape();
		else
		{
			playerAttack = false;

			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
	}
	else if (probabilityRange >= 8)
	{
		playerAttack = false;

		if (enemyBattle) EnemyTurn();
		else if (bossBattle) BossTurn();
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
					app->scene->player1->health -= floor((EnemyDamageLogic() - app->scene->player2->defenseStat) / 2);
					app->scene->player2->health -= ceil((EnemyDamageLogic() - app->scene->player2->defenseStat) / 2);
					LOG("Second Player Hit - PH: %d", app->scene->player2->health);
				}

				LOG("Player Hit - PH: %d", app->scene->player1->health);

				app->audio->SetFx(Effect::PLAYER_HURT_FX);
				
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
			if (app->scene->player1->godMode) LOG("Player Is Inmune");
			else
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
			int randMeat = rand() % 10;
			if (luckArray[a])
			{
				smallMeat += 1;
				largeMeat += 1;
			}
			else if (!luckArray[a])
			{
				if (randMeat < 7) smallMeat++;
				else largeMeat++;
				app->questManager->CheckGatherQuest(ItemType::LITTLE_BEEF_I, 1);
			}

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

void Combat::UpdateBuffs()
{
	if (buffCooldown > 0)
	{
		buffCooldown--;
		app->scene->buffButton->state = GuiControlState::LOCKED;
	}
	else
	{
		app->scene->buffButton->state = GuiControlState::NORMAL;

		if (attackBuff)
		{
			attackBuff = false;
			app->scene->player1->strengthStat = lastStatNotBuffed;
		}
		else if (defenseBuff)
		{
			defenseBuff = false;
			app->scene->player1->defenseStat = lastStatNotBuffed;
		}
		else if (thirdBuff)
		{
			thirdBuff = false;
			//app->scene->player1->strength = lastStatNotBuffed;
		}

		lastStatNotBuffed = 0;
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

	if (playerStepDenied && stepDeniedTurns > 0)
	{
		app->scene->moveButton->state = GuiControlState::NORMAL;
		playerStepDenied = false;
		stepDeniedTurns = 0;
	}
	else
	{
		stepDeniedTurns++;
	}

	if (secondPlayer) secondPlayerChoice = true;

	if (steps == 3) app->scene->moveButton->state = GuiControlState::LOCKED;
	else
	{
		app->scene->moveButton->state = GuiControlState::NORMAL;
	}

	app->scene->escapeButton->state = GuiControlState::NORMAL;
	app->scene->splitButton->state = GuiControlState::NORMAL;

	turnText->SetString("ENEMY TURN");

	switch (enemy->enemyClass)
	{
	case(EnemyClass::SMALL_WOLF): app->audio->SetFx(Effect::WOLF_TURN_FX); break;
	case(EnemyClass::BIRD): app->audio->SetFx(Effect::BIRD_TURN_FX); break;
	case(EnemyClass::MANTIS): app->audio->SetFx(Effect::MANTIS_TURN_FX); break;
	}
}

void Combat::BossTurn()
{
	LOG("BOSS TURN");
	LOG("Boss Health: %d", boss->health);

	combatState = BOSS_TURN;

	currentPlayerAnim = &app->scene->player1->cIdleAnim;
	currentPlayerAnim->Reset();

	playerResponseAble = true;
	playerChoice = true;
	secondPlayerChoice = true;

	if (secondPlayer) secondPlayerChoice = true;

	if (steps == 3) app->scene->moveButton->state = GuiControlState::LOCKED;
	else app->scene->moveButton->state = GuiControlState::NORMAL;

	app->scene->escapeButton->state = GuiControlState::NORMAL;

	turnText->SetString("BOSS TURN");

	//app->audio->SetFx(Effect::BOSS_TURN_FX);
}

void Combat::PlayerTurn()
{
	combatState = PLAYER_TURN;
	LOG("PLAYER TURN");
	LOG("Player Health: %d", app->scene->player1->health);

	if (wearFeather) wearFeather = false;
	if (wearMantisLeg) wearMantisLeg = false;

	if (app->scene->player2->health <= 0) SecondPlayerDie();

	if (steps < 3 && !playerStepDenied) app->scene->moveButton->state = GuiControlState::NORMAL;

	currentEnemyAnim = &enemy->idleAnim;

	turnText->SetString("PLAYER TURN");

	if (attackBuff || defenseBuff || thirdBuff) UpdateBuffs();

	app->audio->SetFx(Effect::PLAYER_TURN_FX);
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

	turnText->SetString("2ND PLAYER TURN");
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

	currentPlayerAnim = &app->scene->player1->cScapeAnim;
	currentPlayerAnim->Reset();
}

void Combat::PlayerSplitWin()
{
	LOG("ENEMY SPLITED");
	splitedEnemy++;
	combatState = SPLIT;
}