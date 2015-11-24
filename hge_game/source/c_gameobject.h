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

    //�������� ?
    c_bool isBraking;
    //������������ ����� ������� ?
    c_bool isLanding;
    //����� �� ��������� ����������� ?
    c_bool isGroundTilted;
    
    //��� ��� ����� � ������ �������� ������� �������, ������� ����� �����, �� �������� ����� ������� �����������
    //����� ���� �������� (��� ������ ������) ������ ������������� �� ������ �������
    //����� ����������� ���������� � ������� ����� �������� - 'Standing'
    hgeVector IntersectBoindingBoxSize;
    hgeRect GetIntersectBoundingBox();

    //���������� ����� ������� (�����) ��� ����������� � ������ ��������
    //BoundingBox1 - ������� ������ �������
    //BoundingBox2 - ������� ������������� �������

    //�������������� �������� (�������� � ��������� ������):
    //tilt_type    - ��� ������� (���� ����� �������� ��� ������ ��������� ������, �.�. ����� ������ � ���� �������, � �� ���� �� ����):
    //                  1/-1 - ������        (��������� ��������������� �������)
    //                  2/-2 - ������ ������ (��������� ������������� �������)
    //                  3/-3 - �����         (��������� ��������������� �������)
    //                  4/-4 - ������ �����  (��������� ������������� �������)
    //tilt_level   - ������� ������� (������������ ����������� ������ � ������)
    //tilt_number  - ����� ����� � ������

    //��� ��������� ����������� � ������������� ��������
    hgeVector GetNewPosition_Rect(hgeRect BoundingBox1, hgeRect BoundingBox2);
    //��� ��������� ����������� � ��������� ��������
    hgeVector GetNewPosition_Tilt(hgeRect BoundingBox1, hgeRect BoundingBox2, int tilt_type, int tilt_level, int tilt_number);
    //���������� ����� ������� �� ����������� �� �������
    hgeVector GetNewPositionOnTiltLine(hgeVector PointToMove);

    //��������� ����� �� �� �� ������ ���������
    // point    - Position
    // platform - ��������� �� ��������� �� ������� �����
    bool TestPoint_Tilt(hgeVector point, c_platform &platform);

public:
    c_gameobject();

    virtual void Update(float delta);
};


#endif