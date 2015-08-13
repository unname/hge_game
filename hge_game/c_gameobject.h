#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"

#include "c_bool.h"
#include "c_drawobject.h"
 
class c_gameobject: public c_drawobject
{
protected:
    hgeVector   Velocity;   // dx,dy

    float   Speed;
    float   Max_Speed;

    float   Acceleration;
    float   Min_Acceleration;
    float   Max_Acceleration;
    float   Acceleration_step;

    float Friction;

    float   JumpImpulse;
    float   Max_JumpImpulse;
    float   JumpImpulse_step;

    static float    g;

    size_t   Size;
    c_bool  OnTheGround;

    void SetVelocity(hgeVector velocity);
public:
    hgeVector GetVelocity();

    virtual void Update(float delta);
};


#endif