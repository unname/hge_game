# include "c_hge.h"

HGE* c_hge::hge = 0;
int c_hge::counter = 0;

c_hge::c_hge()
{
    if (!counter)
    {
        hge = hgeCreate(HGE_VERSION);

        if (!hge)
        {
            DisplayErrorHGE();
        }
    }
    
    counter++;
}

c_hge::~c_hge()
{
    counter--;

    if (!counter)
        hge->Release();
}

int c_hge::GetScreenWidth()
{
    return hge->System_GetState(HGE_SCREENWIDTH);
}

int c_hge::GetScreenHeight()
{
    return hge->System_GetState(HGE_SCREENHEIGHT);
}


void c_hge::DisplayError(LPCSTR error_string)
{
    MessageBox(NULL, error_string, "Error", MB_OK | MB_ICONERROR);
}

void c_hge::DisplayErrorHGE()
{
    MessageBox(NULL, hge->System_GetErrorMessage(), "HGE_Error", MB_OK | MB_ICONERROR);
}