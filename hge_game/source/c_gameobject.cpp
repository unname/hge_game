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

    //Определяем куда движется наш объект
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

hgeVector c_gameobject::GetNewPosition(hgeVector A, hgeVector B, hgeVector C)
{
    hgeVector NewPosition;
    hgeVector SwapPosition;
    c_bool PointUnderLine;

    // Уравнения прямых
    // AB: (A.y - B.y)*X + (B.x - A.x)*Y + (A.x*B.y - B.x*A.y) = 0
    // C-h: Y - C.y = 0
    // C-v: X - C.x = 0

    //Если коэф. при Y меньше 0, то меняем точки местами (нужно для одназначности дальнейших результатов)
    if ((B.x - A.x) < 0)
    {
        SwapPosition = A;
        A = B;
        B = SwapPosition;
    }

    //Проверяем ниже ли точка C относительно прямой AB
    //Так как ось Y инвертрована (отсчёт идёт сверху вниз),
    //то значение больше нуля означает, что точка лежит под линией
    if ((A.y - B.y)*C.x + (B.x - A.x)*C.y + (A.x*B.y - B.x*A.y) > 0)
    {
        PointUnderLine.SetTrue();
    }

    if (((moving.MovingDown.GetState()) && (PointUnderLine.GetState())) ||
        ((moving.MovingUp.GetState()) && (!PointUnderLine.GetState())))
    {
        NewPosition.y = C.y;
        NewPosition.x = -(((A.x*B.y - B.x*A.y) - (-C.y)*(B.x - A.x)) / (A.y - B.y));
    }
    else
    {
        NewPosition.x = C.x;
        NewPosition.y = -(((A.y - B.y)*(-C.x) - (A.x*B.y - B.x*A.y)) / -(B.x - A.x));
    }

    return NewPosition;
 };