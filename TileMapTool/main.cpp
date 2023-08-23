#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
struct Tile {
    int gid; // Global ID of the tile
    // Add more attributes as needed
};

struct Map {
    int width; // Width of the map in tiles
    int height; // Height of the map in tiles
    int tileWidth; // Width of each tile in pixels
    int tileHeight; // Height of each tile in pixels
    std::vector<Tile> tiles; // 1D vector to store tile data
};