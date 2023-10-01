#ifndef NOAENGINE_RENDERER_H
#define NOAENGINE_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "Graphic.h"
#include "NoaMath.h"



namespace noa {

	class Texture;
	class Sprite;

	class Renderer
	{
	public:

	protected:
		int pixelWidth;
		int pixelHeight;
		Uint32* pixelBuffer;

		float invPixelWidth;
		float invPixelHeight;

	public:
		Renderer();
		void SetRenderer(int pixelWidth, int pixelHeight, Uint32* pixelBuffer);
		//绘制像素点
		void DrawPixel(const uint32_t x,const uint32_t y,const uint32_t color) const;
		void DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const;
		void DrawLine(const Vector<int> & point1,const Vector<int> & point2, Uint32 color) const;
		/// <summary>
		/// 绘制矩形
		/// </summary>
		/// <param name="point1"></param>
		/// <param name="point2"></param>
		/// <param name="color"></param>
		void DrawRect(const Vector<int> & point1,const Vector<int> & point2,Uint32 color) const;
		/// <summary>
		/// 将sprite填充到矩形
		/// </summary>
		/// <param name="point1"></param>
		/// <param name="point2"></param>
		/// <param name="sprite"></param>
		void DrawFont(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite,uint32_t color) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite, Uint32 mutiColor, bool isAlpha) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite* sprite, Uint32 mutiColor, bool isAlpha) const;
		/// <summary>
		/// 绘制字符串到屏幕上，
		/// 引擎目前仅仅支持绘制英文字母到屏幕上
		/// </summary>
		/// <param name="str">字符串</param>
		/// <param name="x">屏幕坐标x</param>
		/// <param name="y">屏幕坐标y</param>
		/// <param name="color">字体颜色</param>
		/// <param name="size">字体大小</param>
		void DrawString(const std::string& str, int x, int y, Uint32 color, int size);
		void DrawString(const std::string& str, int x, int y,float narrow, Uint32 color, int size);

		void FullScreen(Uint32 color) const;

		virtual void InitRenderer() = 0;
		virtual Texture* CreateTexture(int w, int h, uint32_t* pixelBuffer) = 0;
		virtual void DrawTexture(Texture* texture,int index, int x, int y, int w, int h, float eulerAngle = 0, bool isFlipX = false) = 0;
		virtual void Clear() = 0;
		virtual void SetContext(SDL_Window * windows) = 0;
		virtual void Present(SDL_Window * windows) = 0;

	};
}


#endif



