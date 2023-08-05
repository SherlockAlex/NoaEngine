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

typedef unsigned char Uint8;

typedef struct MapFile
{
	int w;
	int h;
	uint8_t* image;
}Map;


//���ص�ͼ
inline MapFile LoadMap(const char* fileName)
{
	MapFile map;

	std::ifstream file(fileName, std::ios::binary);
	if (!file)
	{
		std::cout << "�޷����ļ�" << std::endl;
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
	int w;
	int h;
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

		//level = (Uint8*)malloc(sizeof(Uint8) * width * height);
		level = new Uint8(width * height);

		//���ص�ͼ��������
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
			std::cerr << "�޷����ļ���" << std::endl;
		}

		return content;
	}

	void SaveLevel(const std::wstring& map, const char* filePath) {
		std::ofstream file(filePath, std::ios::binary);

		if (file.is_open()) {
			const char* bytes = reinterpret_cast<const char*>(map.c_str());
			size_t length = sizeof(wchar_t) * map.length();

			file.write(bytes, length);
			file.close();

			std::cout << "�����ѳɹ����浽�������ļ��С�" << std::endl;
		}
		else {
			std::cerr << "�޷����ļ���" << std::endl;
		}
	}

};

#endif // !NOAENGINE_SCENE_H


