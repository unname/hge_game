#ifndef C_LOADMAP_H
#define C_LOADMAP_H
#pragma once

#include "tinyxml2.h"

#include "hgesprite.h"
#include "hgerect.h"

#include <vector>
#include <string>

using namespace tinyxml2;
using namespace std;


struct Object
{
    int GetPropertyInt(string name);
    float GetPropertyFloat(string name);
    string GetPropertyString(string name);

    string name;
    string type;
    hgeRect rect;
    vector<pair<string, string>> properties;

    hgeSprite* sprite;
};

struct Layer
{
    int opacity;
    vector<hgeSprite*> tiles;
};


class c_loadmap
{
public:
    c_loadmap();
    ~c_loadmap();

    //Загружает карту из указанного файла
    bool LoadFromFile(string filename);

    //Возвращает первый объект с указанным именем
    Object GetObject(string name);
    
    //Возвращает список объектов с указанным именем
    vector<Object> GetObjects(string name);

    //Рисует все тайлы
    void Draw();

    //sf::Vector2i GetTileSize();

private:
    size_t          width, height,
                    tileWidth, tileHeight;
    size_t          firstTileID;

    hgeRect         drawingBounds;
    HTEXTURE        tilesetImage;

    vector<Object>  objects;
    vector<Layer>   layers;
};

#endif