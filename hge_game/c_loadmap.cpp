#include "c_loadmap.h"

c_loadmap::c_loadmap()
{
    tinyxml2::XMLDocument TME_MAP;
    TME_MAP.LoadFile("resources/test_tmx.tmx");

    XMLElement *Map = NULL;
    Map = TME_MAP.FirstChildElement("map");

    // Ўирина и высота карты в тайлах
    size_t MapWidth, MapHeight;
    size_t TileWidth, TileHeight;

    MapWidth = atoi(Map->Attribute("width"));
    MapHeight = atoi(Map->Attribute("height"));

    TileWidth = atoi(Map->Attribute("tilewidth"));
    TileHeight = atoi(Map->Attribute("tileheight"));
}

c_loadmap::~c_loadmap()
{

}