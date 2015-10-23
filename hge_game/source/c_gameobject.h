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

    size_t   Size_x;
    size_t   Size_y;
    c_bool  OnTheGround;
public:
    Moving moving;

    c_gameobject();

    hgeVector GetVelocity();
    virtual void Update(float delta);

    //A и B - это PreviousPosition и CurrentPosiion соответственно
    //C - это крайняя точка (угол) другого объекта, которая расположена в наешм объекте после пересечения
    //C - номер угла (нумерация с верхнего левого по часовой 1,2,3,4)
    hgeVector GetNewPosition(hgeVector A, hgeVector B, hgeVector C, size_t C_number);
};


#endif