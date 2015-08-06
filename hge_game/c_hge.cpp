# include "c_hge.h"

HGE* c_hge::hge = 0;
size_t c_hge::counter = 0;

c_hge::c_hge()
{
    if (!counter)
    {
        hge = hgeCreate(HGE_VERSION);

        if (!hge)
        {
            //Error;
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