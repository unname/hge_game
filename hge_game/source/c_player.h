#ifndef C_PLAYER_H
#define C_PLAYER_H
#pragma once

#include "hge.h"

#include "c_gameobject.h"
#include "c_game_values.h"

class c_player: public c_gameobject
{
private:
    c_bool SpaceHoldDown;

    hgeAnimation* Standing;
    hgeAnimation* Running;
    hgeAnimation* Braking;

    void StandingAnim(float dt);
    void RunningAnim(float dt);
    void BrakingAnim(float dt);

public:
    c_player(u_int size);
    ~c_player();

    void Update(float delta);
};

#endif