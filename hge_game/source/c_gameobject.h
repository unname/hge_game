#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"

#include "c_bool.h"
#include "c_drawobject.h"
#include "c_platform.h"
#include "c_game_values.h"

class c_gameobject: public c_drawobject
{
protected:
    hgeVector   Velocity;
    hgeVector   PreviousPosition;

    float   Speed;
    float   Max_Speed;

    float   Acceleration;
    float   Min_Acceleration;
    float   Max_Acceleration;
    float   Acceleration_step;

    float   Friction;

    float   JumpImpulse;
    float   Max_JumpImpulse;
    float   JumpImpulse_step;

    float    g;

    c_bool  OnTheGround;
    c_bool  OnTheLeftWall;
    c_bool  OnTheRightWall;
public:

    c_gameobject();

    hgeVector GetVelocity();
    virtual void Update(float delta);

    //¬озвращает новую позицию (центр) при пересечении с другим спрайтом
    //BoundingBox - границы пересечЄнного спрайта
    hgeVector GetNewPosition(hgeRect BoundingBox);
};


#endif