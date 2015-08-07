#include "c_game.h"

size_t c_game::SCREEN_WIDTH = 800;
size_t c_game::SCREEN_HEIGHT = 600;

std::vector < c_gameobject* > c_game::GameObjects;

void c_game::SetWindowState(hgeCallback framefunc)
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

bool c_game::FrameFunc()
{
    //Узнаем сколько прошло времени с последнего вызова функции кадра
    float dt = hge->Timer_GetDelta();

    if (hge->Input_GetKeyState(HGEK_ESCAPE))
        return true;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    //Рендерим всё тут:
    //==========================/=============================/

    for (size_t obj_num = 0; obj_num < GameObjects.size(); obj_num++)
    {
        GameObjects[obj_num]->Update(dt);
    }

    //==========================/=============================/
    hge->Gfx_EndScene();

    return false;
}


bool c_game::LoadResources()
{
    //Загрузка карты
    c_loadmap* map = new c_loadmap;

    //Создание персонажа
    c_player* player = new c_player(20);

    return true;
}

bool c_game::Initialize()
{
    SetWindowState(FrameFunc);

    if (hge->System_Initiate())
    {
        if (!LoadResources())
        {
            DisplayError();
            return false;
        }
    }
    else
    {
        DisplayError();
        return false;
    }

        return true;
}

void c_game::Start()
{
    hge->System_Start();
}

void c_game::Shutdown()
{
    //TODO: delete resources

    hge->System_Shutdown();
}

void c_game::DisplayError()
{
    MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR);
}