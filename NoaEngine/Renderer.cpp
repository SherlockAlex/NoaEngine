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
		if (x<0||x>=pixelWidth||y<0||y>=pixelHeight)
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
		int x1 = point1.x;
		int y1 = point1.y;
		int x2 = point2.x;
		int y2 = point2.y;

		for (int x = min(x1, x2); x <= max(x1, x2); x++) {
			for (int y = min(y1, y2); y <= max(y1, y2); y++) {
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

		for (int x = min(x1, x2); x <= max(x1, x2); x++) 
		{
			for (int y = min(y1, y2); y <= max(y1, y2); y++) 
			{
				const Vector<float> simple(
					(float)(x - x1) / (x2 - x1), 
					(float)(y - y1) / (y2 - y1)
				);
				const uint32_t color = sprite.GetTransposeColor(simple.y,simple.x);
				DrawPixel(x, y, color);
			}
		}
	}

	void Renderer::DrawString(int x, int y, const string& str, Uint32 color, const int size)
	{
		//绘制字符串到屏幕上

	}

	void Renderer::DrawImage(int posX, int posY, int imageW, int imageH, int scaleForSurface, bool isDrawAlpha, vector<Uint32> imageRGB) const
	{
		//用于2维视角的游戏
		//计算放大
		const int wannaW = pixelWidth / scaleForSurface;
		const int wannaH = (int)(((float)imageH / (float)imageW) * wannaW);

		const Sprite sprite = Sprite(imageW, imageH, Vector<float>(1.0,1.0), imageRGB);

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

	void Renderer::FullScreen(Uint32 color) const
	{
		return;
	}

}

