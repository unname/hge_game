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
    hgeVector   PreviousVelocity;
    hgeVector   PreviousPosition;

    moving  Moving;

    float   Speed;
    float   Max_Speed;

    float   Acceleration;
    float   Min_Acceleration;
    float   Max_Acceleration;
    float   Acceleration_step;

    float   GroundFriction;
    float   AirFriction;

    float   JumpImpulse;
    float   Max_JumpImpulse;
    float   JumpImpulse_step;

    float   SpeedForBraking;

    float    g;

    c_bool  OnTheGround;
    c_bool  OnTheLeftWall;
    c_bool  OnTheRightWall;

    //“ормозим ?
    c_bool isBraking;
    //ѕриземлились после падени€ ? (сейчас никак не обрабатываетс€, может не нужен ?)
    c_bool isLanding;

    //“ак как кадры у разных анимаций разного размера, выделим общий кусок, по которому будем считать пересечени€
    //“огда кра€ анимаций (дл€ болших кадров) смогут накладыватьс€ на другие объекты, с которыми рассчитываетс€ пересечение...
    //—амое оптимальное приравн€ть к размеру кадра анимации - 'Standing'
    hgeRect IntersectBoindingBox;
    hgeRect GetIntersectBoundingBox();

    //¬озвращает новую позицию (центр) при пересечении с другим спрайтом
    //BoundingBox1 - границы нашего спрайта
    //BoundingBox2 - границы пересечЄнного спрайта
    hgeVector GetNewPosition(hgeRect BoundingBox1, hgeRect BoundingBox2);

public:
    c_gameobject();

    virtual void Update(float delta);
};


#endif