#include "c_gameobject.h"

c_gameobject::c_gameobject()
{
    g = 40.0;
}

void c_gameobject::Update(float delta)
{
    c_game_values& game_values = c_game_values::getInstance();

    // -------------------------------------
    //
    //    ��������� ������
    //
    // -------------------------------------

    //���� ������ � �������, �� �� ������ ��� ����� ����������
    if (!OnTheGround.GetState())
    {
        Velocity.y += g*delta;
    }

    //���� ���� ������ �����
    Velocity.x *= GroundFriction;
    //���� ���� ������ �������
    Velocity.y *= AirFriction;

    //����������� ������������ ��������
    if (Velocity.x>Max_Speed)   Velocity.x = Max_Speed;
    if (Velocity.y>Max_Speed)   Velocity.y = Max_Speed;

    //������ ���������� ���������� ���������� ����� (��������� ������)
    //������� ���� �������� � ����� ��������� �������� - ������������� ���������������
    if (abs(PreviousVelocity.x) > abs(Velocity.x))
    {
        if (abs(Velocity.x) < delta*100)
            Velocity.x = 0;
    }

    // ������������� ������� ����� �������� ������
    Position.x += Velocity.x;
    Position.y += Velocity.y;

    // -------------------------------------
    //
    //         ��������� �����������
    //    + ���������� �������� ���������
    //
    // -------------------------------------

    OnTheGround.SetFalse();
    OnTheLeftWall.SetFalse();
    OnTheRightWall.SetFalse();

    isLanding.SetFalse();

    // ----------------------------------------
    //
    //      1. ����������� � ������ �����
    //
    // ----------------------------------------

    //���� �������� ������ �������
    if (Position.x >= game_values.GetMapSize().x - IntersectBoindingBoxSize.x / 2)
    {
        Position.x = game_values.GetMapSize().x - IntersectBoindingBoxSize.x / 2;
        Velocity.x = 0;
        Acceleration = 0;
        OnTheLeftWall.SetTrue();
    }

    //���� �������� ����� �������
    if (Position.x <= IntersectBoindingBoxSize.x / 2)
    {
        Position.x = IntersectBoindingBoxSize.x / 2;
        Velocity.x = 0;
        Acceleration = 0;
        OnTheRightWall.SetTrue();
    }

    //���� �������� ������ �������
    if (Position.y >= game_values.GetMapSize().y - IntersectBoindingBoxSize.y / 2)
    {
        Position.y = game_values.GetMapSize().y - IntersectBoindingBoxSize.y / 2;
        Velocity.y = 0;
        JumpImpulse = 0;
        OnTheGround.SetTrue();

        if (Position.x == PreviousPosition.x)
            isLanding.SetTrue();
    }

    //���� �������� ������� �������
    if (Position.y <= IntersectBoindingBoxSize.y / 2)
    {
        Position.y = IntersectBoindingBoxSize.y / 2;
        Velocity.y = 0;
        JumpImpulse = 0;
    }

    // ----------------------------------------
    //
    //      2. ����������� � �����������
    //
    // ----------------------------------------

    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_platform* Platform = dynamic_cast<c_platform*>(c_drawobject::DrawObjects[obj_num]);

        //���� ������ �� ��������� dynamic_cast ������ NULL
        if (Platform)
        {
            //���� ������������, �� ������������ ������������ � ��������� ����� �������
            if (GetIntersectBoundingBox().Intersect(&Platform->GetBoundingBox()))
            {
                Position = GetNewPosition_Rect(GetIntersectBoundingBox(), Platform->GetBoundingBox());
            }

            //���� ������ �� ���������
            if ((GetIntersectBoundingBox().y2 == Platform->GetBoundingBox().y1) && (GetIntersectBoundingBox().x2 > Platform->GetBoundingBox().x1) && (GetIntersectBoundingBox().x1 < Platform->GetBoundingBox().x2))
            {
                OnTheGround.SetTrue();

                if (Position.x == PreviousPosition.x)
                    isLanding.SetTrue();
            }

            //���� ��������� � ����� ������ ���������
            if ((GetIntersectBoundingBox().x2 == Platform->GetBoundingBox().x1) && (GetIntersectBoundingBox().y2>Platform->GetBoundingBox().y1) && (GetIntersectBoundingBox().y1 < Platform->GetBoundingBox().y2))
                OnTheLeftWall.SetTrue();

            //���� ��������� � ������ ������ ���������
            if ((GetIntersectBoundingBox().x1 == Platform->GetBoundingBox().x2) && (GetIntersectBoundingBox().y2>Platform->GetBoundingBox().y1) && (GetIntersectBoundingBox().y1 < Platform->GetBoundingBox().y2))
                OnTheRightWall.SetTrue();
        }
    }

    // ----------------------------------------
    //
    // ��������� ���������� � ������������ ��������
    //
    // ----------------------------------------

    if (Position.x > PreviousPosition.x)
    {
        Moving.MovingRigth.SetTrue();
        Moving.MovingLeft.SetFalse();
    }

    if (Position.x < PreviousPosition.x)
    {
        Moving.MovingLeft.SetTrue();
        Moving.MovingRigth.SetFalse();
    }

    if (Position.y > PreviousPosition.y)
    {
        Moving.MovingDown.SetTrue();
        Moving.MovingUp.SetFalse();
    }

    if (Position.y < PreviousPosition.y)
    {
        Moving.MovingUp.SetTrue();
        Moving.MovingDown.SetFalse();
    }

    if (Position == PreviousPosition)
        Moving.NotMoving.SetTrue();
    else
        Moving.NotMoving.SetFalse();

    //��������� �������� �������� � �������
    PreviousVelocity = Velocity;
    PreviousPosition = Position;      

    c_drawobject::Update(delta);
}

