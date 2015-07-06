#ifndef C_CUBE_H
#define C_CUBE_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

#include "c_gameobject.h"

class c_cube: c_gameobject
{
private:

    hgeQuad         Quad;
    HTEXTURE        Texture;
    hgeAnimation*   Sprite;
    hgeRect         BoundingBox; //необходимо  для обнаружения столкновений

public:
    c_cube(u_int size);
    ~c_cube();

    void Render();
    void Update(float delta);

    hgeRect GetBoundingBox();
    hgeVector GetPosition();
};

#endif