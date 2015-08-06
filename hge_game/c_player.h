#ifndef C_PLAYER_H
#define C_PLAYER_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

#include "c_gameobject.h"

class c_player: c_gameobject
{
private:

    hgeQuad         Quad;
    HTEXTURE        Texture;
    hgeAnimation*   Sprite;
    hgeRect         BoundingBox; //необходимо  для обнаружения столкновений

    c_bool SpaceHoldDown;

public:
    c_player(u_int size);
    ~c_player();

    void Render();
    void Update(float delta);

    hgeRect GetBoundingBox();
    hgeVector GetPosition();
};

#endif