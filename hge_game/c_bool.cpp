#include "c_bool.h"


c_bool::c_bool()
{
   state = false;
}

void c_bool::SetTrue()
{
    state = true;
}

void c_bool::SetFalse()
{
    state = false;
}

bool c_bool::GetState()
{
    return state;
}