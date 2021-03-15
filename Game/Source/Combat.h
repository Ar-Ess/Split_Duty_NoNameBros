#ifndef __COMBAT_H__
#define __COMBAT_H__

#include "Point.h"
#include "SString.h"

class Enemy;

class Combat
{
public:

    Combat() {}

    void Restart();

public:

    Enemy* enemy;
};

#endif // __COMBAT_H__
