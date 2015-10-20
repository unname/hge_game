#ifndef C_GAMEOBJECT_H
#define C_GAMEOBJECT_H
#pragma once

#include "hge.h"
#include "hgevector.h"

#include "c_bool.h"
#include "c_drawobject.h"
 
struct Moving
{
    c_bool MovingRigth;
    c_bool MovingLeft;
    c_bool MovingUp;
    c_bool MovingDown;
};

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

    float Friction;

    float   JumpImpulse;
    float   Max_JumpImpulse;
    float   JumpImpulse_step;

    static float    g;

    size_t   Size;
    c_bool  OnTheGround;
public:
    Moving moving;

    c_gameobject();

    hgeVector GetVelocity();
    virtual void Update(float delta);

    //A и B это PreviousPosition и CurrentPosiion необходимые дл€ вычислени€ уравнени€ пр€мой
    //C это крайн€€ точка (угол) другого объекта, котора€ расположена в наешм объекте после пересечени€
    hgeVector GetNewPosition(hgeVector A, hgeVector B, hgeVector C);
};


#endif