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
		//��ͼ�ļ�
		int w;
		int h;
		vector<uint32_t> image;
	}Map;


	//���ص�ͼ
	extern MapFile LoadMap(const char* fileName);

	extern MapFile LoadMapFromCSV(const std::string filename);

	class LevelMap {
	public:
		vector<uint32_t> level;
		int w = 0;
		int h = 0;
	public:
		LevelMap();
		LevelMap(const MapFile & map);
	};

	//��Ƭ��ͼ
	typedef struct Tile {
		Sprite sprite;
		bool isCollision = false;
		Tile(Sprite sprite,bool isCollision = false) 
		{
			this->sprite = sprite;
			this->isCollision = isCollision;
		}
	}Tile;

	//��Ƭ��ͼ
	class TileMap:public LevelMap
	{
	private:
		unordered_map<Uint8, Tile> tileSet;
	public:
		TileMap(unordered_map<Uint8,Tile> tileSet,MapFile map);
	};

}



#endif // !NOAENGINE_SCENE_H


