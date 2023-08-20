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

using namespace std;

namespace noa {
	typedef unsigned char Uint8;

	typedef struct MapFile
	{
		//地图文件
		int w;
		int h;
		uint8_t* image;
	}Map;


	//加载地图
	inline MapFile LoadMap(const char* fileName)
	{
		MapFile map;

		std::ifstream file(fileName, std::ios::binary);
		if (!file)
		{
			std::cout << "无法打开文件" << std::endl;
			map.image = nullptr;
			return map;
		}

		file.read(reinterpret_cast<char*>(&map.w), sizeof(int));
		file.read(reinterpret_cast<char*>(&map.h), sizeof(int));

		int imageDataSize = map.w * map.h * sizeof(uint8_t);
		map.image = new uint8_t[imageDataSize];

		file.read(reinterpret_cast<char*>(map.image), imageDataSize);

		file.close();

		return map;
	}

	class LevelMap {
	private:


	public:
		Uint8* level = nullptr;
		int w = 0;
		int h = 0;
	public:
		LevelMap() {

		}

		LevelMap(MapFile map)
		{
			this->w = map.w;
			this->h = map.h;

			level = map.image;

		}

		LevelMap(int width, int height, wstring map)
		{
			this->w = width;
			this->h = height;
			level = new Uint8(width * height);

			//加载地图到缓存中
			for (int i = 0; i < map.length(); i++)
			{
				level[i] = map[i];
			}

		}

		void LoadMap(wstring map)
		{
			for (int i = 0; i < map.length(); i++)
			{
				level[i] = map[i];
			}
		}

		std::wstring LoadLevel(const char* filePath) {
			std::wstring content;

			std::ifstream file(filePath, std::ios::binary);

			if (file.is_open()) {
				file.seekg(0, std::ios::end);
				size_t length = file.tellg();
				file.seekg(0, std::ios::beg);

				char* buffer = new char[length];
				file.read(buffer, length);

				content = std::wstring(reinterpret_cast<const wchar_t*>(buffer), length / sizeof(wchar_t));

				delete[] buffer;

				file.close();
			}
			else {
				std::cerr << "无法打开文件。" << std::endl;
			}

			return content;
		}

	};
}



#endif // !NOAENGINE_SCENE_H


