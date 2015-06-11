#include "hge.h"

HGE *hge = 0;
hgeQuad quad;

float x = 100.0f, y = 100.0f;
float dx = 0.0f, dy = 0.0f;

const float speed = 50;
const float friction = 0.98f;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Радиус фигуры
#define CUBE_SIZE 20


void SetWindowState(HGE *hge, hgeCallback framefunc)
{
    hge->System_SetState(HGE_FRAMEFUNC, framefunc);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
    hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
    hge->System_SetState(HGE_SCREENBPP, 32);
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_TITLE, "Main window");
    hge->System_SetState(HGE_FPS, 100);
}

bool FrameFunc()
{
    //Узнаем сколько прошло времени с последнего вызова функции кадра
    float dt = hge->Timer_GetDelta();

    if (hge->Input_GetKeyState(HGEK_ESCAPE))
        return true;

    if (hge->Input_GetKeyState(HGEK_LEFT))  dx -= speed*dt;
    if (hge->Input_GetKeyState(HGEK_RIGHT)) dx += speed*dt;
    if (hge->Input_GetKeyState(HGEK_UP))    dy -= speed*dt;
    if (hge->Input_GetKeyState(HGEK_DOWN))  dy += speed*dt;

    dx *= friction;
    dy *= friction; 
    x += dx;
    y += dy;

    //Если ушли вправо
    if (x > SCREEN_WIDTH-CUBE_SIZE)
    {
        x = 2*(SCREEN_WIDTH - CUBE_SIZE) - x;
        dx = -dx;
    }

    //Если ушли влево
    if (x < CUBE_SIZE)
    {
        x = 2*CUBE_SIZE - x;
        dx = -dx; 
    }

    //Если ушли вниз
    if (y > SCREEN_HEIGHT-CUBE_SIZE)
    { 
        y = 2*(SCREEN_HEIGHT - CUBE_SIZE) - y;
        dy = -dy; 
    }

    //Если ушли вверх
    if (y < CUBE_SIZE)
    { 
        y = 2*CUBE_SIZE - y;
        dy = -dy;
    }

    quad.v[0].x = x - CUBE_SIZE; quad.v[0].y = y - CUBE_SIZE;
    quad.v[1].x = x + CUBE_SIZE; quad.v[1].y = y - CUBE_SIZE;
    quad.v[2].x = x + CUBE_SIZE; quad.v[2].y = y + CUBE_SIZE;
    quad.v[3].x = x - CUBE_SIZE; quad.v[3].y = y + CUBE_SIZE;

    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);

    //Рендерим всё тут
    hge->Gfx_RenderQuad(&quad);

    hge->Gfx_EndScene();

    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    hge = hgeCreate(HGE_VERSION);
    SetWindowState(hge, FrameFunc);

    //Загрузка текстур
    quad.tex = hge->Texture_Load("url.jpeg");

    //Настройка текстур
    quad.blend = BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;

    quad.v[0].tx = 96.0 / 128.0;   quad.v[0].ty = 64.0 / 128.0;
    quad.v[1].tx = 128.0 / 128.0;  quad.v[1].ty = 64.0 / 128.0;
    quad.v[2].tx = 128.0 / 128.0;  quad.v[2].ty = 96.0 / 128.0;
    quad.v[3].tx = 96.0 / 128.0;   quad.v[3].ty = 96.0 / 128.0;

    for (int i = 0; i<4; i++)
    {
        quad.v[i].z = 0.5f;
        quad.v[i].col = 0xFFFFA000;
    }

    //Запуск игры
    if (hge->System_Initiate())
    {
        hge->System_Start();
        
        //Удаляем загруженные объекты
        hge->Texture_Free(quad.tex);
    }
    else
    {
        return 0;
    }

    //Завершаем работу
    hge->System_Shutdown();
    hge->Release();

    return 0;
}