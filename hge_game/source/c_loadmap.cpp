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


c_loadmap::c_loadmap(string map_file)
{
    tilesetImageTex = NULL;

    LoadFromFile(map_file);
}

c_loadmap::~c_loadmap()
{
    if (tilesetImageTex)
        hge->Texture_Free(tilesetImageTex);

    for (size_t l_count = 0; l_count < layers.size(); l_count++)
        for (size_t t_count = 0; t_count < layers[l_count].tiles.size(); t_count++)
        {
            delete layers[l_count].tiles[t_count];
        }

    for (size_t do_count = 0; do_count < tiles.size(); do_count++)
        delete tiles[do_count];
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

hgeVector c_loadmap::GetMapSize()
{
    return hgeVector((float)tileWidth*mapWidth, (float)tileHeight*mapHeight);
}

bool c_loadmap::LoadFromFile(string filename)
{
    //Загрузка файла карты
    tinyxml2::XMLDocument TME_MAP;
    auto error_code = TME_MAP.LoadFile(filename.c_str());

    if (error_code)
    {
        string error_msg;
        error_msg.append("Loading level \"");
        error_msg.append(filename);
        error_msg.append("\" failed.");

        DisplayError(error_msg.c_str());
        return false;
    }


    //Получение парметров карты
    XMLElement* Map = NULL;
    Map = TME_MAP.FirstChildElement("map");

    mapWidth = atoi(Map->Attribute("width"));
    mapHeight = atoi(Map->Attribute("height"));

    tileWidth = atoi(Map->Attribute("tilewidth"));
    tileHeight = atoi(Map->Attribute("tileheight"));


    //Получение описания тайлсета и идентификатора первого тайла
    XMLElement* TileSet;
    TileSet = Map->FirstChildElement("tileset");

    firstTileID = atoi(TileSet->Attribute("firstgid"));


    //Получение параметров картинки c тайлсетом
    XMLElement* Image;
    Image = TileSet->FirstChildElement("image");

    string image_path;
    size_t columns, rows;

    image_path.append(RESOURCES_PATH);
    image_path.append(Image->Attribute("source"));

    tileSetWidth = atoi(Image->Attribute("width"));
    tileSetHeight = atoi(Image->Attribute("height"));


    //Вектор с прямоугольниками тайлсета. 
    columns = tileSetWidth / tileWidth;
    rows = tileSetHeight / tileHeight;

    vector<hgeRect> tileSetRects;

    for (int y = 0; y < rows; y++)
        for (int x = 0; x < columns; x++)
        {
            hgeRect rect;

            rect.x1 = x*tileWidth;
            rect.x2 = (x + 1)*tileWidth;
            rect.y1 = y*tileHeight;
            rect.y2 = (y + 1)*tileHeight;

            tileSetRects.push_back(rect);
        }

    //Загрузка картинки c тайлсетом
    tilesetImageTex = hge->Texture_Load(image_path.c_str());
   
    if (!tilesetImageTex)
    {
        DisplayErrorHGE();
        return false;
    }    

    // Работа со слоями
    XMLElement* Layers;
    Layers = Map->FirstChildElement("layer");

    while (Layers)
    {
        Layer layer;

        // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (Layers->Attribute("opacity") != NULL)
        {
            layer.opacity = 255 * strtod(Layers->Attribute("opacity"), NULL);
        }
        else
        {
            layer.opacity = 255;
        }


        // Контейнер <data>
        XMLElement* LayerData;
        LayerData = Layers->FirstChildElement("data");

        if (LayerData == NULL)
        {
            DisplayError("Bad map. No layer information found.");
            return false;
        }

        // Контейнер <tile> - описание тайлов каждого слоя
        XMLElement* Tiles;
        Tiles = LayerData->FirstChildElement("tile");

        if (Tiles == NULL)
        {
            DisplayError("Bad map. No tile information found.");
            return false;
        }

        
        size_t tileHotSpot_X = 0;
        size_t tileHotSpot_Y = 0;

        while (Tiles)
        {
            int tileGID = atoi(Tiles->Attribute("gid"));
            int tileSetRectToUse = tileGID - firstTileID;

            //Присваиваем каждому тайлу свой Sprite и координаты hgeVector
            if (tileSetRectToUse >= 0)
            {
                hgeSprite* sprite = new hgeSprite(0, 0, 0, 0, 0);
                sprite->SetTexture(tilesetImageTex);
                sprite->SetTextureRect(tileSetRects[tileSetRectToUse].x1 + 0.5, tileSetRects[tileSetRectToUse].y1 + 0.5, tileWidth, tileHeight);
                sprite->SetHotSpot(tileWidth/2, tileHeight/2);
                sprite->SetColor(ARGB(255, 255, 255, layer.opacity));

                layer.tiles.push_back(sprite);

                hgeVector coord;
                coord.x = tileHotSpot_X * tileWidth + tileWidth/2;
                coord.y = tileHotSpot_Y * tileHeight + tileHeight/2;

                layer.tiles_coord.push_back(coord);
            }

            Tiles = Tiles->NextSiblingElement("tile");

            //Сдвигаем координаты для следующего тайла
            tileHotSpot_X++;

            if (tileHotSpot_X >= mapWidth)
            {
                tileHotSpot_X = 0;
                tileHotSpot_Y++;

                if (tileHotSpot_Y >= mapHeight)
                    tileHotSpot_Y = 0;
            }
        }//Tiles

        layers.push_back(layer);

        Layers = Layers->NextSiblingElement("layer");

    }//Layers

    //Создадим на основе наших спрайтов рисующиеся объекты
    for (size_t layers_count = 0; layers_count < layers.size(); layers_count++)
    {
        for (size_t tiles_count = 0; tiles_count < layers[layers_count].tiles.size(); tiles_count++)
        {
            c_platform* tile = new c_platform(layers[layers_count].tiles[tiles_count], layers[layers_count].tiles_coord[tiles_count]);
            tiles.push_back(tile);
        }
    }

    return true;
}