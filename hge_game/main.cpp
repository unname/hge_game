#include "hge.h"

HGE *hge = 0;

bool FrameFunc(){
    if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
    hge = hgeCreate(HGE_VERSION);
    hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_TITLE, "Main window");
    if (hge->System_Initiate()){
        hge->System_Start();
    }
    else {
        return 0;
    }
    hge->System_Shutdown();
    hge->Release();
    return 0;
}