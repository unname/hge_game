#include "c_player.h"

//конструктор
c_player::c_player(u_int size)
{
    //Установка переменных
    SetPosition(hgeVector(2300, 3000));
    PlayerPosition = Position;

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

    Sprite = new hgeSprite(Texture, 0 + 0.5f, 0 + 0.5f, size * 2, size * 2);
    Sprite->SetHotSpot(size, size);
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

    if (hge->Input_GetKeyState(HGEK_LEFT) && !OnTheRightWall.GetState())
    {
        if (Acceleration < Max_Acceleration)
            Acceleration += Acceleration_step;
        else
            Acceleration = Max_Acceleration;

        Velocity.x -= Speed*Acceleration*delta;
    }
  
    if (hge->Input_GetKeyState(HGEK_RIGHT) && !OnTheLeftWall.GetState())
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
    if (Position.x >= c_game::MAP_SIZE.x - Sprite->GetWidth()/2)
    {
        Position.x = c_game::MAP_SIZE.x - Sprite->GetWidth()/2;
    }

    //Если достигли левой границы
    if (Position.x <= Sprite->GetWidth()/2)
    {
        Position.x = Sprite->GetWidth()/2;
    }

    //Если достигли нижней границы
    if (Position.y >= c_game::MAP_SIZE.y - Sprite->GetHeight()/2)
    {
        Position.y = c_game::MAP_SIZE.y - Sprite->GetHeight()/2;
        Velocity.y = 0;
        OnTheGround.SetTrue();
    }

    //Если достигли верхней границы
    if (Position.y <= Sprite->GetHeight())
    {
        Position.y = Sprite->GetHeight();
    }


    //Обработка столкновений + обновление статусов игрока

    OnTheGround.SetFalse();
    OnTheLeftWall.SetFalse();
    OnTheRightWall.SetFalse();

    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_platform* Platform = dynamic_cast<c_platform*>(c_drawobject::DrawObjects[obj_num]);

        //Если объект не платформа dynamic_cast вернёт NULL
        if (Platform)
        {
            //Если пересекаемся, то обрабатываем столкновение и вычисляем новую позицию
            if (GetBoundingBox().Intersect(&Platform->GetBoundingBox()))
            {
                Position = GetNewPosition(Platform->GetBoundingBox());
            }

            //Если встали на платформу
            if ((GetBoundingBox().y2 == Platform->GetBoundingBox().y1) && (GetBoundingBox().x2>Platform->GetBoundingBox().x1) && (GetBoundingBox().x1 < Platform->GetBoundingBox().x2))
                OnTheGround.SetTrue();

            //Если упераемся в левую стенку платформы
            if ((GetBoundingBox().x2 == Platform->GetBoundingBox().x1) && (GetBoundingBox().y2>Platform->GetBoundingBox().y1) && (GetBoundingBox().y1 < Platform->GetBoundingBox().y2))
                OnTheLeftWall.SetTrue();

            //Если упераемся в правую стенку платформы
            if ((GetBoundingBox().x1 == Platform->GetBoundingBox().x2) && (GetBoundingBox().y2>Platform->GetBoundingBox().y1) && (GetBoundingBox().y1 < Platform->GetBoundingBox().y2))
                OnTheRightWall.SetTrue();
        }
    }

    //Если достигли нижнего края карты
    if(Position.y >= c_game::MAP_SIZE.y - Sprite->GetHeight()/2)
        OnTheGround.SetTrue();


    //-------------------------------------------
    //Изменение перемещения камеры при достижение краёв карты
    //-------------------------------------------
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
