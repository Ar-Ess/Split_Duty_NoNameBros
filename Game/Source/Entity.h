#ifndef __ENTITY_H__
#define __ENTITY_H__

class Enemy;
class Item;
class NPC;

enum class EntityType
{
    PLAYER1,
    PLAYER2,
    ENEMY,
	BOSS,
    ITEM,
    MAP,
    NPC,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Update(float dt)
    {
        return true;
    }

public:

    EntityType type;
    bool active = true;
    bool renderable = false;
};

#endif // __ENTITY_H__