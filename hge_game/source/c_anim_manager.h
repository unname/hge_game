#ifndef C_ANIM_MANAGER_H
#define C_ANIM_MANAGER_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "hgeanim.h"

#include "c_hge.h"
#include "c_bool.h"
#include "c_gameobject.h"

#include <map>
#include <string>
#include <vector>

class c_anim_manager : public c_hge
{
    hgeSprite*      AnimatedSprite;
    hgeAnimation*   CurrentAnimation;

    std::map<std::string, hgeAnimation*> Animations;

  public:
      c_anim_manager(hgeSprite* sprite);

      void AddElement(hgeAnimation* anim, std::string anim_name, int mode = HGEANIM_FWD | HGEANIM_LOOP, hgeVector hotspot = hgeVector(-1, -1));
      void RemoveElement(std::string anim_name);
      void GetAnimationList(std::vector<std::string>* anim_list);

      void Animate(std::string anim_name, float delta, moving* moving = nullptr);
};



#endif