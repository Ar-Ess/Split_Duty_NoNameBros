#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Player.h"
#include "Input.h"
#include "DialogueManager.h"
#include "GuiManager.h"
#include "World.h"
#include "Audio.h"

NPC::NPC() : Entity(EntityType::NPC)
{
}

NPC::NPC(SDL_Rect colliderx) : Entity(EntityType::NPC)
{
	collider = colliderx;
}

NPC::~NPC()
{
}

void NPC::SetUp(iPoint position, NPCtype xtype, Places xplace, int xdialog, int directionx)
{
	collider.x = position.x;
	collider.y = position.y;
	contactCollider = collider;
	contactCollider.y += contactCollider.h / 2;
	contactCollider.h /= 2;
	npcType = xtype;
	place = xplace;
	dialog = xdialog;
	direction = directionx;

	switch (xtype)
	{
	case(NPCtype::KNIGHT_M_1):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight_m_spritesheet.png");
		break;
	case(NPCtype::KNIGHT_M_2):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight2_m_spritesheet.png");
		break;
	case(NPCtype::KNIGHT_M_3):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight3_m_spritesheet.png");
		break;
	case(NPCtype::KNIGHT_M_4):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight4_m_spritesheet.png");
		break;
	case(NPCtype::KNIGHT_F):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight_f_spritesheet.png");
		break;
	case(NPCtype::OLD):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/old_spritesheet.png");
		break;
	case(NPCtype::PRIEST):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/priest_spritesheet.png");
		break;
	case(NPCtype::BARMAN):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/barman_spritesheet.png");
		contactCollider.h += 40;
		break;
	case(NPCtype::WITCH):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/whitch_spritesheet.png");
		break;
	case(NPCtype::ADVENTURER):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/adventurer_spritesheet.png");
		break;
	case(NPCtype::FINAL_BOSS):
		//texture = app->tex->Load("Assets/Textures/Environment/sign.png");
		break;
	case(NPCtype::SHOP_WOMAN):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/shopper_spritesheet.png");
		contactCollider.h += 40;
		break;
	case(NPCtype::CITIZEN_1):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen1_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_2):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen2_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_3):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen3_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_4):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen4_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_5):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen5_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_6):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen6_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_7):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen7_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_8):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen8_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_9):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen9_spritesheet.png");
		break;
	case(NPCtype::CITIZEN_10):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen10_spritesheet.png");
		break;
	case(NPCtype::FORESTAL):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/forestal_spritesheet.png");
		break;
	case(NPCtype::CAT):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/cat_spritesheet.png");
		break;
	case(NPCtype::GARDENER):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/gardener_spritesheet.png");
		break;
	}
}

void NPC::Update(SDL_Rect pCollAmp)
{
	if (place == app->scene->world->GetPlace())
	{
		if (collisionUtils.CheckCollision(pCollAmp, contactCollider))
		{
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetControl(A) == KEY_DOWN)
			{
				app->dialogueManager->StartDialogue(dialog);
				app->audio->SetFx(Effect::NPC_TALK_FX);
			}
			app->scene->world->interactionActive = true;
		}
	}
}

void NPC::Draw()
{
	SDL_Rect spriteDir = {};
	if (direction == 1) spriteDir = { 32, 0, 32, 32 };
	else if (direction == 2) spriteDir = { 32, 32, 32, 32 };
	else if (direction == 3) spriteDir = { 32, 64, 32, 32 };
	else if (direction == 4) spriteDir = { 32, 96, 32, 32 };
	const SDL_Rect sprite = spriteDir;

	if (npcType != NO_NPC) app->render->DrawTexture(texture, collider.x - 10, collider.y, 2.3f, &sprite, false);
}

void NPC::Delete()
{
	app->tex->UnLoad(texture);
}
