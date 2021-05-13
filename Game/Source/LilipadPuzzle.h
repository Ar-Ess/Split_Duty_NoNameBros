#ifndef __LILIPADPUZZLE_H__
#define __LILIPÀDPUZZLE_H__

#include "World.h"
#include "Point.h"
#include "SDL/include/SDL.h"

#define MAX_MOVES 21

struct SDL_Rect;
struct SDL_Texture;
class Collision;

struct Lilipad
{
    Lilipad();
    Lilipad(SDL_Rect rectx, SDL_Texture* texx, uint idx);
    ~Lilipad();

    void SetPosition(int x, int y)
    {
        rect.x = x;
        rect.y = y;
        moves = (x - 28) / 28;
        moves = moves / 2;
    }

    void MoveLeft();
    void MoveRight();

    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    uint id;
    uint moves;

};

struct RiverTracks
{
    RiverTracks() {}
    RiverTracks(SDL_Rect riverRect, bool activex)
    {
        rect = riverRect;
        active = activex;
    }

    SDL_Rect rect = {};
    bool active = true;
};

class LilipadPuzzle
{
public:
    LilipadPuzzle();
    LilipadPuzzle(int lilipadAmount, SDL_Rect leftSwitchRect, SDL_Rect rightSwitchRect);
    ~LilipadPuzzle();

    void Start();

    void Restart();

    void Update();

    void Draw();

    void DebugDraw();

    void SwitchLogic();

    void RiverTrackLogic();

    Lilipad GetLiliFromId(uint id);

public:
    List<Lilipad> lilipad;
    bool finished = false;

    SDL_Rect leftSwitch = {};
    SDL_Rect rightSwitch = {};

    List<RiverTracks> riverTracks;

    Collision collisionUtils;

    bool switchPressed = false;
};

#endif // __ITEM_H__#pragma once
