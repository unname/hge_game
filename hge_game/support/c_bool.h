#ifndef C_BOOL_H
#define C_BOOL_H
#pragma once

class c_bool
{
public:
    c_bool();
    bool GetState();
    void SetTrue();
    void SetFalse();

private:
    bool state;
};

#endif