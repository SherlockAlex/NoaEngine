#ifndef NOAENGINE_RESOURCE_H
#define NOAENGINE_RESOURCE_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace noa {
	//��Դ����ϵͳ
	
	//���صľ�����Դ�ļ�
	typedef struct SpriteFile
	{
		std::vector<uint32_t> images;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	}SpriteFile;

	//���ض����ļ�
	typedef struct AnimationFile {
		std::vector<SpriteFile> data;
		int posx = 0;
		int posy = 0;
		int w = 0;
		int h = 0;
	}AnimationFile;

	//���ص�ͼ�ļ�
	typedef struct MapFile
	{
		//��ͼ�ļ�
		int w = 0;
		int h = 0;
		std::vector<int> image;
	}Map;

	class Sprite;
	class SpriteGPU;
	//��Ƭ
	typedef struct Tile
	{
		std::shared_ptr<Sprite> sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
		bool isCollision = false;

		//���һ��Animation

		Tile();

		Tile(std::shared_ptr<Sprite> sprite, bool isCollision = false);

		Tile(SpriteFile spriteFile, bool isCollision = false);

		~Tile();


	}Tile;

	//��Ƭ����
	typedef struct TileData 
	{
		int id = -1;
		SpriteFile sprites;
	}TileData;

	class Resource {
	public:
		static SpriteFile LoadSprite(const char* file);

		static AnimationFile LoadAnimationClip(const char* file);

		static MapFile LoadTileMap(const std::string filename);

		static std::unordered_map<int, Tile> LoadTileSet(const std::string& fileName);

		static std::string ReadTextFrom(const std::string & filePath);

	};

}

#endif // !NOAENGINE_RESOURCE_H


