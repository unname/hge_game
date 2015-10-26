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

    PreviousPosition = Position;

    Render();
}

hgeVector c_gameobject::GetNewPosition(hgeRect BoundingBox)
{

    hgeVector A = PreviousPosition;
    hgeVector B = Position;
    hgeVector C;

    hgeVector NewPosition(0,0);
    hgeVector SwapPosition;

    c_bool PointUnderLine;
    c_bool A_coef_negative,
           B_coef_negative;
    c_bool A_coef_zero,
           B_coef_zero;
    c_bool Y_fixed;

    float Size_x = Sprite->GetWidth()/2; 
    float Size_y = Sprite->GetHeight()/2;
    
    //Определим номер угла спрайта, который мы пересекли 
    //Верхний левый - 1, далее по часовой
    size_t angle_number = 0;

    //1.Верхний левый
    if (GetBoundingBox().TestPoint(BoundingBox.x1, BoundingBox.y1))
    {
        angle_number = 1;
        C = hgeVector(BoundingBox.x1, BoundingBox.y1);
    }
    //2.Верхний правый
    if (GetBoundingBox().TestPoint(BoundingBox.x2, BoundingBox.y1))
    {
        angle_number = 2;
        C = hgeVector(BoundingBox.x2, BoundingBox.y1);
    }
    //3.Нижний правый
    if (GetBoundingBox().TestPoint(BoundingBox.x2, BoundingBox.y2))
    {
        angle_number = 3;
        C = hgeVector(BoundingBox.x2, BoundingBox.y2);
    }
    //4.Нижний левый
    if (GetBoundingBox().TestPoint(BoundingBox.x1, BoundingBox.y2))
    {
        angle_number = 4;
        C = hgeVector(BoundingBox.x1, BoundingBox.y2);
    }

    //Смещаем точки, так как 'AB' - линия перемещения угла (который пересёк другой спрайт), а не центра
    switch (angle_number)
    {
    case 1:
        A.x = A.x + Size_x;
        A.y = A.y + Size_y;
        B.x = B.x + Size_x;
        B.y = B.y + Size_y;

        NewPosition.x -= Size_x;
        NewPosition.y -= Size_y;
        break;
    case 2:
        A.x = A.x - Size_x;
        A.y = A.y + Size_y;
        B.x = B.x - Size_x;
        B.y = B.y + Size_y;

        NewPosition.x += Size_x;
        NewPosition.y -= Size_y;
        break;
    case 3:
        A.x = A.x - Size_x;
        A.y = A.y - Size_y;
        B.x = B.x - Size_x;
        B.y = B.y - Size_y;

        NewPosition.x += Size_x;
        NewPosition.y += Size_y;
        break;
    case 4:
        A.x = A.x + Size_x;
        A.y = A.y - Size_y;
        B.x = B.x + Size_x;
        B.y = B.y - Size_y;

        NewPosition.x -= Size_x;
        NewPosition.y += Size_y;
        break;
    default:
        break;
    }

    // Уравнения прямых
    // AB: (A.y - B.y)*X + (B.x - A.x)*Y + (A.x*B.y - B.x*A.y) = 0
    // C-h: Y - C.y = 0
    // C-v: X - C.x = 0

    //Узнаем знак коэф. при Y
    //Если коэф. при Y меньше 0, то меняем точки местами (нужно для однозначности дальнейших результатов)
    if ((B.x - A.x) < 0)
    {
        B_coef_negative.SetTrue();

        SwapPosition = A;
        A = B;
        B = SwapPosition;
    }

    if ((B.x - A.x) == 0)
    {
        B_coef_zero.SetTrue();
    }

    //Узнаем знак коэф. при X
    if ((A.y - B.y) < 0)
    {
        A_coef_negative.SetTrue();
    }

    if ((A.y - B.y) == 0)
    {
        A_coef_zero.SetTrue();
    }

    //Проверяем ниже ли точка C относительно прямой AB
    //Так как ось Y инвертрована (отсчёт идёт сверху вниз),
    //то значение больше нуля означает, что точка лежит под линией
    if ((A.y - B.y)*C.x + (B.x - A.x)*C.y + (A.x*B.y - B.x*A.y) > 0)
    {
        PointUnderLine.SetTrue();
    }
    //    1\/2   3\/4
    // 16\           /5
    // 15/           \6
    //         o
    // 14\           /7
    // 13/           \8
    //   12/\11  10/\9
    //
    // POINT_UNDER : 1, 4, 7, 8, 10, 11, 13, 14
    // !POINT_UNDER: 2, 3, 5, 6, 9, 12, 15, 16  
    //
    // A_NEGATIVE  : 1, 3, 6, 8, 9, 11, 14, 16
    // !A_NEGATIVE : 2, 4, 5, 7, 10, 12, 13, 15
    //
    // B_NEGATIVE  : 2, 4, 5, 6, 7, 8, 9, 11,
    // !B_NEGATIVE : 1, 3, 10, 12, 13, 14, 15, 16
    //
    // Y_fixed: 1, 2, 3, 4, 9, 10, 11, 12
    // X_fixed: 5, 6, 7, 8, 13, 14, 15, 16

    switch (angle_number)
    {
    case 1:
        if (!A_coef_zero.GetState() && !B_coef_zero.GetState())
        {
            //1
            if (PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //2
            if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //15
            if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
            //16
            if (!PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        else
        {
            //a
            if (B_coef_zero.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //h
            if (A_coef_zero.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        break;
    case 2:
        if (!A_coef_zero.GetState() && !B_coef_zero.GetState())
        {
            //3
            if (!PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //4
            if (PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //5
            if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
            //6
            if (!PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        else
        {
            //b
            if (B_coef_zero.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //c
            if (A_coef_zero.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        break;
    case 3:
        if (!A_coef_zero.GetState() && !B_coef_zero.GetState())
        {
            //9
            if (!PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //10
            if (PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //7
            if (PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
            //8
            if (PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        else
        {
            //e
            if (B_coef_zero.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //d
            if (A_coef_zero.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        break;
    case 4:
        if (!A_coef_zero.GetState() && !B_coef_zero.GetState())
        {
            //11
            if (PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //12
            if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //13
            if (PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
            //14
            if (PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        else
        {
            //f
            if (B_coef_zero.GetState())
            {
                Y_fixed.SetTrue();
                Velocity.y = 0;
                JumpImpulse = 0;
            }
            //g
            if (A_coef_zero.GetState())
            {
                Velocity.x = 0;
                Acceleration = 0;
            }
        }
        break;
    default:
        break;
    }

    //Вычисляем новую позицию куда должны переместить угол нашего объекта (попавший внутрь другого спрайта)
    if (angle_number)
    {
        if (!A_coef_zero.GetState() && !B_coef_zero.GetState())
        {
            if (Y_fixed.GetState())
            {
                NewPosition.y += C.y;
                NewPosition.x += -(((A.x*B.y - B.x*A.y) - (-C.y)*(B.x - A.x)) / (A.y - B.y));
            }
            else
            {
                NewPosition.x += C.x;
                NewPosition.y += -(((A.y - B.y)*(-C.x) - (A.x*B.y - B.x*A.y)) / -(B.x - A.x));
            }
        }
        else
        {
            if (Y_fixed.GetState())
            {
                NewPosition.x += B.x;
                NewPosition.y += C.y;
            }
            else
            {
                NewPosition.x += C.x;
                NewPosition.y += B.y;
            }
        }
    }
    else
    //Если углы спрайта не пересекались, то проверяем стороны
    {
        //1.Верхняя сторона
        if (PreviousPosition.y < BoundingBox.y1)
        {
            NewPosition.y = BoundingBox.y1 - Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //2.Нижняя сторона
        if (PreviousPosition.y > BoundingBox.y2)
        {
            NewPosition.y = BoundingBox.y2 + Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //3.Левая сторона
        if (PreviousPosition.x < BoundingBox.x1)
        {
            NewPosition.x = BoundingBox.x1 - Size_x;
            Velocity.x = 0;
            Acceleration = 0;
        }

        //4.Правая сторона
        if (PreviousPosition.x > BoundingBox.x2)
        {
            NewPosition.x = BoundingBox.x2 + Size_x;
            Velocity.x = 0;
            Acceleration = 0;
        }
    }

    return NewPosition;
 };