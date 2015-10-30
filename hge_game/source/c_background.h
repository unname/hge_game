#ifndef C_BACKGROUND_H
#define C_BACKGROUND_H
#pragma once

#define RESOURCES_PATH "resources/"
#define BACKGROUND_NAME "bg.png"

#include "hge.h"

#include "c_drawobject.h"
#include "c_game_values.h"
#include <string>

using namespace std;

class c_background : public c_drawobject
{
    float t;

public:
    c_background(string background_file);
    ~c_background();

    void Update(float delta);
    void Render();
};

#endif