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

		//��ȡ�ļ�����
		template<class T>
		T Read();

		//������д��������ļ�
		template<class T>
		bool Write(T value);

	};

	//��Դ��ȡ������ȡ������Ϸ������Դ
	extern Resource resource;

}

#endif // !NOAENGINE_RESOURCE_H


