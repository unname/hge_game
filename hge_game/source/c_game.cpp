#include "c_game.h"

size_t c_game::SCREEN_WIDTH = 800;
size_t c_game::SCREEN_HEIGHT = 600;
hgeVector c_game::MAP_SIZE = hgeVector(0, 0);
hgeVector c_game::TILE_SIZE = hgeVector(0, 0);

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
    //������ ������� ������ ������� � ���������� ������ ������� �����
    float dt = hge->Timer_GetDelta();

    if (hge->Input_GetKeyState(HGEK_ESCAPE))
        return true;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    //�������� �� ���:
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
    //�������� �����
    string map_path;
    map_path.append(RESOURCES_PATH);
    map_path.append(MAP_NAME);

    map = new c_loadmap(map_path);
    MAP_SIZE = map->GetMapSize();
    TILE_SIZE = map->GetTileSize();

    //�������� ���������
    player = new c_player(16);

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
