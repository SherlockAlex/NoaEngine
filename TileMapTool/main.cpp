#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

typedef struct MapFile
{
    int w;
    int h;
    std::vector<uint32_t> image;
} Map;

Map readCSV(const std::string& filename)
{
    Map map;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl;
        return map;
    }

    std::string line;
    int row = 0;
    int col = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
        {
            uint32_t value = std::stoi(cell);
            map.image.push_back(value);
            col++;
        }

        if (row == 0)
        {
            map.w = col;
        }
        else if (col != map.w)
        {
            std::cout << "Invalid row width at line: " << row << std::endl;
            map.image.clear();
            break;
        }

        col = 0;
        row++;
    }

    map.h = row;

    file.close();

    return map;
}

int main()
{
    Map map = readCSV("./111.csv");

    std::cout << "Map size: " << map.w << "x" << map.h << std::endl;
    std::cout << "Map image: " << std::endl;
    for (int i = 0;i<map.image.size();i++) 
    {
        std::cout << map.image[i] << " ";
        if ((i+1)%map.w==0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    return 0;
}