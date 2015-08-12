#ifndef C_HGE_H
#define C_HGE_H
#pragma once

#include "hge.h"

class c_hge
{
private:
    static int  counter;    

protected:
    static HGE* hge;

    void DisplayError(LPCSTR error_string);
    void DisplayErrorHGE();
public:
    c_hge();
    ~c_hge();

    int GetScreenWidth();
    int GetScreenHeight();
};

#endif