#include "c_player.h"

//�����������
c_player::c_player(u_int size)
{
    //��������� ����������
    SetPosition(hgeVector(2300, 3000));
    PlayerPosition = Position;

    Size = size;

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

    Sprite = new hgeSprite(Texture, 0 + 0.5f, 0 + 0.5f, Size * 2, Size * 2);
    Sprite->SetHotSpot(Size, Size);
    Sprite->SetColor(ARGB(255, 255, 255, 255));

    //Sprite = new hgeAnimation(Texture, 4, 4, 0, 0, 64, 29);
    //Sprite->Play();
}

//����������
c_player::~c_player()
{
    delete(Sprite);
}

//��������� ������ ������� �������
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

    //��������� ������������, ��� ������ ��������� ������ ��������
    if (!hge->Input_GetKeyState(HGEK_RIGHT) && !hge->Input_GetKeyState(HGEK_LEFT))
        Acceleration = Min_Acceleration;

    if (hge->Input_GetKeyState(HGEK_SPACE))
    {
        //���� ��������� �� ����� � ������ ������ ��� �����:
        if (OnTheGround.GetState() && !SpaceHoldDown.GetState())
        {
            //��������� ���������� ������� ������ ��� ������ ������
            SpaceHoldDown.SetTrue();

            OnTheGround.SetFalse();

            //��� ������ � ����� ��� ������������ �������
            JumpImpulse = Max_JumpImpulse;
        }

        //���� ���������� ������� ������, ������� ����������� ����������
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

        //���� ��������� ������� ������, ������� ��������
        JumpImpulse = 0;
    }


    //���� ���� ������
    Velocity.x *= Friction;
    Velocity.y *= Friction;

    //����������� ������������ ��������
    if (Velocity.x>Max_Speed)   Velocity.x = Max_Speed;
    if (Velocity.y>Max_Speed)   Velocity.y = Max_Speed;


    // ���������� ������� ����� ���� ��������
    Position.x += Velocity.x;
    Position.y += Velocity.y;


    //���� �������� ������ �������
    if (Position.x >= c_game::MAP_SIZE.x - Size)
    {
        Position.x = c_game::MAP_SIZE.x - Size;
    }

    //���� �������� ����� �������
    if (Position.x <= Size)
    {
        Position.x = Size;
    }

    //���� �������� ������ �������
    if (Position.y >= c_game::MAP_SIZE.y - Size)
    {
        Position.y = c_game::MAP_SIZE.y - Size;
        Velocity.y = 0;
        OnTheGround.SetTrue();
    }

    //���� �������� ������� �������
    if (Position.y <= Size)
    {
        Position.y = Size;
    }


    //��������� ������������
    BoundingBox = GetBoundingBox();

    //������ �������� ����� �� �� �� ��������� ��� �����
    OnTheGround.SetFalse();
    for (size_t obj_num = 0; obj_num < c_drawobject::DrawObjects.size(); obj_num++)
    {
        c_platform* Platform = dynamic_cast<c_platform*>(c_drawobject::DrawObjects[obj_num]);

        //���� ������ �� ��������� dynamic_cast ����� NULL
        if (Platform)
        {
            //���� ������������, �� ������������ ������������
            if (BoundingBox.Intersect(&Platform->GetBoundingBox()))
            {
                if (PreviousPosition.y < Platform->BoundingBox.y1)
                {
                    Position.y -= BoundingBox.y2 - Platform->BoundingBox.y1;
                }
                else
                    if (PreviousPosition.y > Platform->BoundingBox.y2)
                    {
                    Position.y += Platform->BoundingBox.y2 - BoundingBox.y1;
                    JumpImpulse = 0;
                    }
                    else
                        if (Position.x < Platform->Position.x)
                            Position.x -= BoundingBox.x2 - Platform->BoundingBox.x1;
                        else
                            Position.x += Platform->BoundingBox.x2 - BoundingBox.x1;
            }

            if (((BoundingBox.y2 == Platform->BoundingBox.y1) && (BoundingBox.x2>Platform->BoundingBox.x1) && (BoundingBox.x1 < Platform->BoundingBox.x2))
                || (Position.y >= c_game::MAP_SIZE.y - Size))
                OnTheGround.SetTrue();
        }
    }

    PlayerPosition = Position;

    //���� ����� �� ������ ���� ������
    if (Position.x <= GetScreenWidth() / 2)
    {
        PlayerPosition.x = GetScreenWidth()/2;
    }

    //���� ����� �� ������� ���� ������
    if (Position.x >= c_game::MAP_SIZE.x - GetScreenWidth()/2)
    {
        PlayerPosition.x = c_game::MAP_SIZE.x - GetScreenWidth()/2;
    }

    //���� ����� �� �������� ���� ������
    if (Position.y <= GetScreenHeight() / 2)
    {
        PlayerPosition.y = GetScreenHeight() / 2;
    }

    //���� ����� �� ������� ���� ������
    if (Position.y >= c_game::MAP_SIZE.y - GetScreenHeight()/2)
    {
        PlayerPosition.y = c_game::MAP_SIZE.y - GetScreenHeight()/2;
    }


    Render();
}
