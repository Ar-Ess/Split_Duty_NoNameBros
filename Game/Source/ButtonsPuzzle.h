#ifndef __BUTTONPUZZLE_H__
#define __BUTTONPUZZLE_H__

#include "World.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Rect;
struct SDL_Texture;
class Collision;

struct Button
{
    Button();
    Button(SDL_Rect rectx);
    ~Button();

    void SetPosition(int x, int y, uint orderx)
    {
        rect.x = x;
        rect.y = y;
        order = orderx;
    }

    SDL_Rect rect;

    bool active = false;

    uint order;
};

class ButtonPuzzle
{
public:
    ButtonPuzzle();
    ButtonPuzzle(int stoneAmount, int width, int height, SDL_Rect bridge1x, SDL_Rect bridge2x, SDL_Rect bridge3x);
    ~ButtonPuzzle();

    void Restart();

    void Update();

    void Draw();

    void DebugDraw();

    void ResetButtons();

public:
    List<Button> button;
    bool finished = false;

    Collision collisionUtils;

    uint step = 1;

    bool resultActive = false;

    SDL_Rect bridge1 = {};
    SDL_Rect bridge2 = {};
    SDL_Rect bridge3 = {};

    SDL_Texture* bridge = nullptr;
};

#endif // __BUTTONPUZZLE_H_
