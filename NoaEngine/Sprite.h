#ifndef NOAENGINE_SPRITE_H
#define NOAENGINE_SPRITE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>
using namespace std;

namespace noa {
	typedef unsigned int Uint32;

	//本地的sprFile
	typedef struct SpriteFile
	{
		uint32_t* images;
		int x;
		int y;
		int width;
		int height;
	}SpriteFile;

	//加载本地的spr文件
	inline SpriteFile LoadSprFile(const char* file)
	{
		SpriteFile spriteFile;
		spriteFile.images = nullptr;

		std::ifstream input(file, std::ios::binary);
		if (input)
		{
			uint32_t imagesCount;
			input.read(reinterpret_cast<char*>(&imagesCount), sizeof(uint32_t));

			spriteFile.images = new uint32_t[imagesCount];
			for (int i = 0; i < imagesCount; ++i)
			{
				input.read(reinterpret_cast<char*>(&spriteFile.images[i]), sizeof(uint32_t));
			}

			input.read(reinterpret_cast<char*>(&spriteFile.x), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.y), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.width), sizeof(int));
			input.read(reinterpret_cast<char*>(&spriteFile.height), sizeof(int));

			input.close();
		}
		else
		{
			std::cerr << "读取失败：" << file << std::endl;
		}

		return spriteFile;
	}

	//精灵贴图
	class Sprite {

	public:
		//显示再屏幕中的位置
		int posx = 0;
		int posy = 0;
		int w = 1;
		int h = 1;
		int sizeForSurface = 3;

	private:
		Uint32* image = nullptr;

	public:
		Sprite(SpriteFile sprFile, int sizeForSurface);
		Sprite(const char* file, int sizeForSurface);
		Sprite();
		Sprite(int w, int h, int size, Uint32* image);
		~Sprite();

	public:
		void UpdateImage(Uint32* image);
		void UpdateImage(SpriteFile image);
		void DrawSprite(int posX, int posY, bool isRenderAlpha) const;
		void DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const;
		void DrawSprite(bool isRenderAlpha) const;
		void DrawSprite(bool isRenderAlpha, bool isMirror) const;

		Uint32 GetColor(float normalizedX, float normalizedY) const;
		Uint32 GetTransposeColor(float normalizedX, float normalizedY) const;

	};
}



#endif
