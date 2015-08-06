#ifndef C_GAME_H
#define C_GAME_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

#include "c_hge.h"
#include "c_player.h"
#include "c_loadmap.h"
#include "c_gameobject.h"

#include <vector>


class c_game : public c_hge
{
private:
    void SetWindowState(hgeCallback framefunc);
    static bool FrameFunc();

public:
    static std::vector < c_gameobject > GameObjects;
    
    static size_t SCREEN_WIDTH;
    static size_t SCREEN_HEIGHT;

    void Run();
};

#endif