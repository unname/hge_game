#include "c_gameobject.h"

float c_gameobject::g = 40.0;

c_gameobject::c_gameobject()
{
    PreviousPosition = Position;
}

hgeVector c_gameobject::GetVelocity()
{
    return Velocity;
};

void c_gameobject::Update(float delta)
{
    c_drawobject::Update(delta);

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

    //���������� ���� �������� ��� ������
    moving.MovingLeft.SetFalse();
    moving.MovingRigth.SetFalse();
    moving.MovingUp.SetFalse();
    moving.MovingDown.SetFalse();

    if (Position.x > PreviousPosition.x)
    {
        moving.MovingRigth.SetTrue();
    }

    if (Position.x < PreviousPosition.x)
    {
        moving.MovingLeft.SetTrue();
      }

    if (Position.y > PreviousPosition.y)
    {
        moving.MovingDown.SetTrue();
    }

    if (Position.y < PreviousPosition.y)
    {
        moving.MovingUp.SetTrue();
    }

    PreviousPosition = Position;

    Render();
}