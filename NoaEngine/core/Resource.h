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

using namespace std;

namespace noa {
	//��Դ����ϵͳ
	
	//���صľ�����Դ�ļ�
	typedef struct SpriteFile
	{
		vector<uint32_t> images;
		int x;
		int y;
		int width;
		int height;
	}SpriteFile;

	//���ض����ļ�
	typedef struct AnimationFile {
		vector<SpriteFile> data;
		int posx;
		int posy;
		int w;
		int h;
	}AnimationFile;

	//���ص�ͼ�ļ�
	typedef struct MapFile
	{
		//��ͼ�ļ�
		int w = 0;
		int h = 0;
		vector<int> image;
	}Map;

	class Sprite;
	//��Ƭ
	typedef struct Tile
	{
		Sprite* sprite = nullptr;
		bool isCollision = false;

		//���һ��Animation

		Tile();

		Tile(Sprite* sprite, bool isCollision = false);

		Tile(SpriteFile spriteFile, bool isCollision = false);

	}Tile;

	//��Ƭ����
	typedef struct TileData {
		int id = -1;
		SpriteFile sprites;
	}TileData;

	class Resource {
	public:
		//���ر��ص�spr�ļ�
		SpriteFile LoadSprFile(const char* file) const;

		AnimationFile LoadAnimationFile(const char* file) const;
		//���ص�ͼ
		MapFile LoadMap(const char* fileName) const;

		MapFile LoadMapFromCSV(const std::string filename) const;

		unordered_map<int, Tile*> LoadTileFromTsd(const std::string& fileName) const;

	};

	class BinaryFile 
	{
	public:
		enum Mode {
			READ = 0,
			WRITE = 1
		};

	private:
		ifstream ifile;
		ofstream ofile;
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


