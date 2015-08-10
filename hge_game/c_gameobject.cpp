#include "c_gameobject.h"

float c_gameobject::g = 40.0;

void c_gameobject::SetVelocity(hgeVector velocity)
{
    Velocity = velocity;
};

hgeVector c_gameobject::GetVelocity()
{
    return Velocity;
};

void c_gameobject::Update(float delta)
{
    c_drawobject::Update(delta);

    int sWidth = GetScreenWidth();
    int sHeight = GetScreenHeight();

    //���� ������ � �������, �� �� ������ ��� ����� ����������
    if (!OnTheGround.GetState())
    {
        Velocity.y += g*delta;
    }
    else
    {
        //���� ������ �� �����, �� �������� ���
        JumpImpulse = 0;
    }

    Render();
}