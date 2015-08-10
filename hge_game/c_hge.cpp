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
            DisplayError();
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

void c_hge::DisplayError()
{
    MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR);
}