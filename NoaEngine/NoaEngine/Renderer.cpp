#include "Renderer.h"
#include "Sprite.h"
#include "NoaGUI.h"
#include "NoaMath.h"

#include <algorithm>

using namespace std;

namespace noa {
	extern void Debug(string msg);

	std::shared_ptr<FontAsset> fontAsset;

	Renderer::Renderer()
	{
		
	}

	void Renderer::InitFontAsset() {
		// 初始化字体
		fontAsset = make_shared<FontAsset>("./Assets/font/font.ttf",64);
	}

	void Renderer::SetRenderer(int pixelWidth, int pixelHeight, Uint32* pixelBuffer) {
		this->pixelWidth = pixelWidth;
		this->pixelHeight = pixelHeight;
		this->pixelBuffer = (Uint32*)pixelBuffer;

		this->invPixelWidth = 1.0f / pixelWidth;
		this->invPixelHeight = 1.0f / pixelHeight;
	}

	void Renderer::DrawPixel(const uint32_t x, const uint32_t y, const uint32_t color) const
	{
		if (x >= static_cast<uint32_t>(pixelWidth) || y >= static_cast<uint32_t>(pixelHeight))
		{
			return;
		}

		pixelBuffer[y * pixelWidth + x] = color;

	}

	void Renderer::DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const
	{
		const int dx = NoaFastAbs(x2 - x1);
		const int dy = NoaFastAbs(y2 - y1);
		const int sx = (x1 < x2) ? 1 : -1;
		const int sy = (y1 < y2) ? 1 : -1;

		int err = dx - dy;

		while (true)
		{

			DRAWPIXEL(x1, y1, color);

			if (x1 == x2 && y1 == y2)
			{
				break;
			}

			const int e2 = err * 2;

			if (e2 > -dy)
			{
				err -= dy;
				x1 += sx;
			}

			if (e2 < dx)
			{
				err += dx;
				y1 += sy;
			}

		}

	}

	void Renderer::DrawLine(const Vector<int> & point1,const Vector<int> & point2, Uint32 color) const
	{

		int x1 = point1.x;
		int x2 = point2.x;

		int y1 = point1.y;
		int y2 = point2.y;

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		int err = dx - dy;

		while (true)
		{
			//DrawPixel(x1, y1, color);
			DRAWPIXEL(x1, y1, color);
			if (x1 == x2 && y1 == y2)
			{
				break;
			}

			int e2 = err * 2;

			if (e2 > -dy)
			{
				err -= dy;
				x1 += sx;
			}

			if (e2 < dx)
			{
				err += dx;
				y1 += sy;
			}
			

		}
	}

	void Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Uint32 color) const
	{
		const int x1 = point1.x;
		const int y1 = point1.y;
		const int x2 = point2.x;
		const int y2 = point2.y;


		const int minX = Min(x1, x2);
		const int maxX = Max(x1, x2);
		const int minY = Min(y1, y2);
		const int maxY = Max(y1, y2);

		for (int x = minX; x <= maxX; x++) {
			for (int y = minY; y <= maxY; y++) {
				DRAWPIXEL(x, y, color);
			}
		}
	}

	void Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite) const
	{
		//将sprite图片填充到矩形上
		const int x1 = point1.x;
		const int y1 = point1.y;
		const int x2 = point2.x;
		const int y2 = point2.y;

		const int minX = Min(x1, x2);
		const int maxX = Max(x1, x2);
		const int minY = Min(y1, y2);
		const int maxY = Max(y1, y2);

		for (int x = minX; x <= maxX; x++) 
		{
			for (int y = minY; y <= maxY; y++) 
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1), 
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = sprite.GetColor(simple.x, simple.y);
				DRAWPIXEL(x, y, color);
			}
		}
	}

	void Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite& sprite,Uint32 mutiColor,bool isAlpha) const
	{
		//将sprite图片填充到矩形上
		const int x1 = point1.x;
		const int y1 = point1.y;
		const int x2 = point2.x;
		const int y2 = point2.y;

		const int minX = Min(x1, x2);
		const int maxX = Max(x1, x2);
		const int minY = Min(y1, y2);
		const int maxY = Max(y1, y2);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = sprite.GetColor(simple.x, simple.y);
				if (isAlpha&& GetAValue(color) == 0)
				{
					continue;
				}

				DRAWPIXEL(x, y, mutiColor);
			}
		}
	}

	void Renderer::DrawRect(const Vector<int>& point1, const Vector<int>& point2, Sprite * sprite, Uint32 mutiColor, bool isAlpha) const
	{
		//将sprite图片填充到矩形上
		const int x1 = point1.x;
		const int y1 = point1.y;
		const int x2 = point2.x;
		const int y2 = point2.y;

		const int minX = Min(x1, x2);
		const int maxX = Max(x1, x2);
		const int minY = Min(y1, y2);
		const int maxY = Max(y1, y2);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				uint32_t color = sprite->GetColor(simple.x, simple.y);
				
				if (isAlpha && GetAValue(color) == 0)
				{
					continue;
				}

				color = RGBA(GetRValue(color) * (GetRValue(mutiColor) / 255.0), GetGValue(color) * (GetGValue(mutiColor) / 255.0), GetBValue(color) * (GetBValue(mutiColor) / 255.0),GetAValue(color));

				DRAWPIXEL(x, y, color);
			}
		}
	}

	void Renderer::DrawString(const std::string& str, int x, int y, Uint32 color, int size)
	{
		int row = 0;
		int offset = 0;

		const float narrowx = 0.8f;
		const int length = static_cast<int>(str.length());
		const char* c_str = str.c_str();
		for (int i=0;i<length;i++) 
		{
			const char c = c_str[i];
			if (c == '\n')
			{
				row++;
				offset = 0;
				continue;
			}
			const Font* font = fontAsset->GetFont(c);
			if (font == nullptr)
			{
				continue;
			}
			
			const Vector<float>& position = { static_cast<float>(x + (offset * size) * narrowx),static_cast<float>(y + row * size) };
			font->spriteGPU->DrawSprite(position.x, position.y,size,size,color,false,0.0f);

			offset++;
		}
	}

	void Renderer::DrawString(const std::string& str, int x, int y, float narrowx, Uint32 color, int size)
	{
		int row = 0;
		int offset = 0;

		const int length = static_cast<int>(str.length());
		const char* c_str = str.c_str();
		for (int i = 0; i < length; i++)
		{
			const char c = c_str[i];
			if (!(c ^ '\n'))
			{
				row++;
				offset = 0;
			}
			const Font* font = fontAsset->GetFont(c);
			if (font == nullptr)
			{
				continue;
			}
			
			const Vector<float>& position = { static_cast<float>(x + (offset * size) * narrowx),static_cast<float>(y + row * size) };
			font->spriteGPU->DrawSprite(position.x, position.y, size, size, color,false,0.0f);
			
			offset++;
		}
	}

	void Renderer::FullScreen(Uint32 color) const
	{
		for (int i=0;i<pixelHeight * pixelWidth;i++)
		{
			pixelBuffer[i] = color;
		}
	}

}

