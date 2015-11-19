#ifndef C_LOADMAP_H
#define C_LOADMAP_H
#pragma once

#define RESOURCES_PATH "resources/"
#define MAP_NAME "test_tmx.tmx"

#include "tinyxml2.h"

#include "hge.h"
#include "hgevector.h"
#include "hgesprite.h"
#include "hgerect.h"

#include "c_hge.h"
#include "c_platform.h"

#include <map>
#include <vector>
#include <string>

using namespace tinyxml2;
using namespace std;

struct Tile
{
    //int GetPropertyInt(string name);
    //float GetPropertyFloat(string name);
    //string GetPropertyString(string name);

    //string name;
    //string type;
    //hgeRect rect;

    map <string, int> properties;

    hgeVector       tile_coord;
    hgeSprite*      tile_sprite_ptr = nullptr;
    c_drawobject*   tile_drawobject_ptr = nullptr;
};

struct Layer
{
    int opacity;
    vector<Tile>  tiles;
};


class c_loadmap: public c_hge
{
public:
    c_loadmap(string map_file);
    ~c_loadmap();

    //��������� ����� �� ���������� �����
    bool LoadFromFile(string filename);

    //������� ���������� �������
    void Release();

    //���������� ��������� �� ������ ������ � ��������� ������
    Tile* GetObject(string name);

    //���������� ������ ���������� �� ������� � ��������� ������
    vector<Tile*> GetObjects(string name);

    //�������� ������ �����
    hgeVector GetMapSize();

    //�������� ������ ������
    hgeVector GetTileSize();

private:
    size_t          mapWidth, mapHeight,
                    tileWidth, tileHeight,
                    tileSetWidth, tileSetHeight;
    size_t          firstTileID;

    hgeRect         drawingBounds;
    HTEXTURE        tilesetImageTex;

    vector<Layer>   layers;
};

#endif