#ifndef NOAENGINE_SCENE_H
#define NOAENGINE_SCENE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Sprite.h"

using namespace std;

namespace noa {
	typedef unsigned char Uint8;

	typedef struct MapFile
	{
		//地图文件
		int w;
		int h;
		vector<uint32_t> image;
	}Map;


	//加载地图
	extern MapFile LoadMap(const char* fileName);

	extern MapFile LoadMapFromCSV(const std::string filename);

	class LevelMap {
	public:
		vector<uint32_t> level;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(MapFile & map);
	};

	//瓦片地图
	typedef struct Tile 
	{
		Sprite sprite;
		bool isCollision = false;

		/*Tile(Sprite sprite,bool isCollision) 
		{
			this->sprite = sprite;
			this->isCollision = isCollision;
		}

		Tile(SpriteFile spriteFile, bool isCollision)
		{
			this->sprite = Sprite(spriteFile,1);
			this->isCollision = isCollision;
		}*/

	}Tile;

	typedef struct PixelData {
		int id;
		SpriteFile sprites;
	}PixelData;

	//extern vector<PixelData> LoadTileFromTsd(const std::string& fileName);

	extern unordered_map<Uint32, Tile> LoadTileFromTsd(const std::string& fileName);

	//瓦片地图
	class TileMap:public LevelMap
	{
	public:
		unordered_map<Uint32, Tile> tileSet;
	public:
		/// <summary>
		/// 加载瓦片地图
		/// </summary>
		/// <param name="tileSet">瓦片集合</param>
		/// <param name="map">地图数据</param>
		TileMap(unordered_map<Uint32,Tile> tileSet,MapFile map);
	};

}



#endif // !NOAENGINE_SCENE_H


