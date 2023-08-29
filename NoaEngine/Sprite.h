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

using namespace std;

namespace noa {
	typedef unsigned int Uint32;

	//���ص�sprFile
	typedef struct SpriteFile
	{
		vector<uint32_t> images;
		//uint32_t* images;
		int x;
		int y;
		int width;
		int height;
	}SpriteFile;

	//���ر��ص�spr�ļ�
	extern SpriteFile LoadSprFile(const char* file);

	//������ͼ
	class Sprite {

	public:
		//��ʾ����Ļ�е�λ��
		int posx = 0;
		int posy = 0;
		int w = 1;
		int h = 1;
		Vector<float> scale = Vector<float>(1.0, 1.0);
		//int sizeForSurface = 3;

	private:
		vector<Uint32> image;
		//Uint32* image = nullptr;

	public:
		Sprite(SpriteFile sprFile, Vector<float> scale);
		Sprite(const char* file, Vector<float> scale);
		Sprite();
		Sprite(int w, int h, Vector<float> scale, vector<Uint32> image);
		~Sprite();

	public:
		//void UpdateImage(Uint32* image);
		void UpdateImage(vector<Uint32> image);
		void UpdateImage(const SpriteFile & image);
		void DrawSprite(int posX, int posY, bool isRenderAlpha) const;
		void DrawSprite(int posX, int posY, bool isRenderAlpha, bool isMirror) const;
		void DrawSprite(bool isRenderAlpha) const;
		void DrawSprite(bool isRenderAlpha, bool isMirror) const;
		//ȫ������ͼƬ
		void DrawSpriteFull();

		Uint32 GetColor(float normalizedX, float normalizedY) const;
		Uint32 GetTransposeColor(float normalizedX, float normalizedY) const;
		Uint32 GetTransposeColor(const Vector<float>& simple) const;

	};
}



#endif
