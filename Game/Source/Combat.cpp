#include "App.h"

#include "Scene.h"
#include "Textures.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiString.h"
#include "World.h"
#include "Audio.h"
#include "EntityManager.h"
#include "DialogueManager.h"

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
	switch(app->scene->enviroment)
	{
	case GRASSY_LANDS:
		background = app->tex->Load("Assets/Textures/Environment/grassy_lands_combat_scene.png");
		break;
	case AUTUM_FALLS:
		background = app->tex->Load("Assets/Textures/Environment/autumn_falls_combat_scene.png");
		break;
	case MOSSY_LANDS:
		background = app->tex->Load("Assets/Textures/Environment/mossy_rocks_combat_scene.png");
		break;
	case BOSS_LANDS:
		background = app->tex->Load("Assets/Textures/Environment/bosses_combat_scene.png");
		break;
	}
	combatInventory = app->tex->Load("Assets/Screens/combat_inventory.png");

	if (enemyBattle) EnemyStart();
	else if (bossBattle) BossStart();

	//Idle Animation Set
	currentPlayerAnim = &p->cIdleAnim;

	//Item Inventory amount
	ItemSetup(p->smallMeatCount, p->largeMeatCount, p->featherCount, p->mantisRodCount, p->splitedEnemyCount, p->moneyCount);

	//Bool preparation for combat
	BoolStart();

	app->scene->buffButton->state = GuiControlState::LOCKED;

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

	app->scene->escapeButton->state = GuiControlState::NORMAL;
}

