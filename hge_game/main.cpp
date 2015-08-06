#include "hge.h"
#include "c_player.h"
#include "c_loadmap.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

HGE *hge = 0;
c_player* player;

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
    //������ ������� ������ ������� � ���������� ������ ������� �����
    float dt = hge->Timer_GetDelta();
    
    if (hge->Input_GetKeyState(HGEK_ESCAPE))
        return true;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    //�������� �� ���:
    //==========================/=============================/

    player->Update(dt);  // ������ ������


    //==========================/=============================/
    hge->Gfx_EndScene();

    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    hge = hgeCreate(HGE_VERSION);
    SetWindowState(hge, FrameFunc);

    //������ ����
    if (hge->System_Initiate())
    {
        //�������� ������
        c_loadmap* map = new c_loadmap();

        //�������� ���������
        player = new c_player(20);

        hge->System_Start();

        delete player;
        delete map;
    }
    else
    {
        return 0;
    }

    //��������� ������
    hge->System_Shutdown();
    hge->Release();

    return 0;
}