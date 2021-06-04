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
	case(NPCtype::KNIGHT_M):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight_m_spritesheet.png");
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
	case(NPCtype::CITIZEN_1):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/citizen1_spritesheet.png");
		break;
	case(NPCtype::WITCH):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/whitch_spritesheet.png");
		break;
	case(NPCtype::ADVENTURER):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/adventurer_spritesheet.png");
		break;
	case(NPCtype::SIGN):
		texture = app->tex->Load("Assets/Textures/Environment/sign.png");
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
		}
	}
}

void NPC::Draw()
{
	SDL_Rect spriteDir;
	if (direction == 1) spriteDir = { 32, 0, 32, 32 };
	else if (direction == 2) spriteDir = { 32, 32, 32, 32 };
	else if (direction == 3) spriteDir = { 32, 64, 32, 32 };
	else if (direction == 4) spriteDir = { 32, 96, 32, 32 };
	const SDL_Rect sprite = spriteDir;

	if (npcType != NO_NPC)
	{
		if (npcType != SIGN) app->render->DrawTexture(texture, collider.x - 10, collider.y, 2.3f, &sprite, false);
		else app->render->DrawTexture(texture, collider.x - 10, collider.y);
	}
}

void NPC::Delete()
{
	app->tex->UnLoad(texture);
}
