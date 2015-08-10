#include "c_drawobject.h"

std::vector<c_drawobject*> c_drawobject::DrawObjects;

c_drawobject::c_drawobject()
{
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

    //delete Sprite;
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

}

void c_drawobject::Render()
{
    hge->Gfx_RenderQuad(&Quad);
    //Sprite->Render(Position.x, Position.y);
}

