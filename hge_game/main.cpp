#include "hge.h"
#include "c_cube.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

HGE *hge = 0;

c_cube* cube;

void SetWindowState(HGE *hge, hgeCallback framefunc)
{
    hge->System_SetState(HGE_FRAMEFUNC, framefunc);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
    hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
    hge->System_SetState(HGE_SCREENBPP, 32);
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_TITLE, "Main window");
    hge->System_SetState(HGE_FPS, 100);
}

bool FrameFunc()
{
    //Узнаем сколько прошло времени с последнего вызова функции кадра
    float dt = hge->Timer_GetDelta();
    
    if (hge->Input_GetKeyState(HGEK_ESCAPE))
        return true;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    //Рендерим всё тут
    cube->Update(dt);

    hge->Gfx_EndScene();

    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    hge = hgeCreate(HGE_VERSION);
    SetWindowState(hge, FrameFunc);

    //Запуск игры
    if (hge->System_Initiate())
    {
        cube = new c_cube(20, hgeVector(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2));

        hge->System_Start();

        delete cube;
    }
    else
    {
        return 0;
    }

    //Завершаем работу
    hge->System_Shutdown();
    hge->Release();

    return 0;
}