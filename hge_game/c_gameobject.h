#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"
#include "c_bool.h"
 
class c_gameobject
{
protected:
    static HGE*     hge;

    hgeVector       Position;   // x,y
    hgeVector       Velocity;   // dx,dy

    float           Speed;
    float           Max_Speed;

    float Acceleration;
    float Max_Acceleration;
    float Acceleration_step;

    float Friction;
    float JumpImpulse;

    c_bool OnTheGround;

    static float    g;

    u_int           Size;

public:

    virtual void Update(float delta);
    virtual void Render();

    void SetVelocity(hgeVector velocity);
    void SetPosition(hgeVector position);

    hgeVector GetPosition();
    hgeVector GetVelocity();

    int GetScreenWidth(HGE* hge);
    int GetScreenHeight(HGE* hge);
};


#endif