#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "c_bool.h"
#include "c_hge.h"
 
class c_gameobject: public c_hge
{
protected:
    hgeVector   Position;   // x,y
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

    u_int   Size;
    c_bool  OnTheGround;

public:
    virtual void Update(float delta);
    virtual void Render();

    void SetVelocity(hgeVector velocity);
    void SetPosition(hgeVector position);

    hgeVector GetPosition();
    hgeVector GetVelocity();

    int GetScreenWidth();
    int GetScreenHeight();
};


#endif