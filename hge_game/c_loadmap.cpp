#include "c_loadmap.h"


int Object::GetPropertyInt(string name)
{
    return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
    return (float) strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string name)
{
    return properties[name];
}


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

Object* c_loadmap::GetObject(string name)
{
    for (size_t obj_num = 0; obj_num < objects.size(); obj_num++)
    {
        if (objects[obj_num]->name == name)
            return objects[obj_num];
    }

    return nullptr;
}

vector<Object*> c_loadmap::GetObjects(string name)
{
    vector<Object*> vec = { nullptr };

    for (size_t obj_num = 0; obj_num < objects.size(); obj_num++)
    {
        if (objects[obj_num]->name == name)
            vec.push_back(objects[obj_num]);
    }

    return vec;
}

hgeVector c_loadmap::GetTileSize()
{
    return hgeVector((float)tileWidth, (float)tileHeight);
}


bool c_loadmap::LoadFromFile(string filename)
{
    tinyxml2::XMLDocument TME_MAP;
    auto error_code = TME_MAP.LoadFile(filename.c_str());

    if (error_code)
    {
        string error_msg;
        error_msg.append("Loading level \"");
        error_msg.append(filename);
        error_msg.append("\" failed.");

        DisplayError(error_msg.c_str());
    }

    return true;
}