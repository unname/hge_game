#ifndef C_PLAYER_H
#define C_PLAYER_H
#pragma once

#define RESOURCES_PATH "resources/"
#define PLAYER_TEX_NAME "player.png"

#include "hge.h"

#include "c_gameobject.h"
#include "c_anim_manager.h"
#include "c_game_values.h"

class c_player: public c_gameobject
{
private:
    c_bool SpaceHoldDown;

    hgeAnimation* CurrentAnimation;
    hgeAnimation* Standing;
    hgeAnimation* Running;
    hgeAnimation* Braking;
    hgeAnimation* Jumping;
    hgeAnimation* Falling;
    hgeAnimation* Landing;

    c_anim_manager* PlayerAnimation;

public:
    c_player();
    ~c_player();

    void Update(float delta);
};

#endif