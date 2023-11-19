#ifndef NOAENGINE_RENDERER_H
#define NOAENGINE_RENDERER_H

#include <memory>
#include <string>
#include <vector>

#include "SDLHelper.h"
#include "NoaMath.h"



namespace noa {

	class Texture;
	class Sprite;

	class Renderer
	{
	public:

	protected:
		int pixelWidth = 0;
		int pixelHeight = 0;
		void* pixelBuffer = nullptr;

		float invPixelWidth = 0;
		float invPixelHeight = 0;

	public:
		Renderer();

		void InitFontAsset();

		void SetRenderer(int pixelWidth, int pixelHeight);

		void DrawPixel(const uint32_t x,const uint32_t y,const uint32_t color) const;
		void DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const;
		void DrawLine(const Vector<int> & point1,const Vector<int> & point2, Uint32 color) const;

		void DrawRect(const Vector<int> & point1,const Vector<int> & point2,Uint32 color) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite & sprite) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite, Uint32 mutiColor, bool isAlpha) const;
		void DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite* sprite, Uint32 mutiColor, bool isAlpha) const;

		void DrawString(const std::wstring& str, int x, int y, Uint32 color, int size);
		Vector<int> GetLabelScale(const std::wstring& str, int size);

		void FullScreen(Uint32 color) const;

		virtual void InitRenderer() = 0;
		virtual Texture* CreateTexture(int w, int h, void* pixelBuffer) = 0;
		virtual void DrawTexture(Texture* texture, int index, int x, int y, int w, int h, unsigned int tint, float eulerAngle, bool isFlipX) = 0;
		virtual void Clear() = 0;
		virtual void SetContext(uint32_t windowID) = 0;
		virtual void Present(uint32_t windowID) = 0;

	};

	extern std::shared_ptr<Renderer> renderer;

}


#endif



