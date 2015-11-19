#include "c_platform.h"

c_platform::c_platform(hgeSprite* sprite, hgeVector sprite_coord) :c_drawobject(sprite, sprite_coord)
{
    LeftBound.SetTrue();
    RightBound.SetTrue();
    TopBound.SetTrue();
    BottomBound.SetTrue();

    TiltType    = 0;
    TiltLevel   = 0;
    TiltNumber  = 0;
}

c_platform::c_platform(hgeSprite* sprite, hgeVector sprite_coord, bool left, bool right, bool top, bool bottom) :c_drawobject(sprite, sprite_coord)
{
    if (left)
        LeftBound.SetTrue();
    if (right)
        RightBound.SetTrue();
    if (top)
        TopBound.SetTrue();
    if (bottom)
        BottomBound.SetTrue();

    TiltType    = 0;
    TiltLevel   = 0;
    TiltNumber  = 0;
}

c_platform::c_platform(hgeSprite* sprite, hgeVector sprite_coord, int tilt_type, int tilt_level, int tilt_number) :c_drawobject(sprite, sprite_coord)
{
    LeftBound.SetTrue();
    RightBound.SetTrue();
    TopBound.SetTrue();
    BottomBound.SetTrue();

    TiltType    = tilt_type;
    TiltLevel   = tilt_level;
    TiltNumber  = tilt_number;
}