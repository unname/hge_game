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
    hgeRect         BoundingBox; //необходимо  для обнаружения столкновений
    hgeQuad         Quad;
    HTEXTURE        Texture;
    hgeSprite*      Sprite;

    void SetSprite(hgeSprite* sprite);
    void SetPosition(hgeVector position);
public:
    static std::vector<c_drawobject*> DrawObjects;

    hgeVector   Position;   // x,y

    c_drawobject();
    c_drawobject(hgeSprite* sprite, hgeVector sprite_coord);
    ~c_drawobject();


    hgeVector GetPosition();
    hgeRect GetBoundingBox();

    virtual void Update(float delta);
    virtual void Render();
};

#endif