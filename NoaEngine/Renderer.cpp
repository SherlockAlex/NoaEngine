#include "Renderer.h"
#include "Sprite.h"

namespace noa {
	Renderer::Renderer()
	{
		pixelBuffer = nullptr;
		pixelHeight = 0;
		pixelWidth = 0;
	}

	Renderer::Renderer(int pixelWidth, int pixelHeight, void* pixelBuffer)
	{
		this->pixelWidth = pixelWidth;
		this->pixelHeight = pixelHeight;
		this->pixelBuffer = (Uint32*)pixelBuffer;
	}

	void Renderer::DrawPixel(const int x, const int y, const Uint32 color) const
	{
		if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
		{
			return;
		}
		pixelBuffer[y * pixelWidth + x] = color;
	}

	void Renderer::DrawLine(int x1, int y1, int x2, int y2, Uint32 color) const
	{
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

	void Renderer::DrawString(int x, int y, const string& str, Uint32 color, const int size)
	{
		//绘制字符串到屏幕上

	}

	void Renderer::DrawImage(int posX, int posY, int imageW, int imageH, int scaleForSurface, bool isDrawAlpha, Uint32* imageRGB) const
	{
		//用于2维视角的游戏
		//计算放大
		const int wannaW = pixelWidth / scaleForSurface;
		const int wannaH = (int)(((float)imageH / (float)imageW) * wannaW);

		const Sprite sprite = Sprite(imageW, imageH, 1, imageRGB);

		for (int width = 0; width < wannaW; width++)
		{
			for (int height = 0; height < wannaH; height++)
			{
				const int x = posX - ((float)wannaW * 0.5f) + width;
				const int y = posY - ((float)wannaH * 0.5f) + height;

				const float fSimpleX = (float)(width) / (float)wannaW;
				const float fSimpleY = (float)(height) / (float)wannaH;

				//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, imageH, inmageW, imageRGB);
				const Uint32 pixelColor = sprite.GetColor(fSimpleY, fSimpleX);
				if (isDrawAlpha)
				{
					if (pixelColor == BLACK)
					{
						continue;
					}
				}

				if (x < 0 || x >= pixelWidth || y < 0 || y >= pixelHeight)
				{
					continue;
				}
				DrawPixel(x, y, pixelColor);
			}
		}
	}

}

