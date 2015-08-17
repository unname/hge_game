#include "c_player.h"

//�����������
c_player::c_player(u_int size)
{
    //��������� ����������
    SetPosition(hgeVector((float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2));
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

    //����������� ���������
    //Quad.tex = hge->Texture_Load("resources/cube_tex.jpg");

    Quad.blend = BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;

    Quad.v[0].tx = 96.0 / 128.0;   Quad.v[0].ty = 64.0 / 128.0;
    Quad.v[1].tx = 128.0 / 128.0;  Quad.v[1].ty = 64.0 / 128.0;
    Quad.v[2].tx = 128.0 / 128.0;  Quad.v[2].ty = 96.0 / 128.0;
    Quad.v[3].tx = 96.0 / 128.0;   Quad.v[3].ty = 96.0 / 128.0;

    for (int i = 0; i < 4; i++)
    {
        Quad.v[i].z = 0.5f;
        Quad.v[i].col = 0xFFFFA000;
    }

    //Sprite = new hgeAnimation(Texture, 4, 4, 0, 0, 64, 29);
    //Sprite->SetHotSpot(32, 14.5);
    //Sprite->Play();
}

//����������
c_player::~c_player()
{

}

//��������� ������ ������� �������
void c_player::Update(float delta)
{
    c_gameobject::Update(delta);

    int sWidth = GetScreenWidth();
    int sHeight = GetScreenHeight();

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

    //���� ����� �� ������ ��� ������� ���� ������, �� �� ������ ��������� � ������
    if ((Position.x <= GetScreenWidth() / 2) || (Position.x >= c_game::MAP_SIZE.x - GetScreenWidth() / 2))
    {
        Quad.v[0].x = Position.x - Size;
        Quad.v[1].x = Position.x + Size;
        Quad.v[2].x = Position.x + Size;
        Quad.v[3].x = Position.x - Size;
    }
    else
    {
        Quad.v[0].x = GetScreenWidth() / 2 - Size;
        Quad.v[1].x = GetScreenWidth() / 2 + Size;
        Quad.v[2].x = GetScreenWidth() / 2 + Size;
        Quad.v[3].x = GetScreenWidth() / 2 - Size;
    }

    //���� ����� �� �������� ��� ������� ���� ������, �� �� ������ ��������� � ������
    if ((Position.y <= GetScreenHeight() / 2) || (Position.y >= c_game::MAP_SIZE.y - GetScreenHeight() / 2))
    {
        Quad.v[0].y = Position.y - Size;
        Quad.v[1].y = Position.y - Size;
        Quad.v[2].y = Position.y + Size;
        Quad.v[3].y = Position.y + Size;
    }
    else
    {
        Quad.v[0].y = GetScreenHeight() / 2 - Size;
        Quad.v[1].y = GetScreenHeight() / 2 - Size;
        Quad.v[2].y = GetScreenHeight() / 2 + Size;
        Quad.v[3].y = GetScreenHeight() / 2 + Size;
    }

    PlayerPosition = Position;

    Render();

    //Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
}
