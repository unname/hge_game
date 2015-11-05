#ifndef C_DRAWOBJECT_H
#define C_DRAWOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

#include "c_hge.h"

#include <vector>

class c_drawobject : public c_hge
{
protected:
    HTEXTURE        Texture;
    hgeSprite*      Sprite;
    hgeVector       Position;
public:
    static std::vector<c_drawobject*> DrawObjects;

    static hgeVector    PlayerPosition;

    c_drawobject();
    c_drawobject(hgeSprite* sprite, hgeVector sprite_coord);
    ~c_drawobject();

    hgeRect GetBoundingBox();
    virtual void Update(float delta);
    virtual void Render();
};

#endif