#ifndef C_PLATFORM_H
#define C_PLATFORM_H
#pragma once

#include "hge.h"

#include "c_bool.h"
#include "c_drawobject.h"

class c_platform : public c_drawobject
{
public:
    c_bool LeftBound;
    c_bool RightBound;
    c_bool TopBound;
    c_bool BottomBound;

    c_platform(hgeSprite* sprite, hgeVector sprite_coord);
    c_platform(hgeSprite* sprite, hgeVector sprite_coord, bool left, bool right, bool top, bool bottom);
};


#endif