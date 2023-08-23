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
		LevelMap(MapFile & map);
	};

	//��Ƭ��ͼ
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

	//��Ƭ��ͼ
	class TileMap:public LevelMap
	{
	public:
		unordered_map<Uint32, Tile> tileSet;
	public:
		/// <summary>
		/// ������Ƭ��ͼ
		/// </summary>
		/// <param name="tileSet">��Ƭ����</param>
		/// <param name="map">��ͼ����</param>
		TileMap(unordered_map<Uint32,Tile> tileSet,MapFile map);
	};

}



#endif // !NOAENGINE_SCENE_H


