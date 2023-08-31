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
		if (x<0||x>=pixelWidth||y<0||y>=pixelHeight||pixelBuffer == nullptr)
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
				const uint32_t color = sprite.GetTransposeColor(simple.y,simple.x);
				DrawPixel(x, y, color);
			}
		}
	}

	void Renderer::DrawString(int x, int y, const string& str, Uint32 color, const int size)
	{
		//绘制字符串到屏幕上

	}

	void Renderer::FullScreen(Uint32 color) const
	{
		return;
	}

}

