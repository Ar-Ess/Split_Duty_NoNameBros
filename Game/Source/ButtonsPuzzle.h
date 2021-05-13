#ifndef __BUTTONPUZZLE_H__
#define __BUTTONPUZZLE_H__

#include "World.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Rect;
struct SDL_Texture;
struct Collision;

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
    ButtonPuzzle(int stoneAmount, uint width, uint height);
    ~ButtonPuzzle();

    void Restart();

    void Update();

    void Draw();

    void DebugDraw();

public:
    List<Button> button;
    bool finished = false;

    Collision collisionUtils;

};

#endif // __BUTTONPUZZLE_H_
