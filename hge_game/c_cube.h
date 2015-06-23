#ifndef C_CUBE_H
#define C_CUBE_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

class c_cube
{
private:
    static HGE*     hge;

    hgeQuad         Quad;
    HTEXTURE        Texture;
    hgeAnimation*   Sprite;
    hgeRect         BoundingBox; //необходимо  для обнаружения столкновений

    hgeVector       Position;   // x,y
    hgeVector       Velocity;   // dx,dy

    float           Speed;
    float           Friction;
    u_int           Size;

public:
    c_cube(u_int size);
    ~c_cube();

    bool Update(float delta);
    void Render();

    void SetVelocity(hgeVector velocity);
    void SetPosition(hgeVector position);

    hgeRect GetBoundingBox();
    hgeVector GetPosition();

    int GetScreenWidth(HGE* hge);
    int GetScreenHeight(HGE* hge);
};

#endif