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
#include "Graphic.h"
#include "Transform.h"

namespace noa {

	typedef unsigned int Uint32;

	//精灵贴图，由CPU进行绘图显示
	class Sprite final {

	public:
		//显示再屏幕中的位置
		int posx = 0;
		int posy = 0;
		int w = 1024;
		int h = 1024;
		Vector<int> scale = Vector<int>(32, 32);
		bool isEmpty = false;
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
		std::vector<uint32_t> GetImage();
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

	// 使用GPU渲染图片，强制要求必须创建在NoaEngine之后
	class SpriteGPU final {
	private:
		Sprite* sprite = nullptr;

#pragma region SDL
		SDL_Texture* sdlTexture = nullptr;
		SDL_Rect srcRect = { 0,0,0,0 };
		SDL_Rect dstRect = { 0,0,0,0 };
#pragma endregion

		GLTexture* glTexture = nullptr;

	public:
		SpriteGPU(Sprite * sprite);

		~SpriteGPU();

		void Update(Sprite* sprite);

		void DrawSprite(float x, float y,bool mirror = false,float eulerAngle = 0.0f);

	};

}



#endif
