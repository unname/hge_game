#ifndef C_GAME_H
#define C_GAME_H
#pragma once

#include "hge.h"

#include <vector>

#include "c_hge.h"
#include "c_loadmap.h"
#include "c_drawobject.h"
#include "c_player.h"
#include "c_background.h"
#include "c_game_values.h"


class c_game : public c_hge
{
private:

    void SetWindowState(hgeCallback framefunc);
    static bool FrameFunc();
    bool LoadResources();

    c_background* background;
    c_loadmap* map;
    c_player* player;

public:    
    bool Initialize();
    void Release();
    void Start();
    void Shutdown();
};

#endif