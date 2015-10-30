#include "c_background.h"

c_background::c_background(string background_file)
{
    Texture = hge->Texture_Load(background_file.c_str());
    if (!Texture)
    {
        DisplayErrorHGE();
    }

    Sprite = new hgeSprite(Texture, 0 + 0.5f, 0 + 0.5f, GetScreenWidth(), GetScreenHeight());
    Sprite->SetHotSpot(GetScreenWidth() / 2, GetScreenHeight() / 2);
    Sprite->SetBlendMode(BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE);
    Sprite->SetZ(0.5f);
    Sprite->SetColor(0xFFFFFFFF);
}

c_background::~c_background()
{
    delete Sprite;
}

void c_background::Update(float delta)
{
    //Анимация бэкграунда - смещение текстуры
    t += delta;

    float tx = 50 * cosf(t / 60);
    float ty = 50 * sinf(t / 60);

    //Sprite->SetTextureRect(tx, ty, GetScreenWidth() / hge->Texture_GetWidth(Texture), GetScreenHeight() / hge->Texture_GetHeight(Texture));

    Render();
}

void c_background::Render()
{
    if (Sprite)
        Sprite->Render(GetScreenWidth() / 2, GetScreenHeight()/2);
}

