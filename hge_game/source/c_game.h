#ifndef C_GAME_H
#define C_GAME_H
#pragma once

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
    void Release();

    c_loadmap* map;
    c_player* player;

public:    
    static size_t SCREEN_WIDTH;
    static size_t SCREEN_HEIGHT;

    bool Initialize();
    void Start();
    void Shutdown();
};

#endif