#include "c_drawobject.h"

std::vector<c_drawobject*> c_drawobject::DrawObjects;
hgeVector c_drawobject::PlayerPosition;

c_drawobject::c_drawobject()
{
    Texture = NULL;
    Sprite = nullptr;

    DrawObjects.push_back(this);
}

c_drawobject::c_drawobject(hgeSprite* sprite, hgeVector sprite_coord)
{
    Texture = NULL;

    Sprite = sprite;
    Position = sprite_coord;

    DrawObjects.push_back(this);
}

c_drawobject::~c_drawobject()
{
    for (size_t obj_num = 0; obj_num < DrawObjects.size(); obj_num++)
    {
        if (DrawObjects[obj_num] == this)
        {
            DrawObjects.erase(DrawObjects.begin() + obj_num);
            break;
        }
    }
}

hgeRect c_drawobject::GetBoundingBox()
{
    hgeRect BoundingBox;

    Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);

    return BoundingBox;
};

void c_drawobject::Update(float delta)
{
    Render();
}

void c_drawobject::Render()
{
    if (Sprite)
    {
        //--------------------------------------------------------------
        //
        //      �������� ������ ��, ��� ������ �� �����
        //
        //--------------------------------------------------------------

        //������� ��������� ������� ������� �� ��������� � '������' (��������� � ����������� �� ������ ������)
        if ((PlayerPosition.x - GetBoundingBox().x2 < GetScreenWidth() / 2) ||  //����� ������ ���� �������
            (GetBoundingBox().x1 - PlayerPosition.x < GetScreenWidth() / 2) ||  //����� ����� ���� �������
            (PlayerPosition.y - GetBoundingBox().y2 < GetScreenHeight() / 2) || //����� ������ ���� �������
            (GetBoundingBox().y1 - PlayerPosition.y < GetScreenHeight() / 2))   //����� ������� ���� �������
        {
            //������ ��������� ������ ��, ��� ���������� ��� �����
            //(�������������� ��� Intersect ������� ��������, ������� ��������� ��� ���������)
            if (GetBoundingBox().Intersect(&hgeRect(PlayerPosition.x - GetScreenWidth() / 2, PlayerPosition.y - GetScreenHeight() / 2, PlayerPosition.x + GetScreenWidth() / 2, PlayerPosition.y + GetScreenHeight() / 2)))
            {
                Sprite->Render(Position.x - PlayerPosition.x + GetScreenWidth() / 2, Position.y - PlayerPosition.y + GetScreenHeight() / 2);
            }
        }
    }
}

