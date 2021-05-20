#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"
enum EnemyClass;
enum BossClass;

class EntityManager : public Module
{
public:

	EntityManager();

	virtual ~EntityManager();

	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	bool CleanUp();

	Entity* CreateEntity(EntityType type);

	Entity* CreateEntity(EntityType type,EnemyClass enemy);

	Entity* CreateEntity(EntityType type, BossClass boss);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

public:

	List<Enemy*> enemies;
	List<Boss*> bosses;
	List<NPC*> NPCs;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
