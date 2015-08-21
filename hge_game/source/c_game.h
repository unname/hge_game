#ifndef C_GAME_H
#define C_GAME_H
#pragma once

class c_player;

#include "hge.h"

#include <vector>

#include "c_hge.h"
#include "c_player.h"
#include "c_loadmap.h"
#include "c_drawobject.h"


class c_game : public c_hge
{
private:
    void SetWindowState(hgeCallback framefunc);
    static bool FrameFunc();
    bool LoadResources();

    c_loadmap* map;
    c_player* player;

public:    
    static size_t SCREEN_WIDTH;
    static size_t SCREEN_HEIGHT;
    static hgeVector MAP_SIZE;
    static hgeVector TILE_SIZE;

    bool Initialize();
    void Release();
    void Start();
    void Shutdown();
};

#endif