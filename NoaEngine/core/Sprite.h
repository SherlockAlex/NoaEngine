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

#include <SDL2/SDL.h>

#include "NoaMath.h"
#include "Resource.h"

namespace noa {
	typedef unsigned int Uint32;

	//精灵贴图
	class Sprite {

	public:
		//显示再屏幕中的位置
		int posx = 0;
		int posy = 0;
		int w = 1024;
		int h = 1024;
		Vector<int> scale = Vector<int>(32, 32);
		//int sizeForSurface = 3;

	private:
		std::vector<Uint32> image;
		//Uint32* image = nullptr;

		int sw = 2;
		int sh = 2;

	public:
		Sprite(SpriteFile sprFile, Vector<int> scale);
		Sprite(Vector<int> scale);
		Sprite(const char* file, Vector<int> scale);
		Sprite();
		Sprite(int w, int h, Vector<int> scale, std::vector<Uint32> image);
		~Sprite();

	public:
		void UpdateImage(const SpriteFile & image);
		void DrawSprite(int posX, int posY, bool isRenderAlpha) const;
		void DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const;
		void DrawSprite(bool isRenderAlpha) const;
		void DrawSprite(bool isRenderAlpha, bool isMirror) const;
		//全屏绘制图片
		void DrawSpriteFull();

		Uint32 GetPixelColor(const int x,const int y) const;
		Uint32 GetColor(const float normalizedX,const float normalizedY) const;
		Uint32 GetTransposeColor(const float normalizedX,const float normalizedY) const;
		Uint32 GetTransposeColor(const Vector<float>& simple) const;

	};
}



#endif
