#include "c_background.h"

c_background::c_background(string background_file)
{
    Texture = hge->Texture_Load(background_file.c_str());
    if (!Texture)
    {
        DisplayErrorHGE();
    }

    Quad.tex = Texture;

    Quad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

    for (int i = 0; i<4; i++)
    {
        // Set up z-coordinate of vertices
        Quad.v[i].z = 0.5f;
        // Set up color. The format of DWORD col is 0xAARRGGBB
        Quad.v[i].col = 0xFFFFFFFF;
    }
}

c_background::~c_background()
{
    if (Texture)
        hge->Texture_Free(Texture);
}

void c_background::Update(float delta)
{
    //Смещаем бэкграунд за игроком
    Quad.v[0].x = PlayerPosition.x - GetScreenWidth() / 2;
    Quad.v[0].y = PlayerPosition.y - GetScreenHeight() / 2;

    Quad.v[1].x = PlayerPosition.x + GetScreenWidth() / 2;
    Quad.v[1].y = PlayerPosition.y - GetScreenHeight() / 2;

    Quad.v[2].x = PlayerPosition.x + GetScreenWidth() / 2;
    Quad.v[2].y = PlayerPosition.y + GetScreenHeight() / 2;

    Quad.v[3].x = PlayerPosition.x - GetScreenWidth() / 2;
    Quad.v[3].y = PlayerPosition.y + GetScreenHeight() / 2;

    //Анимация бэкграунда - смещение текстуры
    t += delta;

    float tx = 50 * cosf(t / 60);
    float ty = 50 * sinf(t / 60);

    Quad.v[0].tx = tx;
    Quad.v[0].ty = ty;

    Quad.v[1].tx = tx + GetScreenWidth() / hge->Texture_GetWidth(Texture);
    Quad.v[1].ty = ty;

    Quad.v[2].tx = tx + GetScreenWidth() / hge->Texture_GetWidth(Texture);
    Quad.v[2].ty = ty + GetScreenHeight() / hge->Texture_GetHeight(Texture);

    Quad.v[3].tx = tx;    
    Quad.v[3].ty = ty + GetScreenHeight() / hge->Texture_GetHeight(Texture);

    Render();
}

void c_background::Render()
{
    hge->Gfx_RenderQuad(&Quad);
    //Sprite->Render(Position.x - PlayerPosition.x + GetScreenWidth() / 2, Position.y - PlayerPosition.y + GetScreenHeight() / 2);
}
