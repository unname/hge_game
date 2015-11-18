#ifndef C_GAME_VALUES_H
#define C_GAME_VALUES_H
#pragma once

#include "hge.h"
#include "hgevector.h"

class c_game_values
{
private:
    //Конструктор и деструктор недоступны
    c_game_values(){};
    ~c_game_values(){};

    //Также запрещаем копирование
    c_game_values(c_game_values const&) = delete;
    c_game_values& operator=(c_game_values const&) = delete;

    //Данные
    size_t  SCREEN_WIDTH;
    size_t  SCREEN_HEIGHT;
    size_t  SCREEN_BPP;
    bool    SCREEN_WINDOWED;

    size_t  FPS;

    hgeVector MAP_SIZE;
    hgeVector TILE_SIZE;

    //только c_game имеет доступ на изменение данных
    friend class c_game;

public:
    static c_game_values& getInstance();

    size_t      GetScreenWidth();
    size_t      GetScreenHeight();
    size_t      GetScreenBPP();
    bool        GetScreenWindowed();
    size_t      GetFPS();
    hgeVector   GetMapSize();
    hgeVector   GetTileSize();
};

#endif