#ifndef C_HGE_H
#define C_HGE_H
#pragma once

#include "hge.h"

class c_hge
{
protected:
    static HGE* hge;
    static int  counter;
public:
    c_hge();
    ~c_hge();
};

#endif