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

#include "NoaMath.h"
#include "Resource.h"
#include "Transform.h"

/*
* Sprite和SpriteGPU
* 都是游戏的重要资源
* 游戏引擎迫切的需要对资源进行统一管理
*/

namespace noa {

	class Texture;
	typedef struct SpriteGPUInstance
	{
		Texture* texture = nullptr;
		uint32_t tint = 0xFFFFFFFF;
		Vector<int> position;
		Vector<int> scale;
		float eulerAngle = 0.0f;
		bool flip = false;

	}SpriteGPUInstance;

	extern std::vector<std::vector<SpriteGPUInstance>> rendererInstanceLayer;

	enum class InstanceLayer {
		MAIN_LAYER = 0,		//默认图层
		GAME_LAYER = 1,		//游戏图层
		UI_LAYER = 2		//UI图层
	};

	class Sprite final {

	public:
		//显示再屏幕中的位置
		int posx = 0;
		int posy = 0;
		int w = 1024;
		int h = 1024;
		Vector<int> scale = Vector<int>(32, 32);
		bool isEmpty = false;

	private:
		std::vector<uint32_t> image;

		int sw = 2;
		int sh = 2;

	public:
		Sprite(const SpriteFile & sprFile, const Vector<int>& scale);
		Sprite(const Vector<int> & scale);
		Sprite(const std::string & filename, const Vector<int>& scale);
		Sprite(const std::string & filename, int scaleX,int scaleY);
		Sprite();
		Sprite(int w, int h,const Vector<int> & scale,const std::vector<uint32_t> & image);
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
		void SetPixelColor(int x,int y,uint32_t color);

		uint32_t GetPixelColor(const int x,const int y) const;
		uint32_t GetColor(const float normalizedX,const float normalizedY) const;
		uint32_t GetTransposeColor(const float normalizedX,const float normalizedY) const;
		uint32_t GetTransposeColor(const Vector<float>& simple) const;

	};

	class Texture;

	class SpriteGPU final
	{
	private:
		friend class Image;
		friend class SpriteRenderer;
		friend class StaticCamera;
		friend struct Font;
		friend struct Tile;

	private:
		Texture* texture = nullptr;
		InstanceLayer layer = InstanceLayer::MAIN_LAYER;
		noa::Vector<int> scale;
	public:
		SpriteGPU(Sprite * sprite);
		SpriteGPU(const SpriteFile & spriteFile,int scaleX,int scaleY);
		~SpriteGPU();
	private:
		static std::shared_ptr<SpriteGPU> Create(Sprite * sprite);
		static std::shared_ptr<SpriteGPU> Create(const SpriteFile& spriteFile, int scaleX, int scaleY);
	public:

		void SetLayer(InstanceLayer layer);

		void Update(Sprite* sprite);

		void DrawSprite(float x = 0, float y = 0,bool mirror = false,float eulerAngle = 0.0f);

		void DrawSprite(float x, float y,float w,float h, unsigned int tint, bool mirror, float eulerAngle);

	};

}



#endif
