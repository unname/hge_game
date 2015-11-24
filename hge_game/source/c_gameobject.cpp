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
    //    Обработка Физики
    //
    // -------------------------------------

    //Если объект в воздухе, то он падаем под силой гравитации
    if (!OnTheGround.GetState())
    {
        Velocity.y += g*delta;
    }

    //Учёт силы трения земли
    Velocity.x *= GroundFriction;
    //Учёт силы трения воздуха
    if (!OnTheGround.GetState())
        Velocity.y *= AirFriction;
    else
        Velocity.y *= GroundFriction;

    //Ограничение максимальной скорости
    if (Velocity.x>Max_Speed)   Velocity.x = Max_Speed;
    if (Velocity.y>Max_Speed)   Velocity.y = Max_Speed;

    //Полное торможение происходит достаточно долго (несколько секунд)
    //поэтому если тормозим с очень маленькой скорость - принудительно останавливаемся
    if (abs(PreviousVelocity.x) > abs(Velocity.x))
    {
        if (abs(Velocity.x) < delta*100)
            Velocity.x = 0;
    }

    // Промежуточная позиция после расчётов физики
    if (!isGroundTilted.GetState())
    {
        //Обычное перемещение
        Position.x += Velocity.x;
        Position.y += Velocity.y;
    }
    else
    {
        //Перемещение по наклонной
        if (Velocity.y == 0)
        {
            //Платформа со спуском 
            if (TiltPoint1.y < TiltPoint2.y)
            {
                //Спуск
                if (Velocity.x > 0)
                {
                    Position = GetNewPositionOnTiltLine(TiltPoint2);
                }
                //Подъём
                else
                {
                    Position = GetNewPositionOnTiltLine(TiltPoint1);
                }
                
            }

            //Платформа с подъёмом
            if (TiltPoint1.y > TiltPoint2.y)
            {
                //Спуск
                if (Velocity.x < 0)
                {
                    Position = GetNewPositionOnTiltLine(TiltPoint1);
                }
                //Подъём
                else
                {
                    Position = GetNewPositionOnTiltLine(TiltPoint2);
                }
            }
        }

    }

    // -------------------------------------
    //
    //         ОБРАБОТКА ПЕРЕСЕЧЕНИЙ
    //    + обновление статусов положений
    //
    // -------------------------------------

    OnTheGround.SetFalse();
    OnTheLeftWall.SetFalse();
    OnTheRightWall.SetFalse();

    isLanding.SetFalse();
    isGroundTilted.SetFalse();

    // ----------------------------------------
    //
    //      1. Пересечение с краями карты
    //
    // ----------------------------------------

    //Если достигли правой границы
    if (Position.x >= game_values.GetMapSize().x - IntersectBoindingBoxSize.x / 2)
    {
        Position.x = game_values.GetMapSize().x - IntersectBoindingBoxSize.x / 2;
        Velocity.x = 0;
        Acceleration = 0;
        OnTheLeftWall.SetTrue();
    }

    //Если достигли левой границы
    if (Position.x <= IntersectBoindingBoxSize.x / 2)
    {
        Position.x = IntersectBoindingBoxSize.x / 2;
        Velocity.x = 0;
        Acceleration = 0;
        OnTheRightWall.SetTrue();
    }

    //Если достигли нижней границы
    if (Position.y >= game_values.GetMapSize().y - IntersectBoindingBoxSize.y / 2)
    {
        Position.y = game_values.GetMapSize().y - IntersectBoindingBoxSize.y / 2;
        Velocity.y = 0;
        JumpImpulse = 0;
        OnTheGround.SetTrue();

        if (PreviousPosition.y < Position.y)
            isLanding.SetTrue();
    }

    //Если достигли верхней границы
    if (Position.y <= IntersectBoindingBoxSize.y / 2)
    {
        Position.y = IntersectBoindingBoxSize.y / 2;
        Velocity.y = 0;
        JumpImpulse = 0;
    }

    // ----------------------------------------
    //
    //      2. Пересечение с платформами
    //
    // ----------------------------------------

    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_platform* Platform = dynamic_cast<c_platform*>(c_drawobject::DrawObjects[obj_num]);

        //Если объект не платформа dynamic_cast вернёт NULL
        if (Platform)
        {
            //Если пересекаемся, то обрабатываем столкновение и вычисляем новую позицию
            if (GetIntersectBoundingBox().Intersect(&Platform->GetBoundingBox()))
            {
                //Разделяем обычные (tilt_type = 0) и наклонные платформы
                if (!Platform->TiltType)
                    Position = GetNewPosition_Rect(GetIntersectBoundingBox(), Platform->GetBoundingBox());
                else
                {
                    //Стоим на наклонной платформе ?
                    if ((Platform->TiltType > 0) && TestPoint_Tilt(Position, *Platform))
                    {
                        isGroundTilted.SetTrue();
                    }
                    else
                        //Если не стоим на наклонной поверхности, то получаем новую позицию.
                        //Функция также устанавливает точки прямой и isGroundTilted, если встали.
                        Position = GetNewPosition_Tilt(GetIntersectBoundingBox(), Platform->GetBoundingBox(), Platform->TiltType, Platform->TiltLevel, Platform->TiltNumber);
                }
            }

            //TODO: 'стоим' - наддо уменьшить количество проверяемых блоков, брыть только ближайшее (если только которые пересекаем корректно не работает)

            //Стоим на наклонной платформе
            if (isGroundTilted.GetState())
            {
                OnTheGround.SetTrue();

                if (PreviousPosition.y < Position.y)
                    isLanding.SetTrue();
            }

            //Стоим на обычной платформе или на наклонной с прямым верхом
            if ((Platform->TiltType <= 0) &&
                (GetIntersectBoundingBox().y2 == Platform->GetBoundingBox().y1) &&
                (GetIntersectBoundingBox().x2 > Platform->GetBoundingBox().x1) &&
                (GetIntersectBoundingBox().x1 < Platform->GetBoundingBox().x2))
            {
                OnTheGround.SetTrue();

                if (PreviousPosition.y < Position.y)
                    isLanding.SetTrue();
            }

            //Упераемся в левую стенку платформы
            if ((Platform->TiltType == 0) || (Platform->TiltType == 3) || (Platform->TiltType == 4) || (Platform->TiltType == -1) || (Platform->TiltType == -2))
            {
                if ((GetIntersectBoundingBox().x2 == Platform->GetBoundingBox().x1) && (GetIntersectBoundingBox().y2 > Platform->GetBoundingBox().y1) && (GetIntersectBoundingBox().y1 < Platform->GetBoundingBox().y2))
                    OnTheLeftWall.SetTrue();
            }

            //У упераемся в правую стенку платформы
            if ((Platform->TiltType == 0) || (Platform->TiltType == 1) || (Platform->TiltType == 2) || (Platform->TiltType == -3) || (Platform->TiltType == -4))
            {
                if ((GetIntersectBoundingBox().x1 == Platform->GetBoundingBox().x2) && (GetIntersectBoundingBox().y2>Platform->GetBoundingBox().y1) && (GetIntersectBoundingBox().y1 < Platform->GetBoundingBox().y2))
                    OnTheRightWall.SetTrue();
            }
        }
    }

    // ----------------------------------------
    //
    // Обновляем информацию о направлениях движения
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

    //Актульные значения скорости и позиции
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
    
    //Определим номер угла спрайта, который мы пересекли 
    //Верхний левый - 1, далее по часовой
    size_t angle_number = 0;

    //1.Верхний левый
    if (BoundingBox1.TestPoint(BoundingBox2.x1, BoundingBox2.y1))
    {
        angle_number = 1;
        C = hgeVector(BoundingBox2.x1, BoundingBox2.y1);
    }
    //2.Верхний правый
    if (BoundingBox1.TestPoint(BoundingBox2.x2, BoundingBox2.y1))
    {
        angle_number = 2;
        C = hgeVector(BoundingBox2.x2, BoundingBox2.y1);
    }
    //3.Нижний правый
    if (BoundingBox1.TestPoint(BoundingBox2.x2, BoundingBox2.y2))
    {
        angle_number = 3;
        C = hgeVector(BoundingBox2.x2, BoundingBox2.y2);
    }
    //4.Нижний левый
    if (BoundingBox1.TestPoint(BoundingBox2.x1, BoundingBox2.y2))
    {
        angle_number = 4;
        C = hgeVector(BoundingBox2.x1, BoundingBox2.y2);
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
    //Если углы спрайтов не пересекались, то проверяем стороны
    {
        //1.Верхняя сторона
        if (PreviousPosition.y < BoundingBox2.y1)
        {
            NewPosition.y = BoundingBox2.y1 - Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //2.Нижняя сторона
        if (PreviousPosition.y > BoundingBox2.y2)
        {
            NewPosition.y = BoundingBox2.y2 + Size_y;
            Velocity.y = 0;
            JumpImpulse = 0;
        }

        //3.Левая сторона
        if (PreviousPosition.x < BoundingBox2.x1)
        {
            NewPosition.x = BoundingBox2.x1 - Size_x;
            Velocity.x = 0;
            Acceleration = 0;
        }

        //4.Правая сторона
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
    hgeVector A1 = PreviousPosition;
    hgeVector B1 = Position;

    hgeVector NewPosition(0, 0);

    //Смещаем точки, так как 'A1B1' - линия перемещения середины нижней или верхней границы спрайта, а не центра
    float Size_y = (BoundingBox1.y2 - BoundingBox1.y1) / 2;

    if (tilt_type > 0)
    {
        A1.y = A1.y + Size_y;
        B1.y = B1.y + Size_y;

        NewPosition.y -= Size_y;
    }
    else
    {
        A1.y = A1.y - Size_y;
        B1.y = B1.y - Size_y;

        NewPosition.y += Size_y;
    }

    // Общее уравнения прямых
    // AB: (A.y - B.y)*X + (B.x - A.x)*Y + (A.x*B.y - B.x*A.y) = 0

    //----------------------------------------------------------
    //
    //        Уравнение ската наклонного припятствия
    //
    //----------------------------------------------------------

    hgeVector A2;
    hgeVector B2;

    c_game_values& game_values = c_game_values::getInstance();

    switch (abs(tilt_type))
    {
    case 1:
        A2.x = BoundingBox2.x1 - (tilt_number - 1)*game_values.GetTileSize().x;
        A2.y = BoundingBox2.y2;
        B2.x = BoundingBox2.x2 + (tilt_level - tilt_number)*game_values.GetTileSize().x;
        B2.y = BoundingBox2.y1;
        break;
    case 2:
        A2.x = BoundingBox2.x1;
        A2.y = BoundingBox2.y2 + (tilt_number - 1)*game_values.GetTileSize().y;
        B2.x = BoundingBox2.x2;
        B2.y = BoundingBox2.y1 - (tilt_level - tilt_number)*game_values.GetTileSize().y;
        break;
    case 3:
        A2.x = BoundingBox2.x1 - (tilt_number - 1)*game_values.GetTileSize().x;
        A2.y = BoundingBox2.y1;
        B2.x = BoundingBox2.x2 + (tilt_level - tilt_number)*game_values.GetTileSize().x;
        B2.y = BoundingBox2.y2;
        break;
    case 4:
        A2.x = BoundingBox2.x1;
        A2.y = BoundingBox2.y1 - (tilt_number - 1)*game_values.GetTileSize().y;
        B2.x = BoundingBox2.x2;
        B2.y = BoundingBox2.y2 + (tilt_level - tilt_number)*game_values.GetTileSize().y;
        break;
    default:
        break;
    }

    //Проверяем ниже ли точка B1 (текущая позиция) относительно прямой A2B2
    //Так как ось Y инвертрована (отсчёт идёт сверху вниз),
    //то значение больше нуля означает, что точка лежит под линией
    //
    //tilt_type / abs(tilt_type) = +1 or -1 - совмещает в одном условие пересечение снизу и сверху
    if (((tilt_type / abs(tilt_type)) *(A2.y - B2.y)*B1.x + (B2.x - A2.x)*B1.y + (A2.x*B2.y - B2.x*A2.y)) > 0)
    {
        //Отсекаем пересечения с ненаклонными сторонами
        //Предыдущая позиция (A1) с той же 'недопустимой' стороны линии, что и нынешняя
        if (((tilt_type / abs(tilt_type)) *(A2.y - B2.y)*A1.x + (B2.x - A2.x)*A1.y + (A2.x*B2.y - B2.x*A2.y)) > 0)
            return GetNewPosition_Rect(BoundingBox1, BoundingBox2);

        // Перемещаем позицию в точку пересечения прямых
        // A1B1: (A1.y - B1.y)*X + (B1.x - A1.x)*Y + (A1.x*B1.y - B1.x*A1.y) = 0
        // A2B2: (A2.y - B2.y)*X + (B2.x - A2.x)*Y + (A2.x*B2.y - B2.x*A2.y) = 0
        // X = - (C1*B2 - C2*B1) / (A1*B2 - A2*B1)
        // Y = - (A1*C2 - A2*C1) / (A1*B2 - A2*B1)

        NewPosition.x += -(((A1.x*B1.y - B1.x*A1.y)*(B2.x - A2.x) - (A2.x*B2.y - B2.x*A2.y)*(B1.x - A1.x)) / ((A1.y - B1.y)*(B2.x - A2.x) - (A2.y - B2.y)*(B1.x - A1.x)));
        NewPosition.y += -(((A1.y - B1.y)*(A2.x*B2.y - B2.x*A2.y) - (A2.y - B2.y)*(A1.x*B1.y - B1.x*A1.y)) / ((A1.y - B1.y)*(B2.x - A2.x) - (A2.y - B2.y)*(B1.x - A1.x)));

        Velocity.y = 0;
        JumpImpulse = 0;

        //Устанавливаем прямую по которой теперь будем перемещаться.
        TiltPoint1 = A2;
        TiltPoint2 = B2;

        isGroundTilted.SetTrue();
    }
    else
        //Не меняем позицию, так как ещё ничего не пересекли
        return  Position;

    return NewPosition;
 }

 hgeVector c_gameobject::GetNewPositionOnTiltLine(hgeVector PointToMove)
 {
     hgeVector NewPostion;

     float L1, L2;
     float dL;

     L1 = abs(Velocity.x);
     L2 = sqrt(pow((PointToMove.x - Position.x), 2) + pow((PointToMove.y - Position.y), 2));
     dL = L1 / L2;

     NewPostion.x = (Position.x + dL*PointToMove.x) / (1 + dL);
     NewPostion.y = (Position.y + dL*PointToMove.y) / (1 + dL);
    
     return NewPostion;
 }

bool c_gameobject::TestPoint_Tilt(hgeVector position, c_platform &platform)
{
    //С - середина нижней границы спрайта, а не центра
    hgeVector C = Position;
    C.y = C.y + IntersectBoindingBoxSize.y/2;

    // Общее уравнение прямой
    // AB: (A.y - B.y)*X + (B.x - A.x)*Y + (A.x*B.y - B.x*A.y) = 0

    hgeVector A;
    hgeVector B;

    c_game_values& game_values = c_game_values::getInstance();

    int tilt_type       = platform.TiltType;
    int tilt_level      = platform.TiltLevel;
    int tilt_number     = platform.TiltNumber;
    hgeRect BoundingBox = platform.GetBoundingBox();
        
    switch (abs(tilt_type))
    {
    case 1:
        A.x = BoundingBox.x1 - (tilt_number - 1)*game_values.GetTileSize().x;
        A.y = BoundingBox.y2;
        B.x = BoundingBox.x2 + (tilt_level - tilt_number)*game_values.GetTileSize().x;
        B.y = BoundingBox.y1;
        break;
    case 2:
        A.x = BoundingBox.x1;
        A.y = BoundingBox.y2 + (tilt_number - 1)*game_values.GetTileSize().y;
        B.x = BoundingBox.x2;
        B.y = BoundingBox.y1 - (tilt_level - tilt_number)*game_values.GetTileSize().y;
        break;
    case 3:
        A.x = BoundingBox.x1 - (tilt_number - 1)*game_values.GetTileSize().x;
        A.y = BoundingBox.y1;
        B.x = BoundingBox.x2 + (tilt_level - tilt_number)*game_values.GetTileSize().x;
        B.y = BoundingBox.y2;
        break;
    case 4:
        A.x = BoundingBox.x1;
        A.y = BoundingBox.y1 - (tilt_number - 1)*game_values.GetTileSize().y;
        B.x = BoundingBox.x2;
        B.y = BoundingBox.y2 + (tilt_level - tilt_number)*game_values.GetTileSize().y;
        break;
    default:
        break;
    }

    //Проверяем что точка С (текущая нижняя позиция) находится на прямой AB (склон)
    float Result = (A.y - B.y)*C.x + (B.x - A.x)*C.y + (A.x*B.y - B.x*A.y);
    
    //Сверяем с эпсилон (пусть бует 0.0001f)
    const float eps = 0.0001f;

    if (abs(Result) < eps)
        Result = 0.0f;

    if (Result == 0)
        return true;
    else
        return false;
}