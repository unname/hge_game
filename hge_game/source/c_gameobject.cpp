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

hgeVector c_gameobject::GetNewPosition(hgeVector A, hgeVector B, hgeVector C, size_t C_number)
{
    hgeVector NewPosition(0,0);
    hgeVector SwapPosition;
    c_bool PointUnderLine;
    c_bool A_coef_negative,
           B_coef_negative;
    c_bool Y_fixed;

    // ��������� ������
    // AB: (A.y - B.y)*X + (B.x - A.x)*Y + (A.x*B.y - B.x*A.y) = 0
    // C-h: Y - C.y = 0
    // C-v: X - C.x = 0

    //������ ���� ����. ��� Y
    //���� ����. ��� Y ������ 0, �� ������ ����� ������� (����� ��� ������������� ���������� �����������)
    if ((B.x - A.x) < 0)
    {
        B_coef_negative.SetTrue();

        SwapPosition = A;
        A = B;
        B = SwapPosition;
    }

    //������ ���� ����. ��� X
    if ((A.y - B.y) < 0)
    {
        A_coef_negative.SetTrue();
    }


    //��������� ���� �� ����� C ������������ ������ AB
    //��� ��� ��� Y ������������ (������ ��� ������ ����),
    //�� �������� ������ ���� ��������, ��� ����� ����� ��� ������
    if ((A.y - B.y)*C.x + (B.x - A.x)*C.y + (A.x*B.y - B.x*A.y) > 0)
    {
        PointUnderLine.SetTrue();
    }
    //    1\/2  3\/4
    // 16\          /5
    // 15/          \6
    // 
    // 14\          /7
    // 13/          \8
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

    switch (C_number)
    {
    case 1:
        //1
        if (PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x -= Size_x;
            NewPosition.y -= Size_y;
        }
        //2
        if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x += Size_x;
            NewPosition.y -= Size_y;
        }
        //15
        if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            NewPosition.x -= Size_x;
            NewPosition.y += Size_y;
        }
        //16
        if (!PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            NewPosition.x -= Size_x;
            NewPosition.y -= Size_y;
        }
        break;
    case 2:
        //3
        if (!PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x -= Size_x;
            NewPosition.y -= Size_y;
        }
        //4
        if (PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x += Size_x;
            NewPosition.y -= Size_y;
        }
        //5
        if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            NewPosition.x += Size_x;
            NewPosition.y -= Size_y;
        }
        //6
        if (!PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            NewPosition.x += Size_x;
            NewPosition.y += Size_y;
        }
        break;
    case 3:
        //9
        if (!PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x += Size_x;
            NewPosition.y += Size_y;
        }
        //10
        if (PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x -= Size_x;
            NewPosition.y += Size_y;
        }
        //7
        if (PointUnderLine.GetState() && !A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            NewPosition.x += Size_x;
            NewPosition.y -= Size_y;
        }
        //8
        if (PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            NewPosition.x += Size_x;
            NewPosition.y += Size_y;
        }
        break;
    case 4:
    default:
        //11
        if (PointUnderLine.GetState() && A_coef_negative.GetState() && B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x += Size_x;
            NewPosition.y += Size_y;
        }
        //12
        if (!PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            Y_fixed.SetTrue();
            NewPosition.x -= Size_x;
            NewPosition.y += Size_y;
        }
        //13
        if (PointUnderLine.GetState() && !A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            NewPosition.x -= Size_x;
            NewPosition.y += Size_y;
        }
        //14
        if (PointUnderLine.GetState() && A_coef_negative.GetState() && !B_coef_negative.GetState())
        {
            NewPosition.x -= Size_x;
            NewPosition.y -= Size_y;
        }
        break;
    }

    //����� ������� ���� ������ ����������� ���� ������� (�������� ������ ���������)
    if (Y_fixed.GetState())
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