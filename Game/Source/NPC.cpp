#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Player.h"
#include "Input.h"
#include "DialogueManager.h"
#include "GuiManager.h"
#include "World.h"

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

void NPC::SetUp(iPoint position, NPCtype xtype, Places xplace)
{
	collider.x = position.x;
	collider.y = position.y;
	contactCollider = collider;
	contactCollider.y += contactCollider.h / 2;
	contactCollider.h /= 2;
	npcType = xtype;
	place = xplace;
	//if (xtype == CITICIZEN) texture = app->tex->Load("Assets/Blablabla");
}

void NPC::Update(SDL_Rect pCollAmp)
{
	if (place == app->scene->world->GetPlace())
	{
		if (collisionUtils.CheckCollision(pCollAmp, contactCollider))
		{
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				app->dialogueManager->StartDialogue(1);
				app->guiManager->debugGui = true;
			}
		}
	}
}

void NPC::Draw()
{
}

void NPC::Delete()
{
	//app->tex->UnLoad(texture);
}
