#include "c_game.h"

void c_game::SetWindowState(hgeCallback framefunc)
{
    size_t  screen_width= 800;
    size_t  screen_height = 600;
    size_t  screen_bpp = 32;
    bool    screen_windowed = true;
    size_t  fps = 100;

    hge->System_SetState(HGE_FRAMEFUNC, framefunc);
    hge->System_SetState(HGE_WINDOWED, screen_windowed);
    hge->System_SetState(HGE_SCREENWIDTH, screen_width);
    hge->System_SetState(HGE_SCREENHEIGHT, screen_height);
    hge->System_SetState(HGE_SCREENBPP, screen_bpp);
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_TITLE, "Main window");
    hge->System_SetState(HGE_FPS, fps);

    c_game_values& game_values = c_game_values::getInstance();
    game_values.SCREEN_WIDTH = screen_width;
    game_values.SCREEN_HEIGHT = screen_height;
    game_values.SCREEN_BPP = screen_bpp;
    game_values.SCREEN_WINDOWED = screen_windowed;
    game_values.FPS = fps;
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

    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_drawobject::DrawObjects[obj_num]->Update(dt);
    }

    //==========================/=============================/
    hge->Gfx_EndScene();

    return false;
}


bool c_game::LoadResources()
{
    //Загрузка карты
    string map_path;
    map_path.append(RESOURCES_PATH);
    map_path.append(MAP_NAME);

    map = new c_loadmap(map_path);

    c_game_values& game_values = c_game_values::getInstance();

    game_values.MAP_SIZE = map->GetMapSize();
    game_values.TILE_SIZE = map->GetTileSize();

    //Создание персонажа
    player = new c_player(20);

    return true;
}

void c_game::Release()
{
    delete map;
    delete player;
}

bool c_game::Initialize()
{
    SetWindowState(FrameFunc);

    if (hge->System_Initiate())
    {
        if (!LoadResources())
        {
            DisplayErrorHGE();
            return false;
        }
    }
    else
    {
        DisplayErrorHGE();
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
    Release();
    hge->System_Shutdown();
}
