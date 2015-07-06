#include "c_cube.h"

//конструктор
c_cube::c_cube(u_int size)
{
    if (hge = hgeCreate(HGE_VERSION))
    {
        //Установка переменных
        SetPosition(hgeVector(GetScreenWidth(hge)/2, GetScreenHeight(hge)/2));    // позиция
        Size = size;                                                          // размер
        Speed = 50.0f;                                                        // скорость
        Friction = 0.98f;                                                     // коэф.трения

        //Графические настройки
        //Quad.tex = hge->Texture_Load("images/cube_tex.jpeg");

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
    else
    {
        //MessageBox
    }
}
//деструктор
c_cube::~c_cube()
{
    if (Texture)
        hge->Texture_Free(Texture);

    //delete Sprite;

    hge->Release();
}

hgeRect c_cube::GetBoundingBox()
{
    return BoundingBox;
};

void c_cube::Render()
{
    hge->Gfx_RenderQuad(&Quad);
    //Sprite->Render(Position.x, Position.y);
}

//Повторный расчёт свойств объекта
void c_cube::Update(float delta)
{
    c_gameobject::Update(delta);

    int sWidth = GetScreenWidth(hge);
    int sHeight = GetScreenHeight(hge);

    if (hge->Input_GetKeyState(HGEK_LEFT))  Velocity.x -= Speed*delta;
    if (hge->Input_GetKeyState(HGEK_RIGHT)) Velocity.x += Speed*delta;
    if (hge->Input_GetKeyState(HGEK_UP))    Velocity.y -= Speed*delta;
    if (hge->Input_GetKeyState(HGEK_DOWN))  Velocity.y += Speed*delta;

    Velocity.x *= Friction;
    Velocity.y *= Friction;

    Position.x += Velocity.x;
    Position.y += Velocity.y;

    //Если ушли вправо
    if (Position.x > sWidth - Size)
    {
        Position.x = 2 * (sWidth - Size) - Position.x;
        Velocity.x = - Velocity.x;
    }

    //Если ушли влево
    if (Position.x < Size)
    {
        Position.x = 2 * Size - Position.x;
        Velocity.x = - Velocity.x;
    }

    //Если ушли вниз
    if (Position.y > sHeight - Size)
    {
        Position.y = 2 * (sHeight - Size) - Position.y;
        Velocity.y = - Velocity.y;
    }

    //Если ушли вверх
    if (Position.y < Size)
    {
        Position.y = 2 * Size - Position.y;
        Velocity.y = - Velocity.y;
    }

    Quad.v[0].x = Position.x - Size; Quad.v[0].y = Position.y - Size;
    Quad.v[1].x = Position.x + Size; Quad.v[1].y = Position.y - Size;
    Quad.v[2].x = Position.x + Size; Quad.v[2].y = Position.y + Size;
    Quad.v[3].x = Position.x - Size; Quad.v[3].y = Position.y + Size;


    Render();
    //hge->Gfx_RenderQuad(&Quad);

    //Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
}