void Combat::BossStart()
{
	combatState = BOSS_TURN;

	switch (boss->bossClass)
	{
	case(BossClass::BOSS_TUTORIAL):
		bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Bosses/golem_grassy_lands_spritesheet.png");
		tutorialBox = app->tex->Load("Assets/Textures/UI/tutorial_textbox.png");
		enterTexture = app->tex->Load("Assets/Textures/UI/enter_control.png");
		tutorialActive = true;
		jumpInstruction = false;
		tutorialStep = 0;
		wave[0] = { 1400, 0, 105, 60 };
		app->dialogueManager->StartDialogue(6);
		bossTAttack1Time = 0;
		jumpInstructionStep = 0;
		endOfTutorial = false;
		bossTAttack2Time = 0;
		tutorialText->bounds = {0, 0, 322, 241};
		tutorialText->SetString("You are this guy here,\nand right in front of you,\nthere is your opponent.");
		break;
	case(BossClass::BOSS_I):
		bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Bosses/golem_atumn_falls_spritesheet.png");
		shieldStep = 0;
		shield.x = shieldPos[shieldStep];
		shield = { 0, 285, 40, 215 };
		for (int i = 0; i < 2; i++) wave[i] = { 1400, 0, 105, 60 };
		bigWave = { 1400, 0, 120, 90 };
		boss1Attack4Time = 0;
		boss1Attack5Time = 0;
		boss1Attack6Time = 0;
		boss1Attack7Time = 0;
		break;
	case(BossClass::BOSS_II):
		bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Bosses/golem_mossy_rocks_spritesheet.png");
		for (int i = 0; i < 2; i++) spikeStep[i] = 0;
		for (int i = 0; i < 2; i++) spike[i] = { spikePos[spikeStep[i]], 488 - 30, 60, 30};
		for (int i = 0; i < 2; i++) wave[i] = { 1400, 0, 105, 60 };
		bigWave = { 1400, 0, 120, 90 };
		boss2Attack1Time = 0;
		boss2Attack2Time = 0;
		boss2Attack3Time = 0;
		boss2Attack4Time = 0;
		boss2Attack5Time = 0;
		boss2Attack6Time = 0;
		boss2Attack7Time = 0;
		bossIIStep = 0;
		spikesFattenator = false;
		b2heal = false;
		break;
	case(BossClass::BOSS_III):
		//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Bosses/golem_atumn_falls_spritesheet.png");
		iZone.SetUp(0, { 1300, 488 - 15,  70, 30 });
		iZone.tex = nullptr;
		rZone.SetUp(0, { 1300, 488 - 15,  70, 30 });
		rZone.tex = nullptr;
		bossIIIStep = 0;
		wave[0] = { 1400, 0, 105, 60 };
		bigWave = { 1400, 0, 120, 90 };
		pusher.SetUp(980 - 20, { 0, 285, 20, 215 }, false, false);
		pusher.tex = nullptr;
		boss3Attack1Time = 0;
		boss3Attack2Time = 0;
		boss3Attack3Time = 0;
		boss3Attack4Time = 0;
		boss3Attack5Time = 0;
		boss3Attack6Time = 0;
		boss3Attack7Time = 0;
		break;
	}

	currentBossAnim = &boss->idleAnim;

	app->scene->escapeButton->state = GuiControlState::LOCKED;
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
	app->tex->UnLoad(background);
	app->tex->UnLoad(combatInventory);
	if (tutorialBox != nullptr) app->tex->UnLoad(tutorialBox);
	if (enterTexture != nullptr) app->tex->UnLoad(enterTexture);

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

	once = false;

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


	if (!tutorialActive) CombatLogic();
	else TutorialLogic();

	if (jumpInstruction) JumpInstructionLogic();

	if (enemyBattle)
	{
		if (steps == 3 && enemy->health <= floor(20 * enemy->maxHealth / 100)) app->scene->splitButton->state == GuiControlState::NORMAL;
		else app->scene->splitButton->state = GuiControlState::LOCKED;
	}
	else if (bossBattle) app->scene->splitButton->state = GuiControlState::LOCKED;

	//DEBUG
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (enemyBattle) enemy->health -= enemy->health;
		else if (bossBattle) boss->health -= boss->health;
	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) app->scene->player1->health = 1;
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

		if (app->scene->player1->smallMeatCount == 0 && smallMeatButton->state != GuiControlState::DISABLED) smallMeatButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->smallMeatCount > 0 && smallMeatButton->state == GuiControlState::DISABLED) smallMeatButton->state = GuiControlState::NORMAL;
		if (app->scene->player1->largeMeatCount == 0 && largeMeatButton->state != GuiControlState::DISABLED) largeMeatButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->largeMeatCount > 0 && largeMeatButton->state == GuiControlState::DISABLED) largeMeatButton->state = GuiControlState::NORMAL;
		if (app->scene->player1->featherCount == 0 && featherButton->state != GuiControlState::DISABLED) featherButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->featherCount > 0 && featherButton->state == GuiControlState::DISABLED) featherButton->state = GuiControlState::NORMAL;
		if (app->scene->player1->mantisRodCount == 0 && mantisButton->state != GuiControlState::DISABLED) mantisButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->mantisRodCount > 0 && mantisButton->state == GuiControlState::DISABLED) mantisButton->state = GuiControlState::NORMAL;
		if (app->scene->player1->moneyCount == 0 && moneyButton->state != GuiControlState::DISABLED) moneyButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->moneyCount > 0 && moneyButton->state == GuiControlState::DISABLED) moneyButton->state = GuiControlState::NORMAL;
		if (app->scene->player1->splitedEnemyCount == 0 && enemySplitButton->state != GuiControlState::DISABLED) enemySplitButton->state = GuiControlState::DISABLED;
		else if (app->scene->player1->splitedEnemyCount > 0 && enemySplitButton->state == GuiControlState::DISABLED) enemySplitButton->state = GuiControlState::NORMAL;
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
			BossAttack();

			if (boss->bossClass == BOSS_I || boss->bossClass == BOSS_II)
			{
				if (boss->attack > 3) PlayerResponse();
			}
			else if (boss->bossClass == BOSS_TUTORIAL)
			{
				if (jumpInstructionStep == 0)
				{
					if (jumpInstruction) PlayerResponse();
				}
				else
				{
					PlayerResponse();
				}
			}
			else if (boss->bossClass == BOSS_III)
			{
				if (boss->attack > 3 && boss->attack < 8)
				{
					if (rZone.step != (steps + 1)) PlayerResponse();
				}
			}

			//PLAYER RESPONSE GOING FALSE LOGIC TODO
			switch (boss->bossClass)
			{
			case BossClass::BOSS_TUTORIAL:
				if (boss->attack == 1) if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				break;
			case BossClass::BOSS_I:
				if (boss->attack == 4) if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				if (boss->attack == 5) if (bigWave.x + bigWave.w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				if (boss->attack == 6)
				{
					if (wave[1].x + wave[1].w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
					if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x - 48 && !once)
					{
						playerHitAble = true;
						once = true;
					}
				}
				break;
			case BossClass::BOSS_II:
				if (boss->attack == 4) if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				if (boss->attack == 5) if (bigWave.x + bigWave.w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				if (boss->attack == 6)
				{
					if (wave[1].x + wave[1].w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
					if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x - 48 && !once)
					{
						playerHitAble = true;
						once = true;
					}
				}
				break;
			case BossClass::BOSS_III:
				if (boss->attack == 4)
				{
					if (bigWave.x + bigWave.w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
					if (wave[0].x + wave[0].w < app->scene->player1->colliderCombat.x - 48 && !once)
					{
						playerHitAble = true;
						once = true;
					}
				}
				else if (boss->attack == 7) if (boss->colliderCombat.x + boss->colliderCombat.w < app->scene->player1->colliderCombat.x) playerResponseAble = false;
				break;
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
		playerWin = true;
		//enemy->active = false;
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
		//enemy->active = false;
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
			if (enemyBattle) enemy->health = enemy->maxHealth;
			else if (bossBattle) boss->health = boss->maxHealth;
			playerTimeEscape = 0;
			playerEscaped = true;
		}
	}
}

void Combat::TutorialLogic()
{
	Scene* s = app->scene;

	if (tutorialStep != 0) if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) tutorialStep++;

	switch (tutorialStep)
	{
	case 0: // Dialog 6
		textBoxPos = { 1290,0 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		break;

	case 1: // You are this guy here, and right in front of you, there is your opponent.
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		break;

	case 2: // In the top part, you can see your life bar, as well as the life bar of your opponent.
		textBoxPos = { 470,60 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("In the top part, you can\nsee your life bar, as well\nas the life bar of\nyour opponent.");
		break;

	case 3: // Now, let`s go to the combat itsef. This is a turn based combat. When your turn arrives, you have to choose an option and create a great strategy to win this battle.
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Now, let`s go to the combat\nitsef. This is a turn based\ncombat. When your turn\narrives, you have to choose\nan option and create a\ngreat strategy to win\nthis battle.");
		break;

	case 4: // You have 5 options to choose, ATTACK, MOVE, ITEM, ESCAPE, SPLIT. Each one of this options consumes a turn, which means, after you select it, will be your opponent's turn.
		textBoxPos = { 470,290 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("You have 5 options to\nchoose, ATTACK, MOVE, ITEM,\nESCAPE, SPLIT.\nEach one of this options\nconsumes a turn, which\nmeans, after you select it,\nwill be your \nopponent's turn.");
		break;

	case 5: // The first option is ATTACK. When you choose it, you will cause damage to your opponent. Depending on your strength and their defense, you'll deal more or less damage.
		textBoxPos = { 130,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("The first option is ATTACK.\nWhen you choose it, you\nwill cause damage to your\nopponent. Depending on your\nstrength and their defense,\nyou'll deal more\nor less damage.");
		break;

	case 6: // But wait! Do you see these white marks on the ground? This is not good, the farther away you are from the opponent, the fewer damage you'll cause to it. 
		textBoxPos = { 340,210 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("But wait!\nDo you see these white\nmarks on the ground?\nThis is not good, the\nfarther away you are from\nthe opponent, the fewer\ndamage you'll cause to it.");
		break;

	case 7: // The first position inflicts a 15% of your total power, the second one inflicts a 30%, the third one a 60 %, and the last one the 100 % .But how do we get closer to the enemy ?
		textBoxPos = { 340,210 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("The first position inflicts\na 15 prcnt of your total\npower, the second one\ninflicts a 30 prcnt,\nthe third one a 60 prcnt,\nthe last one the 100 prcnt.\nBut how do we get closer\nto the enemy ?");
		break;

	case 8: // That's the function of the following option, MOVE. When choosing it, you'll move one step further. Be careful because the closer you are, dodging attacks will be more difficult.
		textBoxPos = { 310,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("That's the function of the\nfollowing option, MOVE.\nWhen choosing it, you'll\nmove one step further.\nBe careful because the\ncloser you are, dodging\nattacks will be\nmore difficult.");
		break;

	case 9: // Note that when you ATTACK, your position will reset the the first one.
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Note that when you ATTACK,\nyour position will reset\nthe the first one.");
		break;

	case 10: // Then we have the ITEM option. When you select it, a menu will open up. There you can choose the items you want to use, it also have a explanatory description for each one!
		textBoxPos = { 470,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Then we have an ITEM option\nWhen you select it, a menu\nwill open up. There you can\nchoose the items you want\nto use, it also have a\nexplanatory description\nfor each one!");
		break;

	case 11: // Next, we have the ESCAPE option. Since you are in a Boss Fight, you can not select it, but I'll tell you its use either way.
		textBoxPos = { 650,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Next, the ESCAPE option.\nSince you are in a\nBoss Fight, you can not\nselect it, but I'll tell\nyou its use either way.");
		break;

	case 12: // The ESCAPE function is used to escape a combat. Depending on your level and your oponent's one, will be easier or harder to escape.Note that escape makes you loose some coins...
		textBoxPos = { 650,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("The ESCAPE function is used\nto escape a combat.\nDepending on your level and\nyour oponent's one, will be\neasier or harder to escape.\nNote that escape makes you\nloose some coins...");
		break;

	case 13: // Finally, we have the SPLIT option. This is a secret power I am giving to you... Unfortunedly, I don't have enough power to split those golems, but I can with regular enemies.
		textBoxPos = { 780,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Finally, the SPLIT option.\nThis is a secret power I am\ngiving to you...\nUnfortunedly, I don't have\nenough power to split those\ngolems, but I can with\nregular enemies.");
		break;

	case 14: // This power will allow you to "split" the soul of your opponents and use it to deal damage in another combat. There's some conditions to be fulfilled before being able to use it.
		textBoxPos = { 780,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("This power will allow you\nto 'split' the soul of your\nopponents and use it to\ndeal damage in another\ncombat. There's some\nconditions to be fulfilled\nbefore being able\nto use it.");
		break;

	case 15: // First of all, your opponents life must be at the 20% or lower. Then, you must be in the last position. And third, you must have more health than your opponent.
		textBoxPos = { 780,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("First of all, the opponents\nlife must be at 20 prcnt\nor lower.\nThen, you must be in the\nlast position. And third,\nyou must have more health\nthan your opponent.");
		break;

	case 16: // This power is too big even for me, so in case you succesfully split your opponent (cause sometimes can fail), its life will be substracted from yours!
		textBoxPos = { 780,270 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("This power is too big even\nfor me, so in the case you\nsuccesfully split your\nopponent\n(cause sometimes can fail)\nits life will be\nsubstracted from yours!");
		break;

	case 17: // Be careful! You can die if you have less life than your opponent!
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Be careful! You can die if\nyou have less life than\nyour opponent!");
		break;

	case 18: // Being said that, let's allow your opponent to attack first! I will show you how to dodge it perfectly, so you can win without getting hurt!
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Being said that, let's\nallow your opponent to\nattack first! I will show\nyou how to dodge it\nperfectly, so you can win\nwithout getting hurt!");
		break;

	case 19:
		textBoxPos = { 1290,0 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("");
		break;
	}
}

void Combat::JumpInstructionLogic()
{
	if (jumpInstructionStep == 0 && jumpInstruction)
	{
		textBoxPos = { 330,150 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("NOW, JUMP!\n\n\nPRESS SPACE / W (keyboard) \nPRESS UP (gamepad)");
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN|| app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN|| app->input->GetControl(UP_PAD) == KEY_DOWN)
		{
			textBoxPos = { 1290,0 };
			jumpInstructionStep = 1;
			bossTAttack1Time++;
		}
	}

	if (jumpInstructionStep == 2)
	{
		textBoxPos = { 470,230 };
		tutorialText->bounds.x = textBoxPos.x + TUTORIAL_TEXT_MARGIN_X;
		tutorialText->bounds.y = textBoxPos.y + TUTORIAL_TEXT_MARGIN_Y;
		tutorialText->SetString("Well dodged!\nNote that some enemies or\nbosses will attack you from\nthe top, so in that\ncase, you should click\n\nLSHIFT or S (keyboard)\nor\nDOWN (gamepad) to crouch!\n\n Now, beat that golem up!");
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN|| app->input->GetControl(A) == KEY_DOWN)
		{
			textBoxPos = { 1290,0 };
			jumpInstructionStep = 3;
			jumpInstruction = false;
			endOfTutorial = true;
			bossTAttack1Time++;
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

	app->scene->DebugSteps();

	app->guiManager->DrawCursor();

	if (tutorialActive) DrawTutorial();

	if (jumpInstruction) DrawJumpInstructions();
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
	// PLAYER
	app->render->DrawRectangle(app->scene->player1->colliderCombat, { 100, 3, 56, 150 });

	// SECOND PLAYER
	if (secondPlayer) app->render->DrawRectangle(app->scene->player2->colliderCombat, { 80, 100, 36, 255 });

	//ENEMY
	if (enemyBattle)
	{
		app->render->DrawRectangle(enemy->colliderCombat, { 255, 0, 0 , 150 });
		//BULLETS
		if (enemy->enemyClass == EnemyClass::MANTIS) for (int i = 0; i < 5; i++) enemy->bullet[i].DebugDraw();
	}
	
	//BOSS
	if (bossBattle)
	{
		app->render->DrawRectangle(boss->colliderCombat, { 255, 0, 0 , 150 });

		switch (boss->bossClass)
		{
		case(BossClass::BOSS_TUTORIAL):
			app->render->DrawRectangle(wave[0], { 230, 30, 20, 100 });
			break;
		case(BossClass::BOSS_I):
			app->render->DrawRectangle(shield, { 20, 30, 230, 100 });
			for (int i = 0; i < 2; i++) app->render->DrawRectangle(wave[i], { 230, 30, 20, 100 });
			app->render->DrawRectangle(bigWave, { 230, 30, 20, 100 });
			break;
		case(BossClass::BOSS_II):
			for (int i = 0; i < 2; i++) app->render->DrawRectangle(spike[i], { 10, 130, 120, 100 });
			for (int i = 0; i < 2; i++) app->render->DrawRectangle(wave[i], { 230, 30, 20, 100 });
			app->render->DrawRectangle(bigWave, { 230, 30, 20, 100 });
			break;
		case(BossClass::BOSS_III):
			//bossSpritesheet = app->tex->Load("Assets/Textures/Characters/Enemies/Mantis/mantis_spritesheet.png");
			app->render->DrawRectangle(iZone.rect, { 230, 30, 20, 100 });
			app->render->DrawRectangle(rZone.rect, { 0, 30, 235, 100 });
			app->render->DrawRectangle(bigWave, { 230, 30, 20, 100 });
			app->render->DrawRectangle(wave[0], { 230, 30, 20, 100 });
			pusher.DebugDraw();
			break;
		}
	}
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
	app->render->DrawTexture(bossSpritesheet, boss->colliderCombat.x - 180, boss->colliderCombat.y - 140, 4.65f, 4.65f, false, &currentBossAnim->GetCurrentFrame());
}

void Combat::DrawBakcground()
{
	app->render->DrawTexture(background, 0, 0);
}

void Combat::DrawPopUps()
{
	//INVENTORY
	if (drawInventory)
	{
		int x = int(app->scene->EaseQuadX(iPoint(-1280, 0), iPoint(0, 0), false, 45));
		Uint8 o = Uint8(app->scene->EaseQuadX(iPoint(0, 0), iPoint(150, 0), false, 45));

		app->render->DrawRectangle({ 0, 0, 1280, 720 }, {0, 0, 0, o});
		app->render->DrawTexture(combatInventory, x, -20, 1, false);

		smallMeatButton->bounds.x += x;
		largeMeatButton->bounds.x += x;
		featherButton->bounds.x += x;
		mantisButton->bounds.x += x;
		enemySplitButton->bounds.x += x;
		moneyButton->bounds.x += x;

		smallMeatDescription->bounds.x += x;
		largeMeatDescription->bounds.x += x;
		featherDescription->bounds.x += x;
		mantisRodDescription->bounds.x += x;
		enemySplitDescription->bounds.x += x;
		moneyDescription->bounds.x += x;

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

		smallMeatButton->bounds.x -= x;
		largeMeatButton->bounds.x -= x;
		featherButton->bounds.x -= x;
		mantisButton->bounds.x -= x;
		enemySplitButton->bounds.x -= x;
		moneyButton->bounds.x -= x;

		smallMeatDescription->bounds.x -= x;
		largeMeatDescription->bounds.x -= x;
		featherDescription->bounds.x -= x;
		mantisRodDescription->bounds.x -= x;
		enemySplitDescription->bounds.x -= x;
		moneyDescription->bounds.x -= x;
	}
	else if (combatState == PLAYER_TURN) app->scene->iterations = 0;

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

void Combat::DrawTutorial()
{
	switch (tutorialStep)
	{
	case 0:
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		break;

	case 1: // You are this guy here, and right in front of you, there is your opponent.
		app->render->DrawRectangle({ 0, 0, 1280, 290 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 498, 1280, 222 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 290, 215, 208 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 275, 290, 705, 208 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1100, 290, 180, 208 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 2: // In the top part, you can see your life bar, as well as the life bar of your opponent.
		app->render->DrawRectangle({ 0, 0, 1280, 20 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 60, 1280, 660 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 20, 176, 40 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 276, 20, 748, 40 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1084, 20, 196, 40 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 3: // Now, let`s go to the combat itsef. This is a turn based combat. When your turn arrives, you have to choose an option and create a great strategy to win this battle.
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 4: // You have 5 options to choose, ATTACK, MOVE, ITEM, ESCAPE, SPLIT. Each one of this options consumes a turn, which means, after you select it, will be your opponent's turn.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 98, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1183, 570, 97, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.8f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 5: // The first option is ATTACK. When you choose it, you will cause damage to your opponent. Depending on your strength and their defense, you'll deal more or less damage.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 98, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 279, 570, 1001, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 6: // But wait! Do you see these white marks on the ground? This is not good, the farther away you are from the opponent, the fewer damage you'll cause to it. 
		app->render->DrawRectangle({ 0, 0, 1280, 488 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 520, 1280, 200 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 488, 229, 32 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 779, 488, 501, 32 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 7: // The first position inflicts a 15% of your total power, the second one inflicts a 30%, the third one a 60 %, and the last one the 100 % .But how do we get closer to the enemy ?
		app->render->DrawRectangle({ 0, 0, 1280, 488 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 520, 1280, 200 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 488, 229, 32 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 779, 488, 501, 32 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.8f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 8: // That's the function of the following option, MOVE. When choosing it, you'll move one step further. Be careful because the closer you are, dodging attacks will be more difficult.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 324, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 505, 570, 775, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.8f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 9: // Note that when you ATTACK, your position will reset the the first one.
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 10: // Then we have the ITEM option. When you select it, a menu will open up. There you can choose the items you want to use, it also have a explanatory description for each one!
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 550, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 731, 570, 549, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 11: // Next, we have the ESCAPE option. Since you are in a Boss Fight, you can not select it, but I'll tell you its use either way.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 776, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 957, 570, 323, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 12: // The ESCAPE function is used to escape a combat. Depending on your level and your oponent's one, will be easier or harder to escape.Note that escape makes you loose some coins...
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 776, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 957, 570, 323, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 13: // Finally, we have the SPLIT option. This is a secret power I am giving to you... Unfortunedly, I don't have enough power to split those golems, but I can with regular enemies.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 1002, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1183, 570, 97, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 14: // This power will allow you to "split" the soul of your opponents and use it to deal damage in another combat. There's some conditions to be fulfilled before being able to use it.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 1002, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1183, 570, 97, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.8f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 15: // First of all, your opponents life must be at the 20% or lower. Then, you must be in the last position. And third, you must have more health than your opponent.
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 1002, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1183, 570, 97, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 16: // This power is too big even for me, so in case you succesfully split your opponent (cause sometimes can fail), its life will be substracted from yours!
		app->render->DrawRectangle({ 0, 0, 1280, 570 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 652, 1280, 68 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 570, 1002, 82 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 1183, 570, 97, 82 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 17: // Be careful! You can die if you have less life than your opponent!
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 18: // Being said that, let's allow your opponent to attack first! I will show you how to dodge it perfectly, so you can win without getting hurt!
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.7f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 19:
		tutorialActive = false;
		break;
	}
	app->render->DrawTexture(enterTexture, 1120, 650, 4.0f, false);
}

void Combat::DrawJumpInstructions()
{
	switch (jumpInstructionStep)
	{
	case 0:
		app->render->DrawRectangle({ 0, 0, 1280, 290 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 498, 1280, 222 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 0, 290, 215, 208 }, { 0, 0, 0, 150 });
		app->render->DrawRectangle({ 275, 290, 1005, 208 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 0.6f, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		break;

	case 1:
		break;

	case 2:
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(tutorialBox, textBoxPos.x, textBoxPos.y, 1, 1, false, (const SDL_Rect*)0, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE, false);
		tutorialText->Draw();
		app->render->DrawTexture(enterTexture, 1120, 650, 4.0f, false);
		break;

	case 3:
		break;
	}
}

// END

void Combat::EndBattleSolving()
{
	if (playerWin)
	{
		if (enemyBattle) app->questManager->CheckKillQuest(enemy);

		if (enemyBattle) ItemDrop(enemy->enemyClass);
		app->scene->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);

		int experience = 0;
		if (enemyBattle)
		{
			experience = enemy->exp;
			short int id = app->entityManager->enemies.Find(enemy);
			app->entityManager->enemies.Del(app->entityManager->enemies.At(id));
		}

		if (app->scene->player2->health <= 0)
		{
			app->scene->player2->Refill();
			secondPlayer = true;
		}
		
		if (bossBattle)
		{
			experience = boss->exp;
			if (boss->bossClass == BOSS_TUTORIAL) app->scene->bossTBeat = true;
			else if (boss->bossClass == BOSS_I) app->scene->boss1Beat = true;
			else if (boss->bossClass == BOSS_II) app->scene->boss2Beat = true;
			else if (boss->bossClass == BOSS_III) app->scene->boss3Beat = true;
		}

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

		if (enemyBattle)
		{
			short int id = app->entityManager->enemies.Find(enemy);
			app->entityManager->enemies.Del(app->entityManager->enemies.At(id));
		}

		if (app->scene->player2->health <= 0)
		{
			app->scene->player2->Refill();
			secondPlayer = true;
		}

		//TODO DELETE BOSS

		app->scene->SetScene(LEVEL_UP);
	}
	else if (playerEscaped)
	{
		Scene* s = app->scene;
		s->player1->ItemSetup(smallMeat, largeMeat, feather, mantisLeg, splitedEnemy, money);

		if (enemyBattle) enemy->Refill();
		else if (bossBattle) boss->Refill();

		if (app->scene->player2->health <= 0)
		{
			app->scene->player2->Refill();
			secondPlayer = true;
		}

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
		if (enemyBattle)
		{
			short int probabilityRange = enemy->lvl - app->scene->player1->lvl;
			EscapeProbability(probabilityRange);
		}
		else if (bossBattle) EscapeProbability(-8);

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
	int enemyDefense = 0;

	if (enemyBattle) enemyDefense = enemy->defense;
	else if (bossBattle) enemyDefense = boss->defense;

	int pDamage = app->scene->player1->strengthStat - enemyDefense;
	int pLuck = app->scene->player1->luckStat;

	if (steps == 0) damage += floor(15 * pDamage / 100);
	else if (steps == 1) damage += floor(35 * pDamage / 100);
	else if (steps == 2) damage += floor(65 * pDamage / 100);
	else if (steps == 3) damage += pDamage;

	if (bossBattle)
	{
		switch (boss->bossClass)
		{
		case(BossClass::BOSS_I):
			if (steps < shieldStep) return 0;
			break;
		case(BossClass::BOSS_II):
			break;
		case(BossClass::BOSS_III):
			break;
		}
	}

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
		if (luckArray[a]) return damage + floor(20 * (pDamage - enemyDefense) / 100);
		else if (!luckArray[a]) return damage;
	}

	return 0;
}

int Combat::SecondPlayerDamageLogic()
{
	int enemyDefense = 0;
	if (enemyBattle) enemyDefense = enemy->defense;
	else if (bossBattle) enemyDefense = boss->defense;

	int damage = app->scene->player2->strengthStat - enemyDefense;

	int variation = rand() % 3;
	bool negative = (bool)(rand() % 2);

	if (!negative) damage += variation;
	else if (negative) damage -= variation;

	if (damage <= 0) damage = 1;

	if (bossBattle)
	{
		switch (boss->bossClass)
		{
		case(BossClass::BOSS_I):
			if (steps < shieldStep) return 0;
			break;
		case(BossClass::BOSS_II):
			break;
		case(BossClass::BOSS_III):
			break;
		}
	}

	return damage;
}

int Combat::EnemyDamageLogic()
{
	int damage = 0;
	int enemyStrength = 0;

	if (enemyBattle) enemyStrength = enemy->strength;
	else if (bossBattle) enemyStrength = boss->strength;

	damage += enemyStrength - app->scene->player1->defenseStat;

	if (damage < 1) damage = 1;

	return damage;
}

int Combat::BulletDamageLogic()
{
	int damage = 0;
	int enemyStrength = 0;

	if (enemyBattle) enemyStrength = enemy->strength;
	else if (bossBattle) enemyStrength = boss->strength;

	damage += enemyStrength - app->scene->player1->defenseStat;

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

void Combat::BossAttack()
{
	switch (boss->bossClass)
	{
	case(BossClass::BOSS_TUTORIAL):
		if (boss->attack == 1)
		{
			if (bossTAttack1Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				bossTAttack1Time++;
			}
			else if (bossTAttack1Time < 52)
			{
				if (bossTAttack1Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				bossTAttack1Time++;
			}
			else if (bossTAttack1Time < 200)
			{
				PlayerWaveHitLogic(0);
				if (bossTAttack1Time == 52) wave[0] = { 950, 428, 105, 60 };

				if (bossTAttack1Time == 102)
				{
					jumpInstruction = true;
				}
				else if (bossTAttack1Time == 130)
				{
					jumpInstructionStep = 2;
				}
				else
				{
					bossTAttack1Time++;
					wave[0].x -= 12;
				}
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				bossTAttack1Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 2)
		{
			if (bossTAttack2Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				bossTAttack2Time++;
			}
			else if (bossTAttack2Time < 52)
			{
				if (bossTAttack2Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				bossTAttack2Time++;
			}
			else if (bossTAttack2Time < 200)
			{
				PlayerWaveHitLogic(0);
				if (bossTAttack2Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 12;
				bossTAttack2Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				bossTAttack2Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		break;
	case(BossClass::BOSS_I):
		if (boss->attack == 1)
		{
			if (shield.x < shieldPos[1]) shield.x += 4;
			else
			{
				shieldStep = 1;
				shield.x = shieldPos[shieldStep];
				AfterBossAttack();
			}
		}
		else if (boss->attack == 2)
		{
			if (shield.x < shieldPos[2]) shield.x += 4;
			else
			{
				shieldStep = 2;
				shield.x = shieldPos[shieldStep];
				AfterBossAttack();
			}
		}
		else if (boss->attack == 3)
		{
			if (shield.x < shieldPos[3])
			{
				shield.x += 4;
				if (shield.x > shieldPos[3] - 40) boss->health++;
			}
			else
			{
				shieldStep = 3;
				shield.x = shieldPos[shieldStep];
				AfterBossAttack();
			}
		}
		else if (boss->attack == 4)
		{
			if (boss1Attack4Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack4Time++;
			}
			else if (boss1Attack4Time < 52)
			{
				if (boss1Attack4Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack4Time++;
			}
			else if (boss1Attack4Time < 200)
			{
				PlayerWaveHitLogic(0);
				if (boss1Attack4Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 12;
				boss1Attack4Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				boss1Attack4Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 5)
		{
			if (boss1Attack5Time < 50)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 0 }, false, 50);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack5Time++;
			}
			else if (boss1Attack5Time < 62)
			{
				if (boss1Attack5Time == 50) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack5Time++;
			}
			else if (boss1Attack5Time < 210)
			{
				PlayerBigWaveHitLogic();
				if (boss1Attack5Time == 62) bigWave = { 950, 403, 120, 85 };
				bigWave.x -= 14;
				boss1Attack5Time++;
			}
			else
			{
				bigWave = { 1400, 0, 120, 90 };
				boss1Attack5Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 6)
		{
			if (boss1Attack6Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack6Time++;
			}
			else if (boss1Attack6Time < 52)
			{
				if (boss1Attack6Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack6Time++;
			}
			else if (boss1Attack6Time < 240)
			{
				if (boss1Attack6Time < 92)
				{
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else if (boss1Attack6Time < 104)
				{
					if (boss1Attack6Time == 92) app->scene->iterations = 0;
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else
				{
					PlayerWaveHitLogic(1);
					if (boss1Attack6Time == 104) wave[1] = { 950, 428, 105, 60 };
					wave[1].x -= 14;
				}

				PlayerWaveHitLogic(0);
				if (boss1Attack6Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 14;
				boss1Attack6Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				wave[1] = { 1400, 0, 105, 60 };
				boss1Attack6Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 7)
		{
			if (boss1Attack7Time < 50)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, (BOSS_C_W * (-1)) - 100 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss1Attack7Time++;
			}
			else if (boss1Attack7Time < 100)
			{
				if (boss1Attack7Time == 50)
				{
					app->scene->iterations = 0;
					if (steps == 0) boss->colliderCombat.x = 249 - (BOSS_C_W / 2);
					else if (steps == 1) boss->colliderCombat.x = 419 - (BOSS_C_W / 2);
					else if (steps == 2) boss->colliderCombat.x = 589 - (BOSS_C_W / 2);
					else if (steps == 3) boss->colliderCombat.x = 759 - (BOSS_C_W / 2);
				}
				boss1Attack7Time++;
			}
			else if (boss1Attack7Time < 190)
			{
				boss->colliderCombat.y += 3;
				boss1Attack7Time++;
			}
			else if (boss1Attack7Time < 230)
			{
				boss1Attack7Time++;
			}
			else if (boss1Attack7Time < 240)
			{
				boss->colliderCombat.y += 17;
				PlayerHitLogic();
				boss1Attack7Time++;
			}
			else if (boss1Attack7Time < 285)
			{
				boss->colliderCombat.y -= 15;
				PlayerHitLogic();
				boss1Attack7Time++;
				if (boss1Attack7Time == 284) boss->colliderCombat.x = BOSS_C_X;
				if (boss3Attack5Time == 255) playerResponseAble = false;
			}
			else
			{
				if (boss->colliderCombat.y < BOSS_C_Y) boss->colliderCombat.y += 7;
				else
				{
					boss->colliderCombat = { BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H };
					boss1Attack7Time = 0;
					AfterBossAttack();
					playerHitAble = true;
				}
			}
		}
		break;
	case(BossClass::BOSS_II):
		if (boss->attack == 1)
		{
			if (boss2Attack1Time < 80)
			{
				boss2Attack1Time++;
			}
			else
			{
				int randm = (rand() % 3) + 1;

				if (spikeStep[0] == randm)
				{
					if (randm == 3) randm = 1;
					else randm++;
				}

				spikeStep[0] = randm;
				spike[0].x = spikePos[spikeStep[0]];

				boss2Attack1Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 2)
		{
			if (boss2Attack2Time < 100)
			{
				if (spikesFattenator)
				{
					if (boss2Attack2Time > 85)
					{
						for (int i = 0; i < 2; i++)
						{
							spike[i].h += 2;
							spike[i].y -= 2;
						}
					}
				}
				boss2Attack2Time++;
			}
			else
			{
				spikesFattenator = false;

				int randm = (rand() % 3) + 1;

				if (spikeStep[0] == randm)
				{
					if (randm == 3) randm = 1;
					else randm++;
				}

				spikeStep[0] = randm;
				spike[0].x = spikePos[spikeStep[0]];

				boss2Attack2Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 3)
		{
			if (boss2Attack3Time < 100)
			{
				boss2Attack3Time++;
				if (b2heal) if (boss2Attack3Time > 50)
				{
					if (boss2Attack3Time > 50) boss->health++;
				}
				b2heal = false;
			}
			else
			{
				//RANDOM POS FOR SPIKE 0
				int randm = (rand() % 3) + 1;
				//IF RANDOM POS == PREVIOUS POS, CHANGE IT
				if (spikeStep[0] == randm)
				{
					if (randm == 3) randm = 1;
					else randm++;
				}
				//UPDATE SPIKE 0 INFO
				spikeStep[0] = randm;
				spike[0].x = spikePos[spikeStep[0]];

				//RANDOM POS FOR SPIKE 1
				int randm1 = (rand() % 3) + 1;
				//IF RANDOM POS FOR SPIKE 1 == RANDOM POS FOR SPIKE 0, CHANGE IT
				if (randm1 == randm)
				{
					if (randm1 == 3) randm1 = 1;
					else randm1++;

					//IF RANDOM POS == PREVIOUS POS, CHANGE IT
					if (randm1 == spikeStep[1])
					{
						if (randm1 == 3) randm1 = 1;
						else randm1++;
					}
				}
				else if (randm1 == spikeStep[1])
				{
					if (randm1 == 3) randm1 = 1;
					else randm1++;

					if (randm1 == randm)
					{
						if (randm1 == 3) randm1 = 1;
						else randm1++;
					}
				}
				//UPDATE SPIKE 1 INFO
				spikeStep[1] = randm1;
				spike[1].x = spikePos[spikeStep[1]];

				boss2Attack3Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 4)
		{
			if (boss2Attack4Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack4Time++;
			}
			else if (boss2Attack4Time < 52)
			{
				if (boss2Attack4Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack4Time++;
			}
			else if (boss2Attack4Time < 200)
			{
				PlayerWaveHitLogic(0);
				if (boss2Attack4Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 12;
				boss2Attack4Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				boss2Attack4Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 5)
		{
			if (boss2Attack5Time < 50)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 0 }, false, 50);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack5Time++;
			}
			else if (boss2Attack5Time < 62)
			{
				if (boss2Attack5Time == 50) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack5Time++;
			}
			else if (boss2Attack5Time < 210)
			{
				PlayerBigWaveHitLogic();
				if (boss2Attack5Time == 62) bigWave = { 950, 403, 120, 85 };
				bigWave.x -= 14;
				boss2Attack5Time++;
			}
			else
			{
				bigWave = { 1400, 0, 120, 90 };
				boss2Attack5Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 6)
		{
			if (boss2Attack6Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack6Time++;
			}
			else if (boss2Attack6Time < 52)
			{
				if (boss2Attack6Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack6Time++;
			}
			else if (boss2Attack6Time < 240)
			{
				if (boss2Attack6Time < 92)
				{
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else if (boss2Attack6Time < 104)
				{
					if (boss2Attack6Time == 92) app->scene->iterations = 0;
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else
				{
					PlayerWaveHitLogic(1);
					if (boss2Attack6Time == 104) wave[1] = { 950, 428, 105, 60 };
					wave[1].x -= 14;
				}

				PlayerWaveHitLogic(0);
				if (boss2Attack6Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 14;
				boss2Attack6Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				wave[1] = { 1400, 0, 105, 60 };
				boss2Attack6Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 7)
		{
			if (boss2Attack7Time < 50)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, (BOSS_C_W * (-1)) - 100 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss2Attack7Time++;
			}
			else if (boss2Attack7Time < 100)
			{
				if (boss2Attack7Time == 50)
				{
					app->scene->iterations = 0;
					if (steps == 0) boss->colliderCombat.x = 249 - (BOSS_C_W / 2);
					else if (steps == 1) boss->colliderCombat.x = 419 - (BOSS_C_W / 2);
					else if (steps == 2) boss->colliderCombat.x = 589 - (BOSS_C_W / 2);
					else if (steps == 3) boss->colliderCombat.x = 759 - (BOSS_C_W / 2);
				}
				boss2Attack7Time++;
			}
			else if (boss2Attack7Time < 190)
			{
				boss->colliderCombat.y += 3;
				boss2Attack7Time++;
			}
			else if (boss2Attack7Time < 230)
			{
				boss2Attack7Time++;
			}
			else if (boss2Attack7Time < 240)
			{
				boss->colliderCombat.y += 17;
				PlayerHitLogic();
				boss2Attack7Time++;
			}
			else if (boss2Attack7Time < 285)
			{
				boss->colliderCombat.y -= 15;
				PlayerHitLogic();
				boss2Attack7Time++;
				if (boss2Attack7Time == 284) boss->colliderCombat.x = BOSS_C_X;
				if (boss3Attack5Time == 255) playerResponseAble = false;
			}
			else
			{
				if (boss->colliderCombat.y < BOSS_C_Y) boss->colliderCombat.y += 7;
				else
				{
					boss->colliderCombat = { BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H };
					boss2Attack7Time = 0;
					AfterBossAttack();
					playerHitAble = true;
				}
			}
		}
		break;
	case(BossClass::BOSS_III):
		if (boss->attack == 1)
		{
			if (boss3Attack1Time < 80)
			{
				boss3Attack1Time++;
			}
			else
			{
				int randm = (rand() % 4) + 1;

				if (randm == rZone.step)
				{
					if (randm == 4) randm = 1;
					else randm++;
				}

				rZone.step = randm;
				rZone.rect.x = zonePos[rZone.step];

				boss3Attack1Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 2)
		{
			if (boss3Attack2Time < 80)
			{
				boss3Attack2Time++;
			}
			else
			{
				int randm = (rand() % 4) + 1;

				if (randm == iZone.step)
				{
					if (randm == 4) randm = 1;
					else randm++;
				}

				iZone.step = randm;
				iZone.rect.x = zonePos[iZone.step];

				boss3Attack2Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 3)
		{
			if (boss3Attack3Time < 80)
			{
				boss3Attack3Time++;
			}
			else
			{
				//RANDOM STEP FOR iZone
				int randm = (rand() % 4) + 1;
				//IF RANDOM STEP == PREVIOUS STEP, CHANGE IT
				if (iZone.step == randm)
				{
					if (randm == 4) randm = 1;
					else randm++;
				}
				if (randm == 1) randm = 0;
				//UPDATE iZone INFO
				iZone.step = randm;
				iZone.rect.x = zonePos[iZone.step];

				//RANDOM STEP FOR rZone
				int randm1 = (rand() % 4) + 1;
				//IF RANDOM STEP FOR rZone == RANDOM STEP FOR iZone, CHANGE IT
				if (randm1 == randm)
				{
					if (randm1 == 4) randm1 = 1;
					else randm1++;

					//IF RANDOM STEP == PREVIOUS STEP, CHANGE IT
					if (randm1 == rZone.step)
					{
						if (randm1 == 4) randm1 = 1;
						else randm1++;
					}
				}
				else if (randm1 == rZone.step)
				{
					if (randm1 == 4) randm1 = 1;
					else randm1++;

					if (randm1 == randm)
					{
						if (randm1 == 4) randm1 = 1;
						else randm1++;
					}
				}
				if (randm1 == 1) randm1 = 0;
				//UPDATE rZone INFO
				rZone.step = randm1;
				rZone.rect.x = zonePos[rZone.step];

				boss3Attack3Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 4)
		{
			if (boss3Attack4Time < 40)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss3Attack4Time++;
			}
			else if (boss3Attack4Time < 52)
			{
				if (boss3Attack4Time == 40) app->scene->iterations = 0;

				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss3Attack4Time++;
			}
			else if (boss3Attack4Time < 240)
			{
				if (boss3Attack4Time < 92)
				{
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, 30 }, false, 40);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else if (boss3Attack4Time < 104)
				{
					if (boss3Attack4Time == 92) app->scene->iterations = 0;
					boss->colliderCombat.y = BOSS_C_Y;
					int y = (int)app->scene->EaseBossFallDown({ boss->colliderCombat.x, 30 }, { boss->colliderCombat.x, boss->colliderCombat.y }, false, 10);
					boss->colliderCombat.y += (y - BOSS_C_Y);
				}
				else
				{
					PlayerBigWaveHitLogic();
					if (boss3Attack4Time == 104) bigWave = { 950, 403, 120, 85 };
					bigWave.x -= 14;
				}

				PlayerWaveHitLogic(0);
				if (boss3Attack4Time == 52) wave[0] = { 950, 428, 105, 60 };
				wave[0].x -= 14;
				boss3Attack4Time++;
			}
			else
			{
				wave[0] = { 1400, 0, 105, 60 };
				bigWave = { 1400, 0, 120, 85 };
				boss3Attack4Time = 0;
				AfterBossAttack();
				playerHitAble = true;
			}
		}
		else if (boss->attack == 5)
		{
			if (boss3Attack5Time < 50)
			{
				boss->colliderCombat.y = BOSS_C_Y;
				int y = (int)app->scene->EaseBossJumpUp({ boss->colliderCombat.x, boss->colliderCombat.y }, { boss->colliderCombat.x, (BOSS_C_W * (-1)) - 100 }, false, 40);
				boss->colliderCombat.y += (y - BOSS_C_Y);
				boss3Attack5Time++;
			}
			else if (boss3Attack5Time < 100)
			{
				if (boss3Attack5Time == 50)
				{
					app->scene->iterations = 0;
					if (steps == 0) boss->colliderCombat.x = 249 - (BOSS_C_W / 2);
					else if (steps == 1) boss->colliderCombat.x = 419 - (BOSS_C_W / 2);
					else if (steps == 2) boss->colliderCombat.x = 589 - (BOSS_C_W / 2);
					else if (steps == 3) boss->colliderCombat.x = 759 - (BOSS_C_W / 2);
				}
				boss3Attack5Time++;
			}
			else if (boss3Attack5Time < 190)
			{
				boss->colliderCombat.y += 3;
				boss3Attack5Time++;
			}
			else if (boss3Attack5Time < 230)
			{
				boss3Attack5Time++;
			}
			else if (boss3Attack5Time < 240)
			{
				boss->colliderCombat.y += 17;
				PlayerHitLogic();
				boss3Attack5Time++;
			}
			else if (boss3Attack5Time < 285)
			{
				boss->colliderCombat.y -= 15;
				PlayerHitLogic();
				boss3Attack5Time++;
				if (boss3Attack5Time == 255) playerResponseAble = false;
				if (boss3Attack5Time == 284) boss->colliderCombat.x = BOSS_C_X;
			}
			else
			{
				if (boss->colliderCombat.y < BOSS_C_Y) boss->colliderCombat.y += 7;
				else
				{
					boss->colliderCombat = { BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H };
					playerHitAble = true;
					boss3Attack5Time = 0;
					AfterBossAttack();
				}
			}
		}
		else if (boss->attack == 6)
		{
			if (boss3Attack6Time < 80)
			{
				boss3Attack6Time++;
			}
			else
			{
				iZone.InstaKill();
				boss3Attack6Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 7) //JUMP FORWARD
		{
			if (boss3Attack7Time < 40)
			{
				boss->colliderCombat.x += 3;
				boss3Attack7Time++;
			}
			else if (boss3Attack7Time < 90)
			{
				boss3Attack7Time++;
				playerHitAble = true;
			}
			else if (boss3Attack7Time < 180)
			{
				if (boss3Attack7Time == 90) app->scene->iterations = 0;
				boss->colliderCombat.x = BOSS_C_X;
				boss->colliderCombat.y = BOSS_C_Y;
				fPoint point = app->scene->EaseQuadXY({ boss->colliderCombat.x, boss->colliderCombat.y }, { -70, boss->colliderCombat.y - 80 }, false, 90, 20);
				boss->colliderCombat.x += ((int)point.x - BOSS_C_X);
				boss->colliderCombat.y += ((int)point.y - BOSS_C_Y);
				PlayerHitLogic();
				boss3Attack7Time++;
			}
			else if (boss3Attack7Time < 255)
			{
				if (boss3Attack7Time == 180)
				{
					app->scene->iterations = 0;
					boss->colliderCombat = { 1280, BOSS_C_Y, BOSS_C_W, BOSS_C_H };
				}

				boss->colliderCombat.x -= 4;
				boss3Attack7Time++;
			}
			else
			{
				boss->colliderCombat = { BOSS_C_X, BOSS_C_Y, BOSS_C_W, BOSS_C_H };
				playerHitAble = true;
				boss3Attack7Time = 0;
				AfterBossAttack();
			}
		}
		else if (boss->attack == 8) //SHIELD FORCE PUSH BACK
		{
			Player* p = app->scene->player1;
			Player* p2 = app->scene->player2;
			if (pusher.rect.x + pusher.rect.w > 0) //TODO FUTURE, BEING ABLE TO SHIFT TO ABOID PUSHING
			{
				pusher.Move();
				if (collisionUtils.CheckCollision(p->colliderCombat, pusher.rect)) pusher.pushing = true;
				if (pusher.pushing)
				{
					if (p->colliderCombat.x + (+p->colliderCombat.w / 2) <= 249)
					{
						PlayerPosReset();
						pusher.pushing = false;
					}
					else
					{
						p->colliderCombat.x -= 12;
						if (secondPlayer) p2->colliderCombat.x -= 12;
					}
				}
			}
			else
			{
				pusher.Restart();
				AfterBossAttack();
			}
			p = nullptr;
			p2 = nullptr;
		}
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

void Combat::AfterBossAttack()
{
	if (app->scene->player1->health > 0)
	{
		PlayerTurn();
	}
	else if (app->scene->player1->health <= 0)
	{
		PlayerDie();
	}

	bossTimeWait = 0;
}

void Combat::PlayerAttack()
{
	int enemyHealth = 0;

	currentPlayerAnim = &app->scene->player1->cAttackAnim;

	if (playerTimeAttack < 35)
	{
		playerTimeAttack++;
	}
	else
	{
		if (enemyBattle)
		{
			switch (enemy->enemyClass)
			{
			case(EnemyClass::SMALL_WOLF): app->audio->SetFx(Effect::WOLF_HURT_FX); break;
			case(EnemyClass::BIRD): /*app->audio->SetFx(Effect::BIRD_HURT_FX);*/ break;
			case(EnemyClass::MANTIS): app->audio->SetFx(Effect::MANTIS_HURT_FX); break;
			}

			enemy->health -= PlayerDamageLogic();
			enemyHealth = enemy->health;

			LOG("Enemy Hit, EH: %d", enemy->health);
		}
		else if (bossBattle)
		{
			//app->audio->SetFx(Effect::BOSS_HURT_FX);

			boss->health -= PlayerDamageLogic();
			enemyHealth = boss->health;

			LOG("Boss Hit, EH: %d", boss->health);
		}

		playerTimeAttack = 0;
		playerAttack = false;

		currentPlayerAnim->Reset();

		PlayerPosReset();

		if (enemyHealth > 0)
		{
			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
			else if (secondPlayer) SecondPlayerTurn();
		}
		else if (enemyHealth <= 0)
		{
			PlayerWin();
		}
	}
}

void Combat::SecondPlayerAttack()
{
	int enemyHealth = 0;

	if (enemyBattle) enemyHealth = enemy->health;
	else if (bossBattle) enemyHealth = boss->health;

	if (secondPlayerTimeAttack < 215)
	{
		secondPlayerTimeAttack++;

		app->scene->player2->colliderCombat.x += 6;

		if (app->scene->player2->colliderCombat.x > 1280) app->scene->player2->colliderCombat.x = 0;
	}
	else
	{
		if (enemyBattle)
		{
			switch (enemy->enemyClass)
			{
			case(EnemyClass::SMALL_WOLF): app->audio->SetFx(Effect::WOLF_HURT_FX); break;
			case(EnemyClass::BIRD): /*app->audio->SetFx(Effect::BIRD_HURT_FX);*/ break;
			case(EnemyClass::MANTIS): app->audio->SetFx(Effect::MANTIS_HURT_FX); break;
			}

			enemy->health -= SecondPlayerDamageLogic();
			LOG("Enemy Hit, EH: %d", enemy->health);
		}
		else if (bossBattle)
		{
			//app->audio->SetFx(Effect::BOSS_HURT_FX);

			boss->health -= SecondPlayerDamageLogic();
			LOG("Enemy Hit, EH: %d", boss->health);
		}

		secondPlayerTimeAttack = 0;
		secondPlayerAttack = false;

		SecondPlayerPosReset();

		if (enemyHealth > 0)
		{
			if (enemyBattle) EnemyTurn();
			else if (bossBattle) BossTurn();
		}
		else if (enemyHealth <= 0)
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

		if (bossBattle && boss->bossClass == BOSS_II)
		{
			for (int i = 0; i < 2; i++)
			{
				if (spikeStep[i] == steps)
				{
					int damage = EnemyDamageLogic();
					if (bossIIStep == 1)
					{
						damage -= 17;
						if (damage <= 0) damage = 1;
						app->scene->player1->health -= damage;
					}
					else
					{
						damage -= 13;
						if (damage <= 0) damage = 1;
						app->scene->player1->health -= damage;
					}
				}
			}
		}

		if (app->scene->player1->health > 0)
		{
			if (!secondPlayer)
			{
				if (enemyBattle) EnemyTurn();
				else if (bossBattle) BossTurn();
			}
			else if (secondPlayer) SecondPlayerTurn();
		}
		else if (app->scene->player1->health <= 0)
		{
			PlayerDie();
		}
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
				if (enemyBattle) EnemyTurn();
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
		int enemyHealth = 0;

		if (enemyBattle) enemyHealth = enemy->health;
		else if (bossBattle) enemyHealth = boss->health;

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

			if (enemyBattle) enemy->health -= EnemyItemDamage();
			else if (bossBattle) boss->health -= EnemyItemDamage();

			if (enemyHealth > 0)
			{
				if (!secondPlayer)
				{
					if (enemyBattle) EnemyTurn();
					else if (bossBattle) BossTurn();
				}
				else if (secondPlayer) SecondPlayerTurn();
			}
			else if (enemyHealth <= 0)
			{
				PlayerWin();
			}
		}
	}
	else if (moneyThrow)
	{
		int enemyHealth = 0;

		if (enemyBattle) enemyHealth = enemy->health;
		else if (bossBattle) enemyHealth = boss->health;

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

			if (enemyBattle) enemy->health -= 1;
			else if (bossBattle) boss->health -= 1;

			if (enemyHealth > 0)
			{
				if (!secondPlayer)
				{
					if (enemyBattle) EnemyTurn();
					else if (bossBattle) BossTurn();
				}
				else if (secondPlayer) SecondPlayerTurn();
			}
			else if (enemyHealth <= 0)
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
					if (enemyBattle) app->scene->player1->health -= enemy->health;
					else if (bossBattle) app->scene->player1->health -= boss->health;

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
			if (enemyBattle) app->scene->player1->health -= enemy->health;
			else if (bossBattle) app->scene->player1->health -= boss->health;

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

	int enemyDefense = 0;

	if (enemyBattle) enemyDefense = enemy->defense;
	else if (bossBattle) enemyDefense = boss->defense;

	int damage = itemDamage + floor(app->scene->player1->strengthStat / 5) - enemyDefense;

	int damagePlus = rand() % 5;
	int negOrPos = rand() % 2;

	if (negOrPos == 0) damagePlus = -damagePlus;

	return damage + damagePlus;
}

int Combat::EnemyStabDamage()
{
	int realStab = ceil((app->scene->player1->stabStat * 20) / 25);
	if (realStab < 1) realStab = 1;

	int damage = ceil((enemy->health * realStab) / 100);
	if (damage < 1) damage = 1;

	return damage;
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
		if (!app->scene->player1->crouch && playerResponseAble && !app->scene->player1->jump && !jumpInstruction)
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
		if (!endOfTutorial) boss->attack = 1;
		else boss->attack = 2;
		break;
	case BossClass::BOSS_I:
		if (shieldStep == 0) boss->attack = 1;
		else if (boss->health <= 30 && boss->health >= 15 && shieldStep < 2) boss->attack = 2;
		else if (boss->health < 15 && shieldStep < 3) boss->attack = 3;
		else
		{
			if (shieldStep == 1) boss->attack = 4;
			else if (shieldStep == 2)
			{
				int randm = rand() % 2;
				if (randm == 0) boss->attack = 5;
				else boss->attack = 6;
			}
			else if (shieldStep == 3)
			{
				int randm = rand() % 6;
				if (randm < 1) boss->attack = 6;
				else if (randm < 3) boss->attack = 5;
				else boss->attack = 7;
			}
		}
		break;
	case BossClass::BOSS_II:
		if (bossIIStep == 0)
		{
			boss->attack = 1;
			bossIIStep = 1;
		}
		else if (bossIIStep == 1)
		{
			if (boss->health < 40)
			{
				bossIIStep = 2;
				boss->attack = 2;
				spikesFattenator = true;
			}
			else
			{
				int randm = rand() % 7;

				if (boss->attack == 1) randm = 2;

				if (randm < 2) boss->attack = 1;
				else boss->attack = 4;
			}
		}
		else if (bossIIStep == 2)
		{
			if (boss->health < 20)
			{
				bossIIStep = 3;
				boss->attack = 3;
				b2heal = true;
			}
			else
			{
				int randm = rand() % 7;

				if (boss->attack == 1) randm = 2;

				if (randm < 2) boss->attack = 1;
				else
				{
					int randm1 = rand() % 2;
					if (randm1 == 0) boss->attack = 5;
					else boss->attack = 6;
				}
			}
		}
		else if (bossIIStep == 3)
		{
			int randm = rand() % 9;

			if (boss->attack == 1) randm = 2;

			if (randm < 2) boss->attack = 3;
			else if (randm < 6)
			{
				int randm1 = rand() % 2;
				if (randm1 == 0) boss->attack = 5;
				else boss->attack = 6;
			}
			else boss->attack = 7;
		}
		break;
	case BossClass::BOSS_III:
		if (bossIIIStep == 0)
		{
			boss->attack = 1;
			bossIIIStep = 1;
		}
		else if (bossIIIStep == 1)
		{
			if (boss->health < 104)
			{
				bossIIIStep = 2;
				boss->attack = 2;
				rZone.step = 0;
				rZone.rect.x = zonePos[rZone.step];
			}
			else
			{
				int randm = rand() % 8;

				if (boss->attack == 1) randm = 3;

				if (randm < 3) boss->attack = 1;
				else
				{
					int fifty = rand() % 2;
					if (fifty == 0) boss->attack = 4;
					else boss->attack = 5;
				}

				if (rZone.step == (steps + 1))
				{
					int fifty = rand() % 2;
					if (fifty == 0) boss->attack = 4;
					else boss->attack = 5;
				}
			}
		}
		else if (bossIIIStep == 2)
		{
			if (boss->health < 52)
			{
				bossIIIStep = 3;
				boss->attack = 3;
				iZone.step = 0;
				iZone.rect.x = zonePos[iZone.step];
			}
			else
			{
				int randm = rand() % 8;

				if (boss->attack == 2) randm = 3;

				if (randm < 3) boss->attack = 2;
				else
				{
					int fifty = rand() % 2;
					if (fifty == 0) boss->attack = 4;
					else boss->attack = 5;
				}

				if (rZone.step == (steps + 1))
				{
					int fifty = rand() % 2;
					if (fifty == 0) boss->attack = 4;
					else boss->attack = 5;
				}

				if (iZone.step == (steps + 1)) boss->attack = 6;
			}
		}
		else if (bossIIIStep == 3)
		{
			int randm = rand() % 14;
			if (boss->attack == 3) randm = 5;

			if (randm < 5) boss->attack = 3;
			else
			{
				int rndm = rand() % 13;
				if (rndm < 4) boss->attack = 4;
				else if (rndm < 8) boss->attack = 7;
				else if (rndm < 11) boss->attack = 5;
				else if (rndm < 13)
				{
					if (steps == 0) boss->attack = 8;
					else
					{
						rndm = rand() % 8;
						if (rndm < 3) boss->attack = 4;
						else if (rndm < 6) boss->attack = 7;
						else if (rndm < 8) boss->attack = 5;
					}
				}
			}

			if (iZone.step == (steps + 1)) boss->attack = 6;
		}
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
	SDL_Rect colliderCombat = {};
	if (enemyBattle) colliderCombat = enemy->colliderCombat; 
	else if (bossBattle) colliderCombat = boss->colliderCombat;

	if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, colliderCombat))
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

void Combat::PlayerWaveHitLogic(int i)
{
	if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, wave[i]))
	{
		if (app->scene->player1->godMode) LOG("Player is inmune");
		else
		{
			playerHitAble = false;
			if (!wearMantisLeg)
			{
				int waveReduction = 0;
				if (boss->attack == 6) waveReduction = 2;
				if (!secondPlayerProtection) app->scene->player1->health -= EnemyDamageLogic() - waveReduction;
				else if (secondPlayerProtection)
				{
					app->scene->player1->health -= floor((EnemyDamageLogic() - app->scene->player2->defenseStat) / 2) - waveReduction;
					app->scene->player2->health -= ceil((EnemyDamageLogic() - app->scene->player2->defenseStat) / 2) - waveReduction;
					LOG("Second Player Hit - PH: %d", app->scene->player2->health);
				}

				LOG("Player Hit - PH: %d", app->scene->player1->health);

				app->audio->SetFx(Effect::PLAYER_HURT_FX);

			}
			else if (wearMantisLeg) wearMantisLeg = false;
		}
	}
}

void Combat::PlayerBigWaveHitLogic()
{
	if (playerHitAble && collisionUtils.CheckCollision(app->scene->player1->colliderCombat, bigWave))
	{
		if (app->scene->player1->godMode) LOG("Player is inmune");
		else
		{
			playerHitAble = false;
			if (!wearMantisLeg)
			{
				if (!secondPlayerProtection) app->scene->player1->health -= EnemyDamageLogic() + 3;
				else if (secondPlayerProtection)
				{
					app->scene->player1->health -= floor((EnemyDamageLogic() + 4 - app->scene->player2->defenseStat) / 2);
					app->scene->player2->health -= ceil((EnemyDamageLogic() + 4 - app->scene->player2->defenseStat) / 2);
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
						app->scene->player1->health -= floor((EnemyDamageLogic() - (app->scene->player2->defenseStat / 2)) / 2);
						app->scene->player2->health -= ceil((EnemyDamageLogic() - (app->scene->player2->defenseStat / 2)) / 2);
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
				//app->questManager->CheckGatherQuest(ItemType::LITTLE_BEEF_I, 1);
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
		//app->scene->buffButton->state = GuiControlState::NORMAL;

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

	once = false;

	if (app->scene->player2->health <= 0) SecondPlayerDie();

	if (steps < 3 && !playerStepDenied) app->scene->moveButton->state = GuiControlState::NORMAL;

	if (enemyBattle) currentEnemyAnim = &enemy->idleAnim;
	else if (bossBattle) currentBossAnim = &boss->idleAnim;

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

void InstaZone::InstaKill()
{
	if (app->scene->player1->health < 2) app->scene->player1->health = 0;
	else app->scene->player1->health = 1;
}
