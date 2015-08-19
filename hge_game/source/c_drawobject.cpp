#include "c_drawobject.h"

std::vector<c_drawobject*> c_drawobject::DrawObjects;
hgeVector c_drawobject::PlayerPosition;

c_drawobject::c_drawobject()
{
    Texture = NULL;

    DrawObjects.push_back(this);
}

c_drawobject::c_drawobject(hgeSprite* sprite, hgeVector sprite_coord)
{
    Texture = NULL;

    SetSprite(sprite);
    SetPosition(sprite_coord);

    DrawObjects.push_back(this);
}

c_drawobject::~c_drawobject()
{
    if (Texture)
        hge->Texture_Free(Texture);

    for (size_t obj_num = 0; obj_num < DrawObjects.size(); obj_num++)
    {
        if (DrawObjects[obj_num] == this)
        {
            DrawObjects.erase(DrawObjects.begin() + obj_num);
            break;
        }
    }
}

void c_drawobject::SetSprite(hgeSprite* sprite)
{
    Sprite = sprite;
}

void c_drawobject::SetPosition(hgeVector position)
{
    Position = position;
};

hgeVector c_drawobject::GetPosition()
{
    return Position;
};


hgeRect c_drawobject::GetBoundingBox()
{
    return BoundingBox;
};


void c_drawobject::Update(float delta)
{
    Render();
}

void c_drawobject::Render()
{
    hge->Gfx_RenderQuad(&Quad);

    if(Sprite)
        Sprite->Render(Position.x - PlayerPosition.x + GetScreenWidth() / 2, Position.y - PlayerPosition.y + GetScreenHeight() / 2);
}

