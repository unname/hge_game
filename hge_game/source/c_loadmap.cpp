#include "c_loadmap.h"


//int Object::GetPropertyInt(string name)
//{
//    return atoi(properties[name].c_str());
//}
//
//float Object::GetPropertyFloat(std::string name)
//{
//    return (float) strtod(properties[name].c_str(), NULL);
//}
//
//std::string Object::GetPropertyString(std::string name)
//{
//    return properties[name];
//}


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
            delete layers[l_count].tiles[t_count].tile_drawobject_ptr;
            delete layers[l_count].tiles[t_count].tile_sprite_ptr;
        }
}

//Object* c_loadmap::GetObject(string name)
//{
//    for (size_t obj_num = 0; obj_num < objects.size(); obj_num++)
//    {
//        if (objects[obj_num]->name == name)
//            return objects[obj_num];
//    }
//
//    return nullptr;
//}

//vector<Object*> c_loadmap::GetObjects(string name)
//{
//    vector<Object*> vec = { nullptr };
//
//    for (size_t obj_num = 0; obj_num < objects.size(); obj_num++)
//    {
//        if (objects[obj_num]->name == name)
//            vec.push_back(objects[obj_num]);
//    }
//
//    return vec;
//}

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
    XMLElement* MapTileSet;
    MapTileSet = Map->FirstChildElement("tileset");

    size_t firstTileID = atoi(MapTileSet->Attribute("firstgid"));
    tileset.firstgid = firstTileID;

    size_t tileCount = atoi(MapTileSet->Attribute("tilecount"));
    tileset.tilecount = tileCount;

    //Получение параметров картинки c тайлсетом
    XMLElement* Image;
    Image = MapTileSet->FirstChildElement("image");

    string image_path;
    size_t columns, rows;

    image_path.append(RESOURCES_PATH);
    image_path.append(Image->Attribute("source"));

    size_t tileSetWidth = atoi(Image->Attribute("width"));
    size_t tileSetHeight = atoi(Image->Attribute("height"));

    //Вектор с прямоугольниками тайлсета. 
    columns = tileSetWidth / tileWidth;
    rows = tileSetHeight / tileHeight;

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

    //Получение свойств тайлов в тайлсете
    XMLElement* TileSetTile;
    TileSetTile = MapTileSet->FirstChildElement("tile");

    while (TileSetTile)
    {
        XMLElement* Properties;

        size_t TileID = atoi(TileSetTile->Attribute("id"));

        Properties = TileSetTile->FirstChildElement("properties");
        if (Properties)
        {
            XMLElement *prop;
            prop = Properties->FirstChildElement("property");
            if (prop)
            {
                map <string, string> propeties;

                while (prop)
                {
                    string propertyName = prop->Attribute("name");
                    string propertyValue = prop->Attribute("value");

                    propeties.insert(pair<string, string>(propertyName, propertyValue));

                    prop = prop->NextSiblingElement("property");
                }

                tileset.properties.insert(pair<size_t, map<string, string>>(TileID, propeties));
            }
        }

        TileSetTile = TileSetTile->NextSiblingElement("tile");
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
            size_t tileGID = atoi(Tiles->Attribute("gid"));
            int tileSetRectToUse = tileGID - firstTileID;

            //Если тайл существует (не пустой) то присваиваем Sprite и координаты hgeVector
            if (tileSetRectToUse >= 0)
            {
                Tile tile;
                tile.id = tileSetRectToUse;

                hgeSprite* sprite = new hgeSprite(0, 0, 0, 0, 0);
                sprite->SetTexture(tilesetImageTex);
                sprite->SetTextureRect(tileSetRects[tileSetRectToUse].x1 + 0.5, tileSetRects[tileSetRectToUse].y1 + 0.5, tileWidth, tileHeight);
                sprite->SetHotSpot(tileWidth / 2, tileHeight / 2);
                sprite->SetColor(ARGB(255, 255, 255, layer.opacity));

                tile.tile_sprite_ptr = sprite;

                hgeVector coord;
                coord.x = tileHotSpot_X * tileWidth + tileWidth / 2;
                coord.y = tileHotSpot_Y * tileHeight + tileHeight / 2;

                tile.tile_coord = coord;

                //Добавляем объект тайла к текущему слою
                layer.tiles.push_back(tile);
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
            if (layers[layers_count].tiles[tiles_count].tile_sprite_ptr)
            {
                c_platform* tile;

                size_t current_tile_id = layers[layers_count].tiles[tiles_count].id;

                //Если номера тайла нет в тайлсете, то он без свойств -> сейчас это обычная платформа
                auto it = tileset.properties.find(current_tile_id);
                if (it != tileset.properties.end())
                {
                    //Проверяем есть ли наклон у платформы
                    auto it_p = it->second.find("tilt_type");
                    if (it_p == it->second.end())
                    {
                        //Обычная платформа
                        tile = new c_platform(layers[layers_count].tiles[tiles_count].tile_sprite_ptr, layers[layers_count].tiles[tiles_count].tile_coord);
                    }
                    else
                    {
                        //Наклонная платформа
                        int tilt_type = atoi(it_p->second.c_str());
                        int tilt_level;
                        int tilt_number;

                        it_p = it->second.find("tilt_level");
                        if (it_p == it->second.end())
                        {
                            std::string err_string;
                            err_string += "Tilted tile property not found: ";
                            err_string += "\"tilt_level\"";

                            DisplayError(err_string.c_str());
                        }
                        else
                            tilt_level = atoi(it_p->second.c_str());

                        it_p = it->second.find("tilt_number");
                        if (it_p == it->second.end())
                        {
                            std::string err_string;
                            err_string += "Tilted tile property not found: ";
                            err_string += "\"tilt_number\"";

                            DisplayError(err_string.c_str());
                        }
                        else
                            tilt_number = atoi(it_p->second.c_str());

                        tile = new c_platform(layers[layers_count].tiles[tiles_count].tile_sprite_ptr, layers[layers_count].tiles[tiles_count].tile_coord, tilt_type, tilt_level, tilt_number);
                    }
                }
                else
                    //Обычная платформа
                    tile = new c_platform(layers[layers_count].tiles[tiles_count].tile_sprite_ptr, layers[layers_count].tiles[tiles_count].tile_coord);

                layers[layers_count].tiles[tiles_count].tile_drawobject_ptr = tile;
            }
        }
    }

    return true;
}