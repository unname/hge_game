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
            MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
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