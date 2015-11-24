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

    hgeVector TiltPoint1;
    hgeVector TiltPoint2;

    c_bool  OnTheGround;
    c_bool  OnTheLeftWall;
    c_bool  OnTheRightWall;

    //Тормозим ?
    c_bool isBraking;
    //Приземлились после падения ?
    c_bool isLanding;
    //Стоим на наклонной поверхности ?
    c_bool isGroundTilted;
    
    //Так как кадры у разных анимаций разного размера, выделим общий кусок, по которому будем считать пересечения
    //Тогда края анимаций (для болших кадров) смогут накладываться на другие объекты
    //Самое оптимальное приравнять к размеру кадра анимации - 'Standing'
    hgeVector IntersectBoindingBoxSize;
    hgeRect GetIntersectBoundingBox();

    //Возвращает новую позицию (центр) при пересечении с другим спрайтом
    //BoundingBox1 - границы нашего спрайта
    //BoundingBox2 - границы пересечённого спрайта

    //Дополнительыне значения (хранятся в свойствах тайлов):
    //tilt_type    - тип наклона (знак минус означает что наклон ограничен сверху, т.е. будем биться о него головой, а не идти по нему):
    //                  1/-1 - подъём        (ограничен горизонтальынми тайлами)
    //                  2/-2 - крутой подъём (ограничен вертикальными тайлами)
    //                  3/-3 - спуск         (ограничен горизонтальынми тайлами)
    //                  4/-4 - крутой спуск  (ограничен вертикальными тайлами)
    //tilt_level   - уровень наклона (определяется количеством тайлов в прямой)
    //tilt_number  - номер тайла в прямой

    //Для обработки пересечения с прямоугольным спрайтом
    hgeVector GetNewPosition_Rect(hgeRect BoundingBox1, hgeRect BoundingBox2);
    //Для обработки пересечения с наклонным спрайтом
    hgeVector GetNewPosition_Tilt(hgeRect BoundingBox1, hgeRect BoundingBox2, int tilt_type, int tilt_level, int tilt_number);
    //Возвращает новую позицию пи пересечении по наклону
    hgeVector GetNewPositionOnTiltLine(hgeVector PointToMove);

    //Проверяет стоим ли мы на склоне платформы
    // point    - Position
    // platform - указатель на платформу на которой стоим
    bool TestPoint_Tilt(hgeVector point, c_platform &platform);

public:
    c_gameobject();

    virtual void Update(float delta);
};


#endif