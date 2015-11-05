#include "c_player.h"

//конструктор
c_player::c_player(u_int size)
{
    //Установка переменных
    Position =  hgeVector(100, 100);
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

    Texture = hge->Texture_Load("resources/player.png");
    if (!Texture)
    {
        DisplayErrorHGE();
    }

    Standing = new hgeAnimation(Texture, 3, 3, 0 + 0.5f, 0 + 0.5f, 64, 128);
    Running  = new hgeAnimation(Texture, 6, 6, 6 * 64 + 0.5f, 0 + 0.5f, 64, 128);
    Braking  = new hgeAnimation(Texture, 3, 3, 0 + 0.5f, 0 + 0.5f, 64, 128);
    Jumping  = new hgeAnimation(Texture, 3, 3, 0 + 0.5f, 0 + 0.5f, 64, 128);
    Falling  = new hgeAnimation(Texture, 3, 3, 0 + 0.5f, 0 + 0.5f, 64, 128);

    Sprite->SetHotSpot(64/2, 128/2);
    Sprite->SetColor(ARGB(255, 255, 255, 255));

    Sprite = Standing;
    Standing->Play();
    CurrentAnimation = Standing;
}

//деструктор
c_player::~c_player()
{
    if (Texture)
        hge->Texture_Free(Texture);

    delete(Sprite);
}

//Повторный расчёт свойств объекта
void c_player::Update(float delta)
{
    c_game_values& game_values = c_game_values::getInstance();

    //Анимация стояния на месте
    if (Moving.NotMoving.GetState())
    {
        StandingAnim(delta);
    }

    if (hge->Input_GetKeyState(HGEK_LEFT) && !OnTheRightWall.GetState())
    {
        if (Acceleration < Max_Acceleration)
            Acceleration += Acceleration_step;
        else
            Acceleration = Max_Acceleration;

        Velocity.x -= Speed*Acceleration*delta;

        //Анимация бега
        RunningAnim(delta);

    }
  
    if (hge->Input_GetKeyState(HGEK_RIGHT) && !OnTheLeftWall.GetState())
    {
        if (Acceleration < Max_Acceleration)
            Acceleration += Acceleration_step;
        else
            Acceleration = Max_Acceleration;

        Velocity.x += Speed*Acceleration*delta;

        //Анимация бега
        RunningAnim(delta);
    }

    //Ускорение сбрасывается, как только отпускаем кнопку движения
    if (!hge->Input_GetKeyState(HGEK_RIGHT) && !hge->Input_GetKeyState(HGEK_LEFT))
    {
        Acceleration = Min_Acceleration;

        //Анимация торможения
        BrakingAnim(delta);
    }

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

    c_gameobject::Update(delta);

    //-------------------------------------------
    //
    //  Проверка изменений в положении камеры
    //
    //-------------------------------------------

    PlayerPosition = Position;

    //Если дошли до левого края экрана
    if (Position.x <= GetScreenWidth() / 2)
    {
        PlayerPosition.x = GetScreenWidth() / 2;
    }

    //Если дошли до правого края экрана
    if (Position.x >= game_values.GetMapSize().x - GetScreenWidth() / 2)
    {
        PlayerPosition.x = game_values.GetMapSize().x - GetScreenWidth() / 2;
    }

    //Если дошли до верхнего края экрана
    if (Position.y <= GetScreenHeight() / 2)
    {
        PlayerPosition.y = GetScreenHeight() / 2;
    }

    //Если дошли до нижнего края экрана
    if (Position.y >= game_values.GetMapSize().y - GetScreenHeight() / 2)
    {
        PlayerPosition.y = game_values.GetMapSize().y - GetScreenHeight() / 2;
    }
}

void c_player::StandingAnim(float dt)
{
    if (CurrentAnimation != Standing)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если двигались влево
    if (Moving.MovingLeft.GetState())
        Standing->SetFlip(true, false);

    if (Standing->IsPlaying())
        Standing->Update(dt);
    else
        Standing->Play();

    CurrentAnimation = Standing;
    Sprite = Standing;
}

void c_player::RunningAnim(float dt)
{
    if (CurrentAnimation != Running)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если двигаемся влево
    if (Moving.MovingLeft.GetState())
        Running->SetFlip(true, false);

    if (Running->IsPlaying())
        Running->Update(dt);
    else
        Running->Play();

    CurrentAnimation = Running;
    Sprite = Running;
}

void c_player::BrakingAnim(float dt)
{
    if (CurrentAnimation != Braking)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если двигаемся влево
    if (Moving.MovingLeft.GetState())
        Braking->SetFlip(true, false);

    if (Braking->IsPlaying())
        Braking->Update(dt);
    else
        Braking->Play();

    CurrentAnimation = Braking;
    Sprite = Braking;
}

void c_player::JumpingAnim(float dt)
{
    if (CurrentAnimation != Jumping)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если двигаемся влево
    if (Moving.MovingLeft.GetState())
        Jumping->SetFlip(true, false);

    if (Jumping->IsPlaying())
        Jumping->Update(dt);
    else
        Jumping->Play();

    CurrentAnimation = Jumping;
    Sprite = Jumping;
}

void c_player::FallingAnim(float dt)
{
    if (CurrentAnimation != Falling)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если двигаемся влево
    if (Moving.MovingLeft.GetState())
        Falling->SetFlip(true, false);

    if (Falling->IsPlaying())
        Falling->Update(dt);
    else
        Falling->Play();

    CurrentAnimation = Falling;
    Sprite = Falling;
}