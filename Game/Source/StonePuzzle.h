#ifndef __STONEPUZZLE_H__
#define __STONEPUZZLE_H__

#include "World.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Rect;
struct SDL_Texture;
struct Collision;

struct Stone
{
    Stone();
    Stone(SDL_Rect rectx, SDL_Texture* texx);
    ~Stone();

    void SetPosition(int x, int y)
    {
        rect.x = x;
        rect.y = y;
    }

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

};

class StonePuzzle
{
public:
    StonePuzzle();
    StonePuzzle(int stoneAmount, SDL_Rect pathx);
    ~StonePuzzle();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void DebugDraw();

    void SwitchLogic();

    void RiverTrackLogic();

public:
    List<Stone> stone;
    bool finished = false;

    Collision collisionUtils;

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    SDL_Rect path = {};

    bool stoneInWater = false;
};

#endif // __STONEPUZZLE_H_

