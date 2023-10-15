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

	class Resource {
	public:
		SpriteFile LoadSprFile(const char* file) const;

		AnimationFile LoadAnimationFile(const char* file) const;

		MapFile LoadMapFromCSV(const std::string filename) const;

		std::unordered_map<int, Tile> LoadTileFromTsd(const std::string& fileName) const;

	};

	class BinaryFile 
	{
	public:
		enum Mode {
			READ = 0,
			WRITE = 1
		};

	private:
		std::ifstream ifile;
		std::ofstream ofile;
	public:
		BinaryFile(const char * fileName,Mode mode);
		~BinaryFile();

		//读取文件数据
		template<class T>
		T Read();

		//将数据写入二进制文件
		template<class T>
		bool Write(T value);

	};

	//资源读取器，读取本地游戏引擎资源
	extern Resource resource;

}

#endif // !NOAENGINE_RESOURCE_H


