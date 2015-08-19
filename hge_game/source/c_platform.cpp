#include "c_platform.h"

c_platform::c_platform(hgeSprite* sprite, hgeVector sprite_coord) :c_drawobject(sprite, sprite_coord)
{
    LeftBound.SetTrue();
    RightBound.SetTrue();
    TopBound.SetTrue();
    BottomBound.SetTrue();
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
}
