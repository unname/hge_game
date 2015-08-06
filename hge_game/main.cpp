#include "c_game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    c_game* Game = new c_game();
    Game->Run();

    return 0;
}