#include "c_player.h"

//конструктор
c_player::c_player(u_int size)
{
    //Установка переменных
    SetPosition(hgeVector(2300, 3000));
    PlayerPosition = Position;

    Size_x = size;
    Size_y = size;

    Speed = 3.0;
    Max_Speed = Speed * 30;

    Min_Acceleration = 1.0;
    Max_Acceleration = 8.0;
    Acceleration = Min_Acceleration;
    Acceleration_step = 0.5;

    Max_JumpImpulse = g + 100;
    JumpImpulse = 0.0;
    JumpImpulse_step = 4.0;

    Friction = 0.96;

    Texture = hge->Texture_Load("resources/cube_tex.jpg");
    if (!Texture)
    {
        DisplayErrorHGE();
    }

    Sprite = new hgeSprite(Texture, 0 + 0.5f, 0 + 0.5f, Size_x * 2, Size_y * 2);
    Sprite->SetHotSpot(Size_x, Size_y);
    Sprite->SetColor(ARGB(255, 255, 255, 255));

    //Sprite = new hgeAnimation(Texture, 4, 4, 0, 0, 64, 29);
    //Sprite->Play();
}

//деструктор
c_player::~c_player()
{
    delete(Sprite);
}

//Повторный расчёт свойств объекта
void c_player::Update(float delta)
{
    c_gameobject::Update(delta);

    if (hge->Input_GetKeyState(HGEK_LEFT))
    {
        if (Acceleration < Max_Acceleration)
            Acceleration += Acceleration_step;
        else
            Acceleration = Max_Acceleration;

        Velocity.x -= Speed*Acceleration*delta;
    }
  
    if (hge->Input_GetKeyState(HGEK_RIGHT))
    {
        if (Acceleration < Max_Acceleration)
            Acceleration += Acceleration_step;
        else
            Acceleration = Max_Acceleration;

        Velocity.x += Speed*Acceleration*delta;
    }

    //Ускорение сбрасывается, как только отпускаем кнопку движения
    if (!hge->Input_GetKeyState(HGEK_RIGHT) && !hge->Input_GetKeyState(HGEK_LEFT))
        Acceleration = Min_Acceleration;

    if (hge->Input_GetKeyState(HGEK_SPACE))
    {
        //Если находимся на земле и пробел только что нажат:
        if (OnTheGround.GetState() && !SpaceHoldDown.GetState())
        {
            //Позволяем удерживать клавишу прыжка для набора высоты
            SpaceHoldDown.SetTrue();

            OnTheGround.SetFalse();

            //При прыжке с земли даём максимальный импульс
            JumpImpulse = Max_JumpImpulse;
        }

        //Если удерживаем клавишу прыжка, импульс уменьшается постепенно
        if (SpaceHoldDown.GetState())
        {
            if (JumpImpulse > 0)
                JumpImpulse -= JumpImpulse_step;
            else
                JumpImpulse = 0;
        }

        Velocity.y -= JumpImpulse*delta;
    }
    else
    {
        SpaceHoldDown.SetFalse();

        //Если отпустили клавишу прыжка, импульс исчезает
        JumpImpulse = 0;
    }


    //Учёт силы трения
    Velocity.x *= Friction;
    Velocity.y *= Friction;

    //Ограничение максимальной скорости
    if (Velocity.x>Max_Speed)   Velocity.x = Max_Speed;
    if (Velocity.y>Max_Speed)   Velocity.y = Max_Speed;


    // Актуальная позиция после всех расчётов
    Position.x += Velocity.x;
    Position.y += Velocity.y;


    //Если достигли правой границы
    if (Position.x >= c_game::MAP_SIZE.x - Size_x)
    {
        Position.x = c_game::MAP_SIZE.x - Size_x;
    }

    //Если достигли левой границы
    if (Position.x <= Size_x)
    {
        Position.x = Size_x;
    }

    //Если достигли нижней границы
    if (Position.y >= c_game::MAP_SIZE.y - Size_y)
    {
        Position.y = c_game::MAP_SIZE.y - Size_y;
        Velocity.y = 0;
        OnTheGround.SetTrue();
    }

    //Если достигли верхней границы
    if (Position.y <= Size_y)
    {
        Position.y = Size_y;
    }


    //Обработка столкновений
    BoundingBox = GetBoundingBox();

    //Заного проверим стоим ли мы на платформе или земле
    OnTheGround.SetFalse();
    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_platform* Platform = dynamic_cast<c_platform*>(c_drawobject::DrawObjects[obj_num]);

        //Если объект не платформа dynamic_cast вернёт NULL
        if (Platform)
        {
            //Если пересекаемся, то обрабатываем столкновение и вычисляем новую позицию
            if (BoundingBox.Intersect(&Platform->GetBoundingBox()))
            {
                //Проверяем какой угол платформы мы пересекли

                //1.Верхний левый
                if (BoundingBox.TestPoint(Platform->BoundingBox.x1, Platform->BoundingBox.y1))
                    Position = GetNewPosition(PreviousPosition, Position, hgeVector(Platform->BoundingBox.x1, Platform->BoundingBox.y1), 1);
                else
                    //2.Верхний правый
                    if (BoundingBox.TestPoint(Platform->BoundingBox.x2, Platform->BoundingBox.y1))
                        Position = GetNewPosition(PreviousPosition, Position, hgeVector(Platform->BoundingBox.x2, Platform->BoundingBox.y1), 2);
                    else
                        //3.Нижний правый
                        if (BoundingBox.TestPoint(Platform->BoundingBox.x2, Platform->BoundingBox.y2))
                            Position = GetNewPosition(PreviousPosition, Position, hgeVector(Platform->BoundingBox.x2, Platform->BoundingBox.y2), 3);
                        else
                            //4.Нижний левый
                            if (BoundingBox.TestPoint(Platform->BoundingBox.x1, Platform->BoundingBox.y2))
                                Position = GetNewPosition(PreviousPosition, Position, hgeVector(Platform->BoundingBox.x1, Platform->BoundingBox.y2), 4);
                            else

                                //Если не один из углов не был пересечён, то проверяем стороны

                                //1.Верхняя сторона
                                if (PreviousPosition.y < Platform->BoundingBox.y1)
                                {
                                    Position.y = Platform->BoundingBox.y1 - Size_y;
                                    Velocity.y = 0;
                                }
                                else
                                    //2.Нижняя сторона
                                    if (PreviousPosition.y > Platform->BoundingBox.y2)
                                    {
                                        Position.y = Platform->BoundingBox.y2 + Size_y;
                                        Velocity.y = 0;
                                    }
                                    else
                                        //3.Левая сторона
                                        if (PreviousPosition.x < Platform->BoundingBox.x1)
                                        {
                                            Position.x = Platform->BoundingBox.x1 - Size_x;
                                            Velocity.x = 0;
                                        }
                                        else
                                            //4.Правая сторона
                                            if (PreviousPosition.x > Platform->BoundingBox.x2)
                                            {
                                                Position.x = Platform->BoundingBox.x2 + Size_x;
                                                Velocity.x = 0;
                                            }
            }


            //Обновим позицию
            BoundingBox = GetBoundingBox();

            //Если достигли нижнего края карты или встали на платформу
            if (((BoundingBox.y2 == Platform->BoundingBox.y1) && (BoundingBox.x2>Platform->BoundingBox.x1) && (BoundingBox.x1 < Platform->BoundingBox.x2))
                || (Position.y >= c_game::MAP_SIZE.y - Size_y))
                OnTheGround.SetTrue();
        }
    }

    PlayerPosition = Position;

    //Если дошли до левого края экрана
    if (Position.x <= GetScreenWidth() / 2)
    {
        PlayerPosition.x = GetScreenWidth()/2;
    }

    //Если дошли до правого края экрана
    if (Position.x >= c_game::MAP_SIZE.x - GetScreenWidth()/2)
    {
        PlayerPosition.x = c_game::MAP_SIZE.x - GetScreenWidth()/2;
    }

    //Если дошли до верхнего края экрана
    if (Position.y <= GetScreenHeight() / 2)
    {
        PlayerPosition.y = GetScreenHeight() / 2;
    }

    //Если дошли до нижнего края экрана
    if (Position.y >= c_game::MAP_SIZE.y - GetScreenHeight()/2)
    {
        PlayerPosition.y = c_game::MAP_SIZE.y - GetScreenHeight()/2;
    }

    Render();
}
