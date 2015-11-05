#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"

#include "c_bool.h"
#include "c_drawobject.h"
#include "c_platform.h"
#include "c_game_values.h"

struct moving
{
    c_bool MovingLeft;
    c_bool MovingRigth;
    c_bool MovingDown;
    c_bool MovingUp;
    c_bool NotMoving;
};

class c_gameobject: public c_drawobject
{
protected:
    hgeVector   Velocity;
    hgeVector   PreviousPosition;

    moving  Moving;

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

    //¬озвращает новую позицию (центр) при пересечении с другим спрайтом
    //BoundingBox - границы пересечЄнного спрайта
    hgeVector GetNewPosition(hgeRect BoundingBox);

public:
    c_gameobject();

    virtual void Update(float delta);
};


#endif