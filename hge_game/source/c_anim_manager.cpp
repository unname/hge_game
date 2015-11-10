#include "c_anim_manager.h"

c_anim_manager::c_anim_manager(hgeSprite* sprite)
{
    AnimatedSprite = sprite;
}

void c_anim_manager::AddElement(hgeAnimation* anim, std::string anim_name, int mode, hgeVector hotspot)
{
    if (hotspot == hgeVector(-1, -1))
    {
        hotspot = hgeVector(anim->GetWidth() / 2, anim->GetHeight() / 2);
    }

    //���� �������� ���, �� ����������� ���������� �������
    if (Animations.empty())
    {
        CurrentAnimation = anim;
    }

    //��������� �������� � ������
    Animations.insert(std::pair<std::string, hgeAnimation*>(anim_name, anim));

    //������������� �����
    anim->SetMode(mode);

    //������������� ����� �������
    anim->SetHotSpot(hotspot.x, hotspot.y);
}

void c_anim_manager::RemoveElement(std::string anim_name)
{
    auto it = Animations.find(anim_name);

    if (it != Animations.end())
    {
        Animations.erase(it);
    }
    else
    {
        std::string err_string;
        err_string += "Animation not found to delete: ";
        err_string += anim_name;

        DisplayError(err_string.c_str());
    }
}

void c_anim_manager::Animate(std::string anim_name, float delta, moving* moving)
{
    //����� ������ ��������
    auto it = Animations.find(anim_name);

    if (it == Animations.end())
    {
        std::string err_string;
        err_string += "Animation not found to play: ";
        err_string += anim_name;

        DisplayError(err_string.c_str());
    }
    
    //-------------------------------
    //
    //      ��������� ��������
    //
    //-------------------------------

    if (CurrentAnimation != it->second)
        CurrentAnimation->Stop();

    //������������� ������, ���� ����
    //(�������������� ��� ��������� 'moving' �������� ������������� �����������)
    if (moving)
    {
        if (moving->MovingLeft.GetState())
            it->second->SetFlip(true, false);
        else
            it->second->SetFlip(false, false);
    }

    //�������� ��� ��������� ��������
    if (it->second->IsPlaying())
        it->second->Update(delta);
    else
        it->second->Play();

    CurrentAnimation = it->second;
    AnimatedSprite = it->second;
}

void c_anim_manager::GetAnimationList(std::vector<std::string>* anim_list)
{
    anim_list->clear();
   
    for (auto it = Animations.begin(); it != Animations.end(); ++it)
    {
        anim_list->push_back(it->first);
    }
}