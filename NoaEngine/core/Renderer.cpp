#include "Renderer.h"
#include "Sprite.h"
#include "NoaGUI.h"

namespace noa {
	extern void Debug(string msg);

	FontAsset fontAsset("./Assets/font/font.tsd");

	Renderer::Renderer()
	{
		pixelBuffer = nullptr;
		pixelHeight = 0;
		pixelWidth = 0;
		this->sdlRenderer = nullptr;
		this->sdlTexture = nullptr;
	}

	Renderer::Renderer(int pixelWidth, int pixelHeight, void* pixelBuffer, SDL_Renderer* sdlRenderer,SDL_Texture* sdlTexture)
	{
		this->pixelWidth = pixelWidth;
		this->pixelHeight = pixelHeight;
		this->pixelBuffer = (Uint32*)pixelBuffer;
		this->sdlRenderer = sdlRenderer;
		this->sdlTexture = sdlTexture;

	}

	void Renderer::DrawPixel(const int x, const int y, const Uint32 color) const
	{
		if (x<0||x>=pixelWidth||y<0||y>=pixelHeight||pixelBuffer == nullptr)
		{
			return;
		}
		pixelBuffer[y * pixelWidth + x] = color;

		

	}

	void Renderer::DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const
	{
		int dx = NoaFastAbs(x2 - x1);
		int dy = NoaFastAbs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		int err = dx - dy;

		while (true)
		{
			DrawPixel(x1, y1, color);

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
			DrawPixel(x1, y1, color);

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


		const int minX = min(x1, x2);
		const int maxX = max(x1, x2);
		const int minY = min(y1, y2);
		const int maxY = max(y1, y2);

		for (int x = minX; x <= maxX; x++) {
			for (int y = minY; y <= maxY; y++) {
				DrawPixel(x, y, color);
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

		const int minX = min(x1, x2);
		const int maxX = max(x1, x2);
		const int minY = min(y1, y2);
		const int maxY = max(y1, y2);

		for (int x = minX; x <= maxX; x++) 
		{
			for (int y = minY; y <= maxY; y++) 
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1), 
					(float)(y - y1) / (y2 - y1)
				);
				//const uint32_t color = RED;
				const uint32_t color = sprite.GetTransposeColor(simple.y,simple.x);
				DrawPixel(x, y, color);
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

		const int minX = min(x1, x2);
		const int maxX = max(x1, x2);
		const int minY = min(y1, y2);
		const int maxY = max(y1, y2);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = sprite.GetTransposeColor(simple.y, simple.x);
				if (isAlpha)
				{
					if (color == ERRORCOLOR)
					{
						continue;
					}
				}



				DrawPixel(x, y, mutiColor);
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

		const int minX = min(x1, x2);
		const int maxX = max(x1, x2);
		const int minY = min(y1, y2);
		const int maxY = max(y1, y2);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1),
					(float)(y - y1) / (y2 - y1)
				);
				uint32_t color = sprite->GetTransposeColor(simple.y, simple.x);
				if (isAlpha)
				{
					if (color == ERRORCOLOR)
					{
						continue;
					}
				}

				color = RGB(GetRValue(color) * (GetRValue(mutiColor) / 255.0), GetGValue(color) * (GetGValue(mutiColor) / 255.0), GetBValue(color) * (GetBValue(mutiColor) / 255.0));

				DrawPixel(x, y, color);
			}
		}
	}

	void Renderer::DrawString(const std::string& str, int x, int y, Uint32 color, int size)
	{
		int row = 0;
		int offset = 0;

		const float narrowx = 0.6;
		const int length = str.length();
		const char* c_str = str.c_str();
		for (int i=0;i<length;i++) 
		{
			const char c = c_str[i];
			if (!(c^'\n'))
			{
				row++;
				offset = 0;
			}
			const Font* font = fontAsset[c];
			if (font == nullptr)
			{
				continue;
			}
			
			
			const Vector<int> point1 = move(Vector<int>(x+(offset *size)* narrowx, y + row * size));
			const Vector<int> point2 = move(Vector<int>(x+size+(offset *size)* narrowx, y+size + row * size));
			DrawRect(point1,point2,*font->sprite,color,true);
			offset++;
		}
	}

	void Renderer::FullScreen(Uint32 color) const
	{
		for (int x = 0;x<pixelWidth*pixelHeight;x++) 
		{
			pixelBuffer[x] = color;
		}
	}

	void Renderer::UpdateScreen()
	{
		SDL_UnlockTexture(sdlTexture);
		SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
		SDL_RenderPresent(sdlRenderer);
	}

}

