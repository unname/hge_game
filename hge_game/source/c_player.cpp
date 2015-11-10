#include "c_player.h"

//конструктор
c_player::c_player()
{
    //Установка переменных
    Position =  hgeVector(2300, 2300);
    PlayerPosition = Position;

    Speed               = 3.0;
    Max_Speed           = Speed * 30;

    Min_Acceleration    = 1.0;
    Max_Acceleration    = 8.0;
    Acceleration        = Min_Acceleration;
    Acceleration_step   = 0.5;

    Max_JumpImpulse     = g + 100;
    JumpImpulse         = 0.0;
    JumpImpulse_step    = 4.0;

    GroundFriction      = 0.99;
    AirFriction         = 0.97;

    SpeedForBraking     = 6;

    std::string player_tex_path;
    player_tex_path.append(RESOURCES_PATH);
    player_tex_path.append(PLAYER_TEX_NAME);
    Texture = hge->Texture_Load(player_tex_path.c_str());
    if (!Texture)
    {
        DisplayErrorHGE();
    }

    PlayerAnimation = new c_anim_manager(Sprite);

    //Добавляем анмации для игрока
    Standing = new hgeAnimation(Texture, 3, 3, 0, 0, 36, 64);
    Running = new hgeAnimation(Texture, 6, 6, 120, 0, 51, 64);
    Braking = new hgeAnimation(Texture, 3, 9, 440, 0, 56, 64);
    Jumping = new hgeAnimation(Texture, 2, 9, 625, 0, 36, 70);
    Falling = new hgeAnimation(Texture, 1, 1, 700, 0, 39, 64);
    Landing = new hgeAnimation(Texture, 1, 1, 740, 0, 37, 64);

    PlayerAnimation->AddElement(Standing, "Standing");
    PlayerAnimation->AddElement(Running, "Running");
    PlayerAnimation->AddElement(Braking, "Braking", HGEANIM_FWD | HGEANIM_NOLOOP);
    PlayerAnimation->AddElement(Jumping, "Jumping", HGEANIM_FWD | HGEANIM_NOLOOP);
    PlayerAnimation->AddElement(Falling, "Falling", HGEANIM_FWD | HGEANIM_NOLOOP);
    PlayerAnimation->AddElement(Landing, "Landing", HGEANIM_FWD | HGEANIM_NOLOOP);

    //В начале стоим
    Sprite = Standing;

    //Выставляем общий IntersectBoindingBox для всех кадров
    //Принимаем равным Standing->BoundingBox 
    Standing->GetBoundingBox(Standing->GetWidth() / 2, Standing->GetHeight() / 2, &IntersectBoindingBox);
}

//деструктор
c_player::~c_player()
{
    //Текстуры
    if (Texture)
        hge->Texture_Free(Texture);

    //Анимации
    delete(Standing);
    delete(Running);
    delete(Braking);
    delete(Jumping);
    delete(Falling);
    delete(Landing);

    delete(PlayerAnimation);
}

//Повторный расчёт свойств объекта
void c_player::Update(float delta)
{
    c_game_values& game_values = c_game_values::getInstance();


    //Анимация стояния/презимления
    if (Moving.NotMoving.GetState())
    {
        //if (isLanding.GetState())
        //{
        //    PlayerAnimation->Animate("Landing", delta, &Moving);
        //}
        //else
            PlayerAnimation->Animate("Standing", delta, &Moving);
    }
    else
    {
        //Анимация бега/торможения
        if (OnTheGround.GetState())
        {
            if (isBraking.GetState())
            {
                PlayerAnimation->Animate("Braking", delta, &Moving);
            }
            else
                PlayerAnimation->Animate("Running", delta, &Moving);
        }
        else
        {
            //Анимация прыжка/падения
            if (Moving.MovingUp.GetState())
                PlayerAnimation->Animate("Jumping", delta, &Moving);
            else
                PlayerAnimation->Animate("Falling", delta, &Moving);
        }
    }

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
    {
        Acceleration = Min_Acceleration;

        //Если двигаемся на большой скорости -> Торможение
        if (!Moving.NotMoving.GetState())
        {
            if (abs(Velocity.x) > SpeedForBraking)
                isBraking.SetTrue();
        }
        else
            //Если остановились, сбрасываем торможение
            isBraking.SetFalse();
    }
    else
        //Как только начали двигаться, сбрасываем торможение
        isBraking.SetFalse();

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