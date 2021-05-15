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

void NPC::SetUp(iPoint position, NPCtype xtype, Places xplace, int xdialog)
{
	collider.x = position.x;
	collider.y = position.y;
	contactCollider = collider;
	contactCollider.y += contactCollider.h / 2;
	contactCollider.h /= 2;
	npcType = xtype;
	place = xplace;
	dialog = xdialog;

	switch (xtype)
	{
	case(NPCtype::KNIGHT):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/knight_spritesheet.png");
		break;
	case(NPCtype::OLD):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/old_spritesheet.png");
		break;
	case(NPCtype::PRIEST):
		texture = app->tex->Load("Assets/Textures/Characters/NPCs/priest_spritesheet.png");
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
	const SDL_Rect sprite = { 0,0,32,32 };
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