hgeRect c_gameobject::GetIntersectBoundingBox()
{
    hgeRect BoundingBox;

    BoundingBox.x1 = Position.x - IntersectBoindingBoxSize.x / 2;
    BoundingBox.x2 = Position.x + IntersectBoindingBoxSize.x / 2;
    BoundingBox.y1 = Position.y - IntersectBoindingBoxSize.y / 2;
    BoundingBox.y2 = Position.y + IntersectBoindingBoxSize.y / 2;

    return BoundingBox;
}

hgeVector c_gameobject::GetNewPosition_Rect(hgeRect BoundingBox1,hgeRect BoundingBox2)
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

    float Size_x = (BoundingBox1.x2 - BoundingBox1.x1) / 2;
    float Size_y = (BoundingBox1.y2 - BoundingBox1.y1) / 2;
    
    //��������� ����� ���� �������, ������� �� ��������� 
    //������� ����� - 1, ����� �� �������
    size_t angle_number = 0;

    //1.������� �����
    if (BoundingBox1.TestPoint(BoundingBox2.x1, BoundingBox2.y1))
    {
        angle_number = 1;
        C = hgeVector(BoundingBox2.x1, BoundingBox2.y1);
    }
    //2.������� ������
    if (BoundingBox1.TestPoint(BoundingBox2.x2, BoundingBox2.y1))
    {
        angle_number = 2;
        C = hgeVector(BoundingBox2.x2, BoundingBox2.y1);
    }
    //3.������ ������
    if (BoundingBox1.TestPoint(BoundingBox2.x2, BoundingBox2.y2))
    {
        angle_number = 3;
        C = hgeVector(BoundingBox2.x2, BoundingBox2.y2);
    }
    //4.������ �����
    if (BoundingBox1.TestPoint(BoundingBox2.x1, BoundingBox2.y2))
    {
        angle_number = 4;
        C = hgeVector(BoundingBox2.x1, BoundingBox2.y2);
    }

    //������� �����, ��� ��� 'AB' - ����� ����������� ���� (������� ������ ������ ������), � �� ������
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

    if ((B.x - A.x) == 0)
    {
        B_coef_zero.SetTrue();
    }

    //������ ���� ����. ��� X
    if ((A.y - B.y) < 0)
    {
        A_coef_negative.SetTrue();
    }

    if ((A.y - B.y) == 0)
    {
        A_coef_zero.SetTrue();
    }

    //��������� ���� �� ����� C ������������ ������ AB
    //��� ��� ��� Y ������������ (������ ��� ������ ����),
    //�� �������� ������ ���� ��������, ��� ����� ����� ��� ������
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

    //��������� ����� ������� ���� ������ ����������� ���� ������ ������� (�������� ������ ������� �������)
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
    //���� ���� �������� �� ������������, �� ��������� �������
    {
        //1.������� �������
        if (PreviousPosition.y < BoundingBox2.y1)
        {
            NewPosition.y = BoundingBox2.y1 - Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //2.������ �������
        if (PreviousPosition.y > BoundingBox2.y2)
        {
            NewPosition.y = BoundingBox2.y2 + Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //3.����� �������
        if (PreviousPosition.x < BoundingBox2.x1)
        {
            NewPosition.x = BoundingBox2.x1 - Size_x;
            Velocity.x = 0;
            Acceleration = 0;
        }

        //4.������ �������
        if (PreviousPosition.x > BoundingBox2.x2)
        {
            NewPosition.x = BoundingBox2.x2 + Size_x;
            Velocity.x = 0;
            Acceleration = 0;
        }
    }

    return NewPosition;
 };

hgeVector c_gameobject::GetNewPosition_Tilt(hgeRect BoundingBox1, hgeRect BoundingBox2, int tilt_type, int tilt_level, int tilt_number)
{
    hgeVector A = PreviousPosition;
    hgeVector B = Position;
    hgeVector C;

    hgeVector NewPosition(0, 0);
    hgeVector SwapPosition;

    c_bool PointUnderLine;
    c_bool A_coef_negative,
        B_coef_negative;
    c_bool A_coef_zero,
        B_coef_zero;
    c_bool Y_fixed;    

    return NewPosition;
 }