#include "Scene.h"

namespace noa 
{
	MapFile LoadMap(const char* fileName)
	{
		MapFile map;

		std::ifstream file(fileName, std::ios::binary);
		if (!file)
		{
			std::cout << "无法打开文件" << std::endl;
			//map.image = nullptr;
			return map;
		}

		file.read(reinterpret_cast<char*>(&map.w), sizeof(int));
		file.read(reinterpret_cast<char*>(&map.h), sizeof(int));

		int imageDataSize = map.w * map.h * sizeof(uint8_t);
		//map.image = new uint8_t[imageDataSize];
		map.image.resize(imageDataSize);
		file.read(reinterpret_cast<char*>(map.image.data()), imageDataSize);

		file.close();

		return map;
	}

	LevelMap::LevelMap() {

	}

	LevelMap::LevelMap(MapFile map)
	{
		this->w = map.w;
		this->h = map.h;

		level = map.image;

	}

	TileMap::TileMap(unordered_map<Uint8, Tile> tileSet, MapFile map) :LevelMap(map)
	{
		this->tileSet = tileSet;
	}

}