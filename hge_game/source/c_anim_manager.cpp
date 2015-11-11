#include "c_anim_manager.h"

void c_anim_manager::AddElement(hgeAnimation* anim, std::string anim_name, int mode, hgeVector hotspot)
{
    if (hotspot == hgeVector(-1, -1))
    {
        hotspot = hgeVector(anim->GetWidth() / 2, anim->GetHeight() / 2);
    }

    //Если анимаций нет, то добавленная становится текущей
    if (Animations.empty())
    {
        CurrentAnimation = anim;
    }

    //Добавляем анимацию в список
    Animations.insert(std::pair<std::string, hgeAnimation*>(anim_name, anim));

    //Устанавливаем режим
    anim->SetMode(mode);

    //Устанавливаем центр спрайта
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

void c_anim_manager::Animate(std::string anim_name, float delta, hgeSprite*& sprite, moving* moving)
{
    //Поиск нужной анимации
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
    //      Обработка анимаций
    //
    //-------------------------------

    if (CurrentAnimation != it->second)
        CurrentAnimation->Stop();

    //Разворачиваем спрайт, если надо
    //(Предполагается что структуры 'moving' означает необходимость отображения)
    if (moving)
    {
        if (moving->MovingLeft.GetState())
            it->second->SetFlip(true, false);
        else
            it->second->SetFlip(false, false);
    }

    //Начинаем или продожаем анимацию
    if (it->second->IsPlaying())
        it->second->Update(delta);
    else
        it->second->Play();

    CurrentAnimation = it->second;
    sprite = it->second;
}

void c_anim_manager::GetAnimationList(std::vector<std::string>* anim_list)
{
    anim_list->clear();
   
    for (auto it = Animations.begin(); it != Animations.end(); ++it)
    {
        anim_list->push_back(it->first);
    }
}