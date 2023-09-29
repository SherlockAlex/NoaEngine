#ifndef NOAENGINE_RENDERER_H
#define NOAENGINE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Graphic.h"
#include "NoaMath.h"

namespace noa {

	class Sprite;

	class Renderer
	{
	public:
		GRAPHIC API;

	private:
		int pixelWidth;
		int pixelHeight;
		Uint32* pixelBuffer;
		SDL_Renderer* sdlRenderer;
		SDL_Texture* sdlTexture;

	public:
		Renderer();
		Renderer(int pixelWidth, int pixelHeight, void* pixelBuffer,SDL_Renderer* sdlRenderer, SDL_Texture* sdlTexture);
		//�������ص�
		void DrawPixel(const uint32_t x,const uint32_t y,const uint32_t color) const;
		void DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const;
		void DrawLine(const Vector<int> & point1,const Vector<int> & point2, Uint32 color) const;
		/// <summary>
		/// ���ƾ���
		/// </summary>
		/// <param name="point1"></param>
		/// <param name="point2"></param>
		/// <param name="color"></param>
		void DrawRect(const Vector<int> & point1,const Vector<int> & point2,Uint32 color) const;
		/// <summary>
		/// ��sprite��䵽����
		/// </summary>
		/// <param name="point1"></param>
		/// <param name="point2"></param>
		/// <param name="sprite"></param>
		void DrawFont(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite,uint32_t color) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite, Uint32 mutiColor, bool isAlpha) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite* sprite, Uint32 mutiColor, bool isAlpha) const;
		/// <summary>
		/// �����ַ�������Ļ�ϣ�
		/// ����Ŀǰ����֧�ֻ���Ӣ����ĸ����Ļ��
		/// </summary>
		/// <param name="str">�ַ���</param>
		/// <param name="x">��Ļ����x</param>
		/// <param name="y">��Ļ����y</param>
		/// <param name="color">������ɫ</param>
		/// <param name="size">�����С</param>
		void DrawString(const std::string& str, int x, int y, Uint32 color, int size);
		void DrawString(const std::string& str, int x, int y,float narrow, Uint32 color, int size);

		void FullScreen(Uint32 color) const;
		
		void UpdateScreen();

		SDL_Renderer* GetSDLRenderer();

	};
}


#endif



