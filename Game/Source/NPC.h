#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Collider.h"

#include "SDL/include/SDL.h"

class World;
enum Places;

enum NPCtype
{
    KNIGHT_M,
    KNIGHT_F,
    OLD,
    PRIEST,
    NO_NPC,
    BARMAN,
    WITCH,
    CITIZEN_1,
    ADVENTURER,
    SIGN,
    FINAL_BOSS
};

class NPC : public Entity
{
public:

    NPC();

    NPC(SDL_Rect colliderx);

    ~NPC();

    void SetUp(iPoint position, NPCtype type, Places place, int dialog, int direction = 1);

    void Update(SDL_Rect pCollAmp);

    void Draw();

    void Delete();

    Places GetPlace() const
    {
        return place;
    }

private:
    friend class World;

    SDL_Rect collider;
    SDL_Rect contactCollider;
    NPCtype npcType;
    Places place;
    SDL_Texture* texture = nullptr;
    int dialog;
    int direction;

    Collision collisionUtils;
};

#endif // __NPC_H__
