#include "c_game_values.h"


c_game_values& c_game_values::getInstance()
{
    static c_game_values s;
    return s;
}

size_t c_game_values::GetScreenWidth()
{
    return SCREEN_WIDTH;
}

size_t c_game_values::GetScreenHeight()
{
    return SCREEN_HEIGHT;
}

size_t c_game_values::GetScreenBPP()
{
    return SCREEN_BPP;
}

bool c_game_values::GetScreenWindowed()
{
    return SCREEN_WINDOWED;
}

size_t c_game_values::GetFPS()
{
    return FPS;
}

hgeVector c_game_values::GetMapSize()
{
    return MAP_SIZE;
}

hgeVector c_game_values::GetTileSize()
{
    return TILE_SIZE;
}