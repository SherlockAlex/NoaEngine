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
#include <memory>

namespace noa {
	//资源管理系统
	
	//本地的精灵资源文件
	typedef struct SpriteFile
	{
		std::vector<uint32_t> images;
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	}SpriteFile;

	//本地动画文件
	typedef struct AnimationFile {
		std::vector<SpriteFile> data;
		int posx = 0;
		int posy = 0;
		int w = 0;
		int h = 0;
	}AnimationFile;

	//本地地图文件
	typedef struct MapFile
	{
		//地图文件
		int w = 0;
		int h = 0;
		std::vector<int> image;
	}Map;

	
	class Sprite;
	class SpriteGPU;
	//瓦片
	typedef struct Tile
	{
		std::shared_ptr<Sprite> sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
		bool isCollision = false;

		//添加一个Animation

		Tile();

		Tile(std::shared_ptr<Sprite> sprite, bool isCollision = false);

		Tile(SpriteFile spriteFile, bool isCollision = false);

		~Tile();


	}Tile;

	//瓦片数据
	typedef struct TileData 
	{
		int id = -1;
		SpriteFile sprites;
	}TileData;

	class AudioClip;
	class Resource {
	public:
		static const std::string path;
	public:
		static std::shared_ptr<AudioClip> LoadAudioClip(const std::string & filename);

		static SpriteFile LoadSprite(const std::string & filename);

		static AnimationFile LoadAnimationClip(const std::string & filename);

		static MapFile LoadTileMap(const std::string & filename);

		static std::unordered_map<int, Tile> LoadTileSet(const std::string& filename);

		static std::string ReadTextFrom(const std::string & filename);

	};

}

#endif // !NOAENGINE_RESOURCE_H


