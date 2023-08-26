#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <rapidxml/rapidxml.hpp>


struct Tile {
    int id;
    // You can add more properties here
};

struct TileLayer {
    std::vector<int> tileIds;
    // You can add more properties here
};

struct TiledMap {
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<TileLayer> layers;
    // You can add more properties here
};

void parseTMX(const std::string& filename, TiledMap& map) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open TMX file: " << filename << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    rapidxml::xml_document<> doc;
    doc.parse<0>(&content[0]);

    rapidxml::xml_node<>* mapNode = doc.first_node("map");
    if (mapNode) {
        map.width = std::atoi(mapNode->first_attribute("width")->value());
        map.height = std::atoi(mapNode->first_attribute("height")->value());
        map.tileWidth = std::atoi(mapNode->first_attribute("tilewidth")->value());
        map.tileHeight = std::atoi(mapNode->first_attribute("tileheight")->value());

        for (rapidxml::xml_node<>* layerNode = mapNode->first_node("layer"); layerNode; layerNode = layerNode->next_sibling("layer")) {
            TileLayer layer;
            rapidxml::xml_node<>* dataNode = layerNode->first_node("data");
            if (dataNode) {
                std::string data = dataNode->value();
                // Parse the data and extract tileIds
                // Modify the following logic to extract tileIds from the data
                // Example: comma-separated list of tile GIDs
                size_t pos = 0;
                while ((pos = data.find(',')) != std::string::npos) {
                    layer.tileIds.push_back(std::stoi(data.substr(0, pos)));
                    data.erase(0, pos + 1);
                }
                if (!data.empty()) {
                    layer.tileIds.push_back(std::stoi(data));
                }
                map.layers.push_back(layer);
            }
        }
    }
}

int main() {
    std::string tmxFile = "level1-backup.tmx";
    TiledMap tiledMap;
    parseTMX(tmxFile, tiledMap);

    // Print some basic map information
    std::cout << "Map width: " << tiledMap.width << std::endl;
    std::cout << "Map height: " << tiledMap.height << std::endl;

    for (const auto& layer : tiledMap.layers) {
        std::cout << "Layer with " << layer.tileIds.size() << " tiles" << std::endl;
    }

    return 0;
}