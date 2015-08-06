#include "c_gameobject.h"

float c_gameobject::g = 40.0;

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

int c_gameobject::GetScreenWidth()
{
    return hge->System_GetState(HGE_SCREENWIDTH);
}

int c_gameobject::GetScreenHeight()
{
    return hge->System_GetState(HGE_SCREENHEIGHT);
}

void c_gameobject::Render()
{

}

void c_gameobject::Update(float delta)
{
    int sWidth = GetScreenWidth();
    int sHeight = GetScreenHeight();

    //Если объект в воздухе, то он падаем под силой гравитации
    if (!OnTheGround.GetState())
    {
        Velocity.y += g*delta;
    }
    else
    {
        //Если объект на земле, то импульса нет
        JumpImpulse = 0;
    }

    Render();
}