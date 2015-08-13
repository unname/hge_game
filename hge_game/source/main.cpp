#include "c_game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    c_game Game;

    if (Game.Initialize())
    {
        Game.Start();
    }

    Game.Shutdown();

    return 0;
}