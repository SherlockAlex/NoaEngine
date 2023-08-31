#ifndef NOAENGINE_RENDERER__H
#define NOAENGINE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "NoaMath.h"
using namespace std;

namespace noa {

	// Color constant
#define	BLACK			0x000000
#define	BLUE			0xAA0000
#define	GREEN			0x00AA00
#define	CYAN			0xAAAA00
#define	RED				0x0000AA
#define	MAGENTA			0xAA00AA
#define	BROWN			0x0055AA
#define	LIGHTGRAY		0xAAAAAA
#define	DARKGRAY		0x555555
#define	LIGHTBLUE		0xFF5555
#define	LIGHTGREEN		0x55FF55
#define	LIGHTCYAN		0xFFFF55
#define	LIGHTRED		0x5555FF
#define	LIGHTMAGENTA	0xFF55FF
#define	YELLOW			0x55FFFF
#define	WHITE			0xFFFFFF

//ȡ��λ
#define LOBYTE(w)           ((Uint8)(w&0xff))

// Color conversion macro
#define BGR(color)	((((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16))

#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

	class Sprite;

	class Renderer
	{
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
		void DrawPixel(int x, int y, Uint32 color) const;
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
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite, Uint32 mutiColor, bool isAlpha) const;

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
		
		void FullScreen(Uint32 color) const;
		

	};
}


#endif



