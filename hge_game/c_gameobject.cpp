#include "c_gameobject.h"

HGE* c_gameobject::hge = 0;

float c_gameobject::g = 30.0;

void c_gameobject::SetVelocity(hgeVector velocity)
{
    Velocity = velocity;
};

void c_gameobject::SetPosition(hgeVector position)
{
    Position = position;
};

hgeVector c_gameobject::GetVelocity()
{
    return Velocity;
};

hgeVector c_gameobject::GetPosition()
{
    return Position;
};

int c_gameobject::GetScreenWidth(HGE* hge)
{
    return hge->System_GetState(HGE_SCREENWIDTH);
}

int c_gameobject::GetScreenHeight(HGE* hge)
{
    return hge->System_GetState(HGE_SCREENHEIGHT);
}


void c_gameobject::Render()
{

}

void c_gameobject::Update(float delta)
{
    int sWidth = GetScreenWidth(hge);
    int sHeight = GetScreenHeight(hge);

    //Реализовываем гравитацию
    if (Position.y < sHeight - Size)
    {
        Velocity.y += g*delta;
    }

    Render();
}