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
#include "Graphic.h"
#include "Transform.h"
namespace noa {

	enum class InstanceLayer {
		MAIN_LAYER = 0,		//Ĭ��ͼ��
		GAME_LAYER = 1,		//��Ϸͼ��
		UI_LAYER = 2			//UIͼ��
	};

	typedef unsigned int Uint32;

	//������ͼ����CPU���л�ͼ��ʾ
	class Sprite final {

	public:
		//��ʾ����Ļ�е�λ��
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
		Sprite(const SpriteFile & sprFile, const Vector<int>& scale);
		Sprite(const Vector<int> & scale);
		Sprite(const char* file, const Vector<int>& scale);
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
		//ȫ������ͼƬ
		void DrawSpriteFull();

		Uint32 GetPixelColor(const int x,const int y) const;
		Uint32 GetColor(const float normalizedX,const float normalizedY) const;
		Uint32 GetTransposeColor(const float normalizedX,const float normalizedY) const;
		Uint32 GetTransposeColor(const Vector<float>& simple) const;

	};

	class Texture;
	// ʹ��GPU��ȾͼƬ��ǿ��Ҫ����봴����NoaEngine֮��
	class SpriteGPU final {
	private:
		Sprite* sprite = nullptr;

		Texture* texture = nullptr;

		InstanceLayer layer = InstanceLayer::MAIN_LAYER;

	public:
		SpriteGPU(Sprite * sprite);

		~SpriteGPU();

		void SetLayer(InstanceLayer layer);

		void Update(Sprite* sprite);

		void DrawSprite(float x, float y,bool mirror = false,float eulerAngle = 0.0f);

		void DrawSprite(float x, float y,float w,float h, bool mirror = false, float eulerAngle = 0.0f);

	};

}



#endif
