#include "Scene.h"
#include "NoaEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace noa 
{
	MapFile LoadMap(const char* fileName)
	{

		MapFile map;

		// 打开二进制文件
		std::ifstream file(fileName, std::ios::binary);
		if (!file)
		{
			std::cout << "打开文件失败" << std::endl;
			return map;
		}

		// 读取地图宽度和高度
		file.read(reinterpret_cast<char*>(&map.w), sizeof(int));
		file.read(reinterpret_cast<char*>(&map.h), sizeof(int));

		// 计算图像的总像素数
		int imageSize = map.w * map.h;

		// 读取图像灰度值
		map.image.resize(imageSize);
		file.read(reinterpret_cast<char*>(&map.image[0]), imageSize * sizeof(uint32_t));

		// 关闭文件
		file.close();

		return map;

	}

	MapFile LoadMapFromCSV(const std::string filename)
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

	LevelMap::LevelMap() {

	}

	LevelMap::LevelMap(const MapFile & map)
	{
		this->w = map.w;
		this->h = map.h;

		level = map.image;

		Debug("load map from file successfully");

	}

	TileMap::TileMap(unordered_map<Uint8, Tile> tileSet, MapFile map) :LevelMap(map)
	{
		this->tileSet = tileSet;
	}

